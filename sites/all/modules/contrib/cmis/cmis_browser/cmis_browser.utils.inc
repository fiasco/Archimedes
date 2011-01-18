<?php 
// $Id: cmis_browser.utils.inc,v 1.1.2.1.2.11 2010/03/08 20:30:24 cbalan Exp $

/**
 * Helper method used to lookup CMIS object from request.
 * 
 */
function _cmis_browser_content_object_from_request($repository) {
  $object_id = NULL;
  $object_path = NULL;
  $bcarray = array_slice(explode('/', $_GET['q']), 2);
  
  if (array_key_exists('id', $_GET)) {
    // grab objectId from GET
    $object_id = urldecode($_GET['id']);
  } 
  elseif (!empty($bcarray)) {
    // grab path 
    $object_path = drupal_urlencode('/'. implode('/', $bcarray));
  } 
  elseif (array_key_exists('browser_default_folderId', $repository->settings)) {
    // grab default folderId from repository's settings
    $object_id = $repository->settings['browser_default_folderId'];
  }
  elseif (array_key_exists('browser_default_folderPath', $repository->settings)) {
    // grab default folderPath from repository's settings
    $object_path = drupal_urlencode($repository->settings['browser_default_folderPath']);
  }
  else {
    // fallback to repository's root folderId
    $object_id = $repository->info->repositoryInfo['cmis:rootFolderId'];
  }
  
  if (!is_null($object_id)) {
    $object = cmisapi_getProperties($repository->repositoryId, $object_id);
  }
  elseif (!is_null($object_path)) {
    $object = cmisapi_getObjectByPath($repository->repositoryId, $object_path);
  }
  else {
    throw new CMISException('Unknown CMIS object');
  }

  return $object;
}
