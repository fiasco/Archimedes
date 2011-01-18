<?php 
// $Id: cmis_browser.content_delete.inc,v 1.1.2.1.2.11 2010/03/08 20:30:24 cbalan Exp $

/**
 * Menu callback -- ask for confirmation of CMIS object deletion
 * 
 */
function cmis_browser_content_delete_confirm(&$form_state) {
  module_load_include('api.inc', 'cmis');
  
  $cmis_objectId = urldecode($_GET['id']);
  $form['cmis_objectId'] = array('#type' => 'value', '#value' => $cmis_objectId);
  $form['return_url'] = array('#type' => 'value', '#value' => $_GET['return_url']);

  try {
    $repository = cmis_get_repository();
    $cmis_object = cmisapi_getProperties($repository->repositoryId, $cmis_objectId);  
  }
  catch (CMISException $e) {
    cmis_error_handler('cmis_content_delete', $e);
  }
  
  return confirm_form($form,
    t('Are you sure you want to delete %name?', array('%name' => $cmis_object->properties['cmis:name'])),
    $_GET['return_url'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute CMIS object deletion.
 * 
 */
function cmis_browser_content_delete_confirm_submit($form, &$form_state) {
  module_load_include('api.inc', 'cmis');
  
  if ($form_state['values']['confirm']) {
    $cmis_objectId = $form_state['values']['cmis_objectId'];
    try {
      $repository = cmis_get_repository();
      $cmis_object = cmisapi_getProperties($repository->repositoryId, $cmis_objectId);
      $content = cmisapi_deleteObject($repository->repositoryId, $cmis_object->id);
      drupal_set_message(t('CMIS object @name has been deleted.', array('@name' => $cmis_object->properties['cmis:name'])));
    }
    catch (CMISException $e) {
      cmis_error_handler('cmis_content_delete', $e);
    }
  }

  $form_state['redirect'] = $form_state['values']['return_url'];
}
