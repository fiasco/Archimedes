<?php 
// $Id: cmis_browser.content_properties.inc,v 1.1.2.1.2.11 2010/03/08 20:30:24 cbalan Exp $

/**
 * CMIS object properties page.
 * 
 */
function cmis_browser_content_properties() {
  module_load_include('api.inc', 'cmis');
  module_load_include('utils.inc', 'cmis_browser');
  
  // Invoke CMIS service
  try {
    $repository = cmis_get_repository();
    $object = _cmis_browser_content_object_from_request($repository);
    
    $output = theme('cmis_browser_content_properties', $object);
    
    if (isset($_GET['no_layout'])) {
      print $output;
      exit();
    }
    
    return $output;
  }
  catch (CMISException $e) {
    cmis_error_handler('cmis_browser', $e);
    return '';
  }
}
