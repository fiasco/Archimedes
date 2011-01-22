// $Id: README.txt,v 1.1.2.10 2010/03/17 10:40:06 cbalan Exp $

Content Management Interoperability Services client api
-------------------------------------------------------

 CMIS package contains the following modules:
  * cmis.module - CMIS client api
  * cmis_common.module - CMIS common client library implementation
  * cmis_browser.module - CMIS repository browser
  * cmis_query.module - Provides the ability to run CMIS 1.0 queries against
                        the current CMIS repository.
  * cmis_sync.module - Allows synchronization between
                       Drupal nodes and CMIS objects.
  * cmis_headerswing.module - Demo module that demonstrates using hook_cmis_service()
			      to access the CMIS repository via header-based authentication
                              such as Basic Auth or NTLM.
  * cmis_dev.module - Demo module that displays current CMIS repository's properties. Useful for basic connection testing.


Contents
--------

 * Installation
 * Repository Configuration
 * Drupal-CMIS Syncronization
 * CMIS Hooks
 * CMIS Sync Hooks
 * CMIS Headerswing Settings
 * Credits
 

Installation
------------

 * Place the entire cmis folder into your modules directory.
 * Go to Administer -> Site building -> Modules and enable the cmis modules.
 * Configure at least one CMIS repository (see below)


Repository Configuration
------------------------

 Make sure that `cmis`, `cmis_common`, `cmis_browser` and `cmis_query` modules
 are enabled and add the following lines in your `settings.php` file:
 
$conf['cmis_repositories'] = array(
  'default' => array(
    'user' => '<cmis_user_username>',
    'password' => '<cmis_user_password>',
    'url' => 'http://path/to/cmis/interface'
  )
);
 
 
 Settings:
  * user - Generic username used by cmis_common to authenticate Drupal to the CMIS repository
         - optional, used by cmis_common
  * password - Generic password used by cmis_common to authenticate Drupal to the CMIS repository
             - optional, used by cmis_common
  * url - CMIS repository endpoint url
        - mandatory, used by cmis_common
  * label - repository label
          - optional, used by cmis_browser's CMIS repository switcher block, useful if connecting to multiple repositories
  * browser_default_folderId, browser_default_folderPath
      - default CMIS folder displayed by cmis_browser module
      - optional, defaults to `repositoryInfo['cmis:rootFolderId']`, used by cmis_browser
  * transport - Drupal's module that implements hook_cmis_service($url, $properties, $settings) hook, where :
                      - $url - CMIS absolute REST url
                      - $properties - request properties
                      - $settings - CMIS repositories settings comming from $conf['cmis_repositories']
              - optional, defaults to `cmis_common` used by cmis module
	      - See cmis_headerswing section below for more information


 To browse the CMIS repository go to http://localhost/cmis/browser.
 To query it go to http://localhost/cmis/query.
 
 Query example: 
 To perform the query "select * from cmis:document" go to
 http://localhost/cmis/query/select%2B%252A%2Bfrom%2Bcmis%253Adocument  
 
Drupal-CMIS synchronization
---------------------------

 Make sure that cmis_sync module is enabled and cmis_repositories config var is set.
 Add the following lines to your settings.php file:

$conf['cmis_sync_map'] = array(
  'page' => array(
    'enabled' => TRUE,
    'cmis_folderPath' => '/SomePath'
  ),

 'custom_drupal_content_type' => array(
    'enabled' => TRUE,
    'cmis_folderPath' => '/SomeOtherPath'
		'fields' => array(
			'title' => 'cmis:name',
			'custom_field' => 'cmis:otherCustomProperty',
			array('drupal' => 'some_other_custom_field', 'cmis' => 'cmis:someOtherCustomProperty', 'cmis to drupal' => TRUE, 'drupal to cmis' => TRUE)
		)
  ),
	
 	// ...
);

 This will enable the synchronization process which will sync drupal nodes
 of type page to cmis:document cmis objects under /SomePath folder.
 For each created/updated node, sync process will create/update a cmis object
 under cmis_folderPath by mapping $node->title to cmis:name
 and $node->body to cmis object's content stream.

 
 Settings:
  * enabled - synchronization state for current drupal content type
            - mandatory
  * cmis_repositoryId - repository id or alias
                      - optional, deaults to 'default' CMIS repository
  * cmis_type - CMIS type used by sync process for CMIS objects synchronized
                with Drupal nodes
              - optional, defaults to 'cmis:document'
  * cmis_folderId, cmis_folderPath
      - CMIS folder used as destination for CMIS objects synchronized
        with Drupal nodes
      - mandatory
  * content_field - Drupal node field that will be considered as content.
                  - optional, defaults to 'body'
  * fields - field sync map. Which Drupal field should sync with which CMIS property.
           - optional, defaults to 'array('title' => 'cmis:name')`'
  * deletes - if TRUE, sync process will delete drupal nodes
              if CMIS objects have been deleted and vice versa
            - optional, default: 'FALSE'
  * subfolders - if TRUE, CMIS objects under cmis_folderId will also be synchronized.
               - optional, default: 'FALSE'
  * full_sync_next_cron - if TRUE, at next cron run, sync process
                          will synchronize all CMIS objects under cmis_folderId
                          not only the recent changed items
                        - optional, default: 'FALSE'
  * cmis_sync_cron_enabled - if TRUE, CMIS to Drupal sync will be triggered by cron.
                           - useful if sync process is triggered by another event
                           - optional, default: 'TRUE'
  * cmis_sync_nodeapi_enabled - if TRUE, Drupal to CMIS sync will be triggered by
                                nodeapi's insert, update, and delete operations
                              - useful if sync process is triggered by another event.
                              - optional, default: 'TRUE'

 Fields sync setting syntax:
  * short hand syntax:
  
