<?php 
// $Id: cmis_browser.content_create.inc,v 1.1.2.1.2.11 2010/03/08 20:30:24 cbalan Exp $

/**
 * Create $folder_name under $parent_folder
 * 
 */
function _cmis_browser_actions_folder_create($parent_folder_path, $folder_name) {
  module_load_include('api.inc', 'cmis');
  
  $repository = cmis_get_repository();
  try {
    $parent_folder = cmisapi_getObjectByPath($repository->repositoryId, $parent_folder_path);
  }
  catch (CMISException $e) {
    cmis_error_handler('_cmis_browser_actions_folder_create', $e);
    form_set_error('path', t('Error while trying to lookup @object_id', array('@path' => $parent_folder_path)));
    return '';
  }
  
  if ($parent_folder->properties['cmis:baseTypeId'] == 'cmis:folder') {
    try {
      $folder_id = cmisapi_createFolder($repository->repositoryId, $parent_folder->id, $folder_name);
      drupal_set_message(t('CMIS object @name has been created.', array('@name' => $folder_name)));
    }
    catch (CMISException $e) {
      cmis_error_handler('cmis_folder_create_form_submit', $e);
      form_set_error('path', t('Unable to create @object_name object.', array('@object_name' => $folder_name)));
      return '';
    }
  }
  else {
    form_set_error('path', t('Error while trying to lookup @object_path', array('@object_path' => $parent_folder_path)));
    return FALSE;
  }
}

/**
 * Create $content_name under $parent_folder folder.
 *  
 */
function _cmis_browser_actions_content_create($folder_path, $document_name, $document_content, $document_mimetype='text/html') {
  module_load_include('api.inc', 'cmis');
      
  try {
    $repository = cmis_get_repository();
    $folder = cmisapi_getObjectByPath($repository->repositoryId, $folder_path);
  }
  catch (CMISException $e) {
    cmis_error_handler('_cmis_browser_actions_content_create', $e);
    form_set_error('path', t('Error while trying to lookup @object_path', array('@object_path' => $folder_path)));
    return;
  }
  
  if ($folder->properties['cmis:baseTypeId'] == 'cmis:folder') {
    try {
      $document_id = cmisapi_createDocument($repository->repositoryId, $folder->id, $document_name, array(), $document_content, $document_mimetype);      
      drupal_set_message(t('CMIS object @object_name has been created.', array('@object_name' => $document_name)));
    }
    catch (CMISException $e) {
      cmis_error_handler('_cmis_browser_actions_content_create', $e);
      form_set_error('path', t('Unable to create @object_name object.', array('@object_name' => $document_name)));
      return '';
    }
  }
  else {
    form_set_error('path', t('Error while locating the target space @object_id', array('@object_id' => $folder_path)));
    return FALSE;
  }
}
