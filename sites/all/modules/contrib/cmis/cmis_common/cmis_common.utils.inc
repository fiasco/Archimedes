<?php 
// $Id: cmis_common.utils.inc,v 1.1.2.7 2010/03/08 20:30:25 cbalan Exp $

module_load_include('php', 'cmis_common', 'lib/cmis_repository_wrapper');

class CommonCMISService extends CMISService {
  function doRequest($url, $method='GET', $content = NULL, $contentType = NULL, $charset = NULL) {
    $result = cmis_service($this->workspace->repositoryInfo->repositoryId, $url, array(
      'method' => $method,
      'data' => $content,
      'headers' => array('Content-Type' => $contentType .(!is_null($charset)?'; charset='. $charset:'')),
    ));
    
    $retval = new stdClass();
    $retval->url = $url;
    $retval->method = $method;
    $retval->content_sent = $content;
    $retval->content_type_sent = $contentType;
    $retval->body = $result->body;
    $retval->code = $result->code;
    $retval->content_type = $result->content_type;
    $retval->content_length = $result->content_length;
    
    $this->last_request = $retval;
    
    return $retval;
  }
}
