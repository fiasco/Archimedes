<?php
// $Id: cmis_sync.drupal.inc,v 1.1.2.14 2010/03/16 12:31:42 cbalan Exp $

/**
 * Handles Drupal to CMIS updates.
 */
function _cmis_sync_drupal_cmis_update($context = array()) {
  if (isset($context['node']) && isset($context['op'])) {
    $node = $context['node'];
    $op = $context['op'];
    
    if (in_array($op, array('insert', 'update', 'delete'))) {
      module_load_include('api.inc', 'cmis');
      
      $cmis_object = _cmis_sync_drupal_cmis_prepare($node);
      if ($cmis_object && !isset($node->cmis_sync_disabled)) {
        switch ($op) {
          // node created
          case 'insert' :
            try {
              $new_cmis_object = cmisapi_createDocument($cmis_object->repositoryId,
                                                      $cmis_object->folderId,
                                                      $cmis_object->name,
                                                      $cmis_object->properties,
                                                      $cmis_object->content,
                                                      $cmis_object->contentStreamMimeType); 
            }
            catch (CMISException $e) {
              cmis_error_handler('cmis_sync_nodeapi', $e);
              return;
            }
            
            // saving CMIS reference id in {cmis_sync_node} table
            db_query('INSERT INTO {cmis_sync_node} (nid, cmis_repositoryId, cmis_objectId, changed_timestamp) VALUES (%d, \'%s\', \'%s\', %d)',
                     $node->nid, $cmis_object->repositoryId, $new_cmis_object->id, $_SERVER['REQUEST_TIME']);
            break;
            
          // node updated 
          case 'update' :
            // send updates only if the current node has a CMIS reference id.
            if ($cmis_objectId = db_result(db_query('SELECT cmis_objectId FROM {cmis_sync_node} WHERE nid = %d', $node->nid))) {
              try {
                // updating CMIS reference object content stream
                if ($cmis_object->content) {
                  cmisapi_setContentStream($cmis_object->repositoryId, $cmis_objectId, $cmis_object->content, $cmis_object->contentStreamMimeType);
                }

                // updating CMIS properties
                cmisapi_updateProperties($cmis_object->repositoryId, $cmis_objectId, $cmis_object->properties);
                
                // update changed timestamp 
                db_query('UPDATE {cmis_sync_node} SET changed_timestamp=%d WHERE nid = %d', $_SERVER['REQUEST_TIME'], $node->nid);
              }
              catch (CMISException $e) {
                cmis_error_handler('cmis_sync_nodeapi', $e);
                return;
              }              
            }
            break;
          
          // node delete
          case 'delete' :
            if ($cmis_object->sync_deletes && $cmis_objectId = db_result(db_query('SELECT cmis_objectId FROM {cmis_sync_node} WHERE nid = %d', $node->nid))) {        
              try {
                // delete CMIS reference object content
                $cmis_object_properties = cmisapi_getProperties($cmis_object->repositoryId, $cmis_objectId);
                cmisapi_deleteObject($cmis_object->repositoryId, $cmis_object_properties->id);
                
                // delete sync refs 
                db_query('DELETE FROM {cmis_sync_node} WHERE nid = %d', $node->nid);
              }
              catch (CMISException $e) {
                cmis_error_handler('cmis_sync_nodeapi', $e);
                return;
              }
            }
            break;
        }
      }
    }
  }
}

/**
 * Maps a drupal node to a cmis_object
 * 
 * @param $node
 * @return stdClass $cmis_object wrapper
 */
