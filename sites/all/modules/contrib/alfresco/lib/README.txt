Alfredo PHP-API
===============

This the PHP-SOAP distributed with Alfresco, repackaged as a SugarCRM module.
It provides no new functionality on its own - it is supposed to be used as a 
foundation for other modules using the PHP-API.

This version has been taken from SVN Rev. 8433


Copyright
=========

* The PHP files under Alfresco/ are Copyright (C) Alfresco, Inc.
  See PHP source files for details.
  
* The module definiton files and accompanying docs are (C)2009 abcona eK
  and licensed to the public under the terms of the GPLv3.

Dependencies
============

The following extra packages are required to run the PHPAPI.
These are for CentOS 5.2, the actual naming on your distribution may vary

* php-soap
* php-xml

Changes from original Alfresco sources
======================================

* Change all PHP files to use 'Alf' as class prefix
  This avoids name clashes with SugarCRMs classes (eg. Store vs. Store)
  Using PHP 5.3's namespace feature would surely give a more elegant approach;
  this idea has been dropped to not limit use on older PHP versions.

* Support for retrieving versioned document content

* Support for storing AlfSession objects in $_SESSION
  Requires Alfredo-Patch R2 or later; used in ALfredo Connector

* Support for loading additional namespace mappings at runtime

* Numerous bugfixes and improvements...


***************************************
* Original "readme.txt" from Alfresco *
***************************************

== Alfresco PHP Libarary ==

Installation and developer documentation for the Alfresco PHP Library can be found on the Alfresco Wiki.

== Documentation Links ==

Installation Instructions - http://wiki.alfresco.com/wiki/Alfresco_PHP_Library_Installation_Instructions
MediaWiki Integration Installation Instructions - http://wiki.alfresco.com/wiki/Alfresco_MediaWiki_Installation_Instructions
PHP API Documentation - http://wiki.alfresco.com/wiki/Alfresco_PHP_API