$conf['cmis_sync_map']['page']['fields'] = array(
  'title' => 'cmis:name',
  'custom_cck_field' => 'some cmis custom property'
)

  * long hand syntax

$conf['cmis_sync_map']['page']['fields'] = array(
  array('drupal' => 'title', 'cmis' => 'cmis:name', 'drupal to cmis' => TRUE, 'cmis to drupal' => TRUE),
  array('drupal' => 'custom_cck_field', 'cmis' => 'some cmis custom property', 'drupal to cmis' => TRUE, 'cmis to drupal' => TRUE),
  
  // copy cmis:objectId to custom_cck_field2 drupal field.
  // 'drupal to cmis' => FALSE, 'cmis to drupal' => TRUE means that only custom_cck_field2 Drupal field is updated.
  // sync process will not try to update cmis:objectId from custom_cck_field2 field's value.
  array('drupal' => 'custom_cck_field2', 'cmis' => 'cmis:objectId', 'drupal to cmis' => FALSE, 'cmis to drupal' => TRUE)
)
  
  
CMIS Hooks
----------
 
 * hook_cmis_service() - allows control over CMIS repository connection.
 * hook_cmis_info() - used to register a module that implements a CMIS client.
 * hook_cmisapi_invoke() - called by cmis api whenever a cmisapi_* is called.
 * hook_cmisapi_*() - where * means any CMIS call(ie. getRepositoryInfo).
                    - these hooks are called only if hook_cmisapi_invoke()
                    is not defined.

 Examples of how these hooks are used, can be found in the following files:
  - cmis.module
  - cmis.api.inc
  - cmis_custom.module (hook_cmisapi_invoke)
  - cmis_headerswing.module (hook_cmis_service)
 
  
CMIS Sync Hooks
---------------
 
 In order to allow other Drupal modules to manipulate the way Drupal nodes
 are mapped to CMIS objects and back, cmis_sync module exposes two hooks:
  * hook_sync_drupal_cmis_prepare($node, $cmis_object)
        - Called after cmis_sync, based on $conf['cmis_sync_map'],
        prepared `$cmis_object` to be sent to CMIS repository.
  * hook_sync_cmis_drupal_prepare($cmis_object, $node)
        - Called after cmis_sync, based on $conf['cmis_sync_map'],
        prepared $node to be sent to Drupal's node_save()
  
  
CMIS Headerswing Settings
-------------------------

 The CMIS Headerswing module provides a mechanism for relaying (or "swinging") HTTP header data
 from Drupal to the CMIS repository. In theory, this can be used to relay any HTTP header.
 In practice, this is particularly useful for passing user authentication information from 
 Drupal to the CMIS repository, providing Single Sign-On (SSO), when authentication is managed by 
 a third party component that populates $_SERVER vars with credentials, such as HTTP Basic or NTLM.

 This module also provides an example of how to create a custom implementation of 
 hook_cmis_service(), overriding the default transport mechanism.

 Configuration sample:

$conf['cmis_repositories'] = array(
  'default' => array(
    'user' => 'admin',
    'password' => 'admin',
    'label' => 'local cmis repo',
    'url' => 'http://127.0.0.1:8080/cmis',
    'transport' => 'cmis_headerswing', 
    'headerswing_headers' => array(
      'HTTP_HOST' => 'FRONTEND_HOST',
      'HTTP_HOST' => 'FRONTEND_HOST_AGAIN',
      'HTTP_USER' => 'FRONTEND_USER',
      'PHP_AUTH_USER' => 'FRONTEND_USER'
      'PHP_AUTH_DIGEST' => 'FRONTEND_AUTH'
    )
  )
);

 Based on these settings cmis_headerswing module will copy $_SERVER[ headerswing_headers's keys ]
 to the CMIS request headers.


Credits
-------

 Contributors
  - Dries Buytaert (dries@acquia.com)
  - Yong Qu (yong.qu@alfresco.com)
  - Matt Asay (masay@alfresco.com)
  - Scott Davis (scott.davis@alfresco.com)
  - Jeff Potts (jpotts@optaros.com)
  - Dave Gynn (dgynn@optaros.com)
  - Chris Fuller (cfuller@optaros.com)
  - Rich McKnight (rich.mcknight@alfresco.com)

 Maintainers
  - Catalin Balan (cbalan@optaros.com)