function _cmis_sync_drupal_cmis_prepare($node) {
  $sync_map = variable_get('cmis_sync_map', array());

  // is cmis sync enabled for this node type?
  if ($sync_map[$node->type] && $sync_map[$node->type]['enabled']) {
    $cmis_object = new stdClass();
    
    // merge in defaults
    $sync_map[$node->type] += array(
      'fields' => array(),
      'content_field' => 'body',
      'cmis_type' => 'cmis:document',
      'cmis_repositoryId' => 'default',
      'subfolders' => FALSE,
      'deletes' => FALSE,
      'full_sync_next_cron' => FALSE
    );
    
    // CMIS destination type
    $cmis_object->type = $sync_map[$node->type]['cmis_type'];
    
    // CMIS destination type
    $cmis_repository = cmis_get_repository($sync_map[$node->type]['cmis_repositoryId']);
    $cmis_object->repositoryId = $cmis_repository->repositoryId;
    
    // CMIS destination folder id
    if (isset($sync_map[$node->type]['cmis_folderId'])) {
      $cmis_object->folderId = $sync_map[$node->type]['cmis_folderId'];
    }
    elseif (isset($sync_map[$node->type]['cmis_folderPath'])) {
      $folder = cmisapi_getObjectByPath($cmis_repository->repositoryId, $sync_map[$node->type]['cmis_folderPath']);
      $cmis_object->folderId = $folder->id;
    }
    else {
      throw new CMISException(t("Please set `cmis_folderPath` or `cmis_folderId` properties for [@type] Drupal type.", array(
        '@type' => $node->type
      )));
    }
    
    // map Drupal node fields to cmis object properties 
    $cmis_object->properties = array();
    foreach ($sync_map[$node->type]['fields'] as $drupal_field => $cmis_field) {
      if (is_string($cmis_field)) {
        $cmis_object->properties[$cmis_field] = _cmis_sync_drupal_node_field_value($node, $drupal_field);
      }
      elseif (is_array($cmis_field)) {
        if (array_key_exists('drupal to cmis', $cmis_field) && $cmis_field['drupal to cmis'] === False) {
          continue;
        }
        $cmis_object->properties[$cmis_field['cmis']] = _cmis_sync_drupal_node_field_value($node, $cmis_field['drupal']);  
      }
      else {
        throw new CMISException(t('Unknown field map type. Expects "string" or "array". Received @type',
                                  array('@type' => gettype($cmis_field))));
      }
    }
    
    // map Drupal node field as object's content
    if (array_key_exists('content_field', $sync_map[$node->type])) {
      $cmis_object->content = _cmis_sync_drupal_node_field_value($node, $sync_map[$node->type]['content_field']);
      
      // setting content's content-type
      $cmis_object->properties['content-type'] = 'text/html';
      $content_type = content_types($node->type);
      $content_field_name = $sync_map[$node->type]['content_field'];
      if (array_key_exists($content_field_name, $content_type['fields'])) {
        if ($content_type['fields'][$content_field_name]['type'] == 'filefield') {
          $content_field = $node->$content_field_name;
          $cmis_object->properties['content-type'] = $content_field[0]['filemime'];
        }
      }
    }
    
    // clean up properties array
    $cmis_object->contentStreamMimeType = $cmis_object->properties['content-type'];
    unset($cmis_object->properties['content-type']);
    
    // make sure that cmis:name property is filled.
    if (!array_key_exists('cmis:name', $cmis_object->properties)) {
      $cmis_object->properties['cmis:name'] = $node->title;
    }
    
    $cmis_object->name = $cmis_object->properties['cmis:name'];
    
    // sync deletes flag
    $cmis_object->sync_deletes = $sync_map[$node->type]['deletes'];
    
    // call hook_sync_drupal_cmis_prepare() hooks
    module_invoke_all('sync_drupal_cmis_prepare', $node, & $cmis_object);
    
    return $cmis_object;
  }

  return FALSE;
}

/**
 * Utility that gets/sets node field value.
 * Only supports regular, text and filefield fields types.
 */
function _cmis_sync_drupal_node_field_value(& $node, $field_name, $field_value = NULL, $context = array()) {
  if (isset($node)) {
    $content_type = content_types($node->type);
    $value = NULL;
  
    if (array_key_exists($field_name, $content_type['fields'])) {
      // CCK field
      $content_field = $node->$field_name;
      switch($content_type['fields'][$field_name]['type']) {
        case 'filefield':
          if ($field_value == NULL) {
            if (realpath($content_field[0]['filepath'])) {
              $value = file_get_contents($content_field[0]['filepath']);
            }
          }
          else {
            if (is_array($content_field) && is_array($content_field[0]) && array_key_exists('filepath', $content_field[0])) {
              // update Drupal file node on filesystem
              file_put_contents($content_field[0]['filepath'], $field_value);
            }
            else {
              // create Drupal file node on filesystem
              $file_context = array();
              if (array_key_exists('file', $context)) {
                $file_context = $context['file'];  
              }
              
              // merge in defaults
              $file_context += array(
                'id' => uniqid('cmis_sync_'),
                'mime' => 'text/html'
              );
              
              // create file
              $file_drupal_path = file_directory_path(). '/' .$file_context['id'];
              file_put_contents($file_drupal_path, $field_value);
              
              // create file field
              $file = new stdClass();
              $file->filename = basename($file_drupal_path);
              $file->filepath = $file_drupal_path;
              $file->filemime = $file_context['mime'];
              $file->filesize = filesize($file_drupal_path);              
              $file->status = FILE_STATUS_PERMANENT;
              $file->timestamp = time();
              drupal_write_record('files', $file);
              
              // create new filefield
              $node->$field_name = array(array(
                'fid' => $file->fid,
                'filename' => $file->filename,
                'filepath' => $file->filepath,
                'filesize' => $file->filesize,
                'filemime' => $file->filemime,
                'list' => 1,
              ));
              
            }
          }
          break;
        case 'text':
        case 'date':
        case 'datestamp':
        case 'datetime':
        case 'number_decimal':
        case 'number_float':
        case 'number_integer':
          if ($field_value == NULL) {
            $value = $content_field[0]['value'];
          }
          else {
            $content_field[0]['value'] = $field_value;
            $node->$field_name = $content_field;
          }
          break;
        default:
          throw new CMISException(t('Unsupported sync drupal content field type [@field_type] for field [@field_name]. Please review your configuration.', array(
            '@field_name' => $field_name,
            '@field_type' => $content_type['fields'][$field_name]['type']
          )));
          break;
      }
    }
    else {
      // regular node field value
      if ($field_value == NULL) {
        $value = $node->$field_name;
      }
      else {
        $node->$field_name = $field_value;
      }
    }
    
    return $value;
  }
  else {
    return NULL;
  }
}
