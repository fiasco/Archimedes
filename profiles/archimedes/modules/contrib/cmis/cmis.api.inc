<?php
// $Id: cmis.api.inc,v 1.2.2.1.2.12 2010/03/18 09:03:09 cbalan Exp $

/**
 * @file
 * CMIS 0.5 client implementation
 * 
 * @todo 
 *   Document all CMIS calls
 *   Update methods signatures to match CMIS specs
 */

/*
 * Repository services
 */

function cmisapi_getRepositories($endpoint_service) {
  return cmis_vendor_invoke('getRepositories', $endpoint_service);
}

function cmisapi_getRepositoryInfo() {
  return cmis_vendor_invoke('getRepositoryInfo');
}

function cmisapi_getTypes($repositoryId, $typeId = NULL) {
  return cmis_vendor_invoke('getTypes', $repositoryId, $typeId);
}

function cmisapi_getTypeDefinition($repositoryId, $typeId, $options = array()) {
  return cmis_vendor_invoke('getTypeDefinition', $repositoryId, $typeId, $options);
}

function cmisapi_getObjectTypeDefinition($repositoryId, $objectId) {
  return cmis_vendor_invoke('getObjectTypeDefinition', $repositoryId, $objectId);
}

/*
 * Navigation servicies
 */
function cmisapi_getFolderTree($repositoryId) {
  return cmis_vendor_invoke('getFolderTree', $repositoryId);
}

function cmisapi_getDescendants($repositoryId, $folderId) {
  return cmis_vendor_invoke('getDescendants', $repositoryId, $folderId);
}

function cmisapi_getChildren($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('getChildren', $repositoryId, $objectId, $options);
}

function cmisapi_getFolderParent($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('getFolderParent', $repositoryId, $objectId, $options);
}

function cmisapi_getObjectParents($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('getObjectParents', $repositoryId, $objectId, $options);
}

function cmisapi_getCheckedOutDocs($repositoryId, $options = array()) {
  return cmis_vendor_invoke('getCheckedOutDocs', $repositoryId, $options);
}

/*
 * Object services
 */

function cmisapi_getObject($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('getObjectByPath', $repositoryId, $objectId, $options);
}

function cmisapi_getObjectByPath($repositoryId, $path, $options = array()) {
  return cmis_vendor_invoke('getObjectByPath', $repositoryId, $path, $options);
}

function cmisapi_createDocument($repositoryId, $folderId, $fileName, $properties=array(), $content=null, $contentType = "application/octet-stream", $options = array()) { 
  return cmis_vendor_invoke('createDocument', $repositoryId, $folderId, $fileName, $properties, $content, $contentType, $options);
}

function cmisapi_createFolder($repositoryId, $folderId, $folderName, $properties=array(),$options=array()) {
  return cmis_vendor_invoke('createFolder', $repositoryId, $folderId, $folderName, $properties, $options);
}

function cmisapi_createRelationship($repositoryId, $typeId, $properties, $sourceObjectId, $targetObjectId) {
  return cmis_vendor_invoke('createRelationship', $repositoryId, $typeId, $properties, $sourceObjectId, $targetObjectId);
}

function cmisapi_createPolicy($repositoryId, $typeId, $properties, $folderId) {
  return cmis_vendor_invoke('createPolicy', $repositoryId, $typeId, $properties, $folderId);
}

function cmisapi_getAllowableActions($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('getAllowableActions', $repositoryId, $objectId, $options);
}

function cmisapi_getRenditions($repositoryId, $objectId, $options = array('renditionFilter' => "*")) {
  return cmis_vendor_invoke('getProperties', $repositoryId, $objectId, $options);
}

function cmisapi_getProperties($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('getProperties', $repositoryId, $objectId, $options);
}

function cmisapi_getContentStream($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('getContentStream', $repositoryId, $objectId, $options);
}

function cmisapi_updateProperties($repositoryId, $objectId, $properties = array(), $options = array()) {
  return cmis_vendor_invoke('updateProperties', $repositoryId, $objectId, $properties, $options);
}

function cmisapi_moveObject($repositoryId, $objectId, $targetFolderId, $sourceFolderId = NULL, $options = array()) {
  return cmis_vendor_invoke('moveObject', $repositoryId, $objectId, $targetFolderId, $sourceFolderId, $options);
}

function cmisapi_deleteObject($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('deleteObject', $repositoryId, $objectId, $options);
}

function cmisapi_deteleTree($repositoryId, $folderId, $unfileNonfolderObjects) {
  return cmis_vendor_invoke('deleteTree', $repositoryId, $folderId, $unfileNonfolderObjects);
}

function cmisapi_setContentStream($repositoryId, $objectId, $content = NULL, $contentType = NULL, $options = array()) {
  return cmis_vendor_invoke('setContentStream', $repositoryId, $objectId, $content, $contentType, $options);
}

function cmisapi_deleteContentStream($repositoryId, $objectId, $options = array()) {
  return cmis_vendor_invoke('deleteContentStream', $repositoryId, $objectId, $options);
}

/*
 * Multi-filling services
 */

function cmisapi_addObjectToFolder($repositoryId, $objectId, $folderId) {
  return cmis_vendor_invoke('addObjectToFolder', $repositoryId, $objectId, $folderId);
}

function cmisapi_removeObjectFromFolder($repositoryId, $objectId, $folderId = NULL) {
  return cmis_vendor_invoke('removeObjectFromFolder', $repositoryId, $objectId, $folderId);
}

/*
 * Discovery servicies 
 */

function cmisapi_query($repositoryId, $statement, $options = array()) {
  return cmis_vendor_invoke('query', $repositoryId, $statement, $options);
}

/*
 * Versioning services
 */

function cmisapi_checkOut($repositoryId, $documentId) {
  return cmis_vendor_invoke('checkOut', $repositoryId, $objectId);
}

function cmisapi_cancelCheckOut($repositoryId, $documentId) {
  return cmis_vendor_invoke('cancelCheckOut', $repositoryId, $objectId);
}

function cmisapi_checkIn($repositoryId, $documentId, $major = NULL, $bag = NULL, $content = NULL, $checkinComment = NULL) {
  return cmis_vendor_invoke('checkIn', $repositoryId, $documentId, $major, $bag, $content, $checkinComment);
}

function cmisapi_getPropertiesOfLatestVersion($repositoryId, $versionSeriesId) {
  return cmis_vendor_invoke('getPropertiesOfLatestVersion', $repositoryId, $versionSeriesId);
}

function cmisapi_getAllVersions($repositoryId, $versionSeriesId) {
  return cmis_vendor_invoke('getAllVersions', $repositoryId, $versionSeriesId);
}

function cmisapi_deleteAllVersions($repositoryId, $versionSeriesId) {
  return cmis_vendor_invoke('deleteAllVersions', $repositoryId, $versionSeriesId);
}

/*
 * Relationships services
 */

function cmisapi_getRelationships($repositoryId, $objectId) {
  return cmis_vendor_invoke('getRelationships', $repositoryId, $objectId);
}

/*
 * Policy services
 */

function cmisapi_applyPolicy($repositoryId, $policyId, $objectId) {
  return cmis_vendor_invoke('applyPolicy', $repositoryId, $policyId, $objectId);
}

function cmisapi_removePolicy($repositoryId, $policyId, $objectId) {
  return cmis_vendor_invoke('removePolicy', $repositoryId, $policyId, $objectId);
}

function cmisapi_getAppliedPolicies($repositoryId, $objectId) {
  return cmis_vendor_invoke('getAppliedPolicies', $repositoryId, $objectId);
}
