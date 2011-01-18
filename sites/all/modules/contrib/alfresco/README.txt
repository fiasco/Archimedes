// $Id: README.txt,v 1.2.2.2 2010/04/26 12:33:02 xergius Exp $

-- SUMMARY --

Alfresco module is an out of the box solution to integrate a Drupal site with
Alfresco Open Source Enterprise Content Management System (ECM). It consists of
a set of modules that offer multiple functionalities ready to use with minimal
configuration. The core of integration is based on the type of content
'Alfresco Item', following the paradigm Alfresco items as Drupal nodes.

* alfresco.module: The main module. Provides a new 'Alfresco item' node content
  type for Drupal sites. The 'Alfresco item' node embeds Alfresco contents
  inside your site, with support for direct and private download of the files
  stored in Alfresco repository, caching of node properties, search repository,
  Views support, ... 

* alfresco_browser.module: Allows users to visualize, search, browse and
  retrieve nodes of the Alfresco repository.

* alfresco_attach.module: Allows users to create and attach alfresco items to
  other Drupal content (similar to upload.module).

* alfresco_import.module: Allows importing multiple alfresco nodes from a space
  of Alfresco.


-- REQUIREMENTS --

* Alfresco 3.x or 2.x (Alfresco Community or Alfresco Enterprise)

* PHP 5.2 or later, with:
  * DOM Extension (part of PHP 5 core)
  * SOAP Extension (--enable-soap)

* Ext JS - JavaScript Library
  * Only for Alfresco browser module


-- INSTALLATION --

* Note that you will need an installed and configured Alfresco (on remote or
  local server) prior to installing this module.

* Install Alfresco module as usual on your Drupal site.

* Alfresco browser module requires the Ext JS Library. Please download the Ext
  JS library, extract the archive and copy its contents to the following
  location: sites/all/libraries/ext. If you are using Libraries module, you can
  place Ext JS in the Libraries module's search path.


-- CONFIGURATION --

* Configure module settings in Administer >> Site configuration >> Alfresco:

  - Repository: Set URL and credentials for the Alfresco repository.
  
  - File downloads: Select the download method of the files stored in Alfresco
    repository. 
 
* Configure user permissions in Administer >> User management >> Permissions
  >> alfresco module


-- CREDITS --

Author and maintainer:
* Sergio Martín Morillas <smartin@gmv.com> - http://drupal.org/user/191570

Contributors:
* Manuel Jesús Recena Soto
  Thanks for your helpful comments and encouragement.

* Some of the icons used by this module are part of the Alfresco Community.

This project is sponsored by:
* GMV
  Technological business group with an international presence. GMV offers its
  solutions, services and products in very diverse sectors: Aeronautics,
  Banking and Finances, Space, Defense, Health, Security, Transportation,
  Telecommunications, and Information Technology for Public Administration and
  large corporations. Visit http://www.gmv.com for more information. 
