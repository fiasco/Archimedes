<?php
/*
 * Copyright (C) 2005 Alfresco, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

 * As a special exception to the terms and conditions of version 2.0 of
 * the GPL, you may redistribute this Program in connection with Free/Libre
 * and Open Source Software ("FLOSS") applications as described in Alfresco's
 * FLOSS exception.  You should have recieved a copy of the text describing
 * the FLOSS exception, and it is also available here:
 * http://www.alfresco.com/legal/licensing"
 */

class AlfNamespaceMap {

  const DELIMITER = "_";

  /**
   * Singleton instance.
   */
  private static $smInstance = NULL;

  private $_namespaceMap = array(
    "d" => "http://www.alfresco.org/model/dictionary/1.0",
    "sys" => "http://www.alfresco.org/model/system/1.0",
    "cm" => "http://www.alfresco.org/model/content/1.0",
    "app" => "http://www.alfresco.org/model/application/1.0",
    "bpm" => "http://www.alfresco.org/model/bpm/1.0",
    "wf" => "http://www.alfresco.org/model/workflow/1.0",
    "fm" => "http://www.alfresco.org/model/forum/1.0",
    "view" => "http://www.alfresco.org/view/repository/1.0",
    "security" => "http://www.alfresco.org/model/security/1.0",
    "wcm" => "http://www.alfresco.org/model/wcmmodel/1.0",
    "wca" => "http://www.alfresco.org/model/wcmappmodel/1.0",
    "ia" => "http://www.alfresco.org/model/calendar",
    "lnk" => "http://www.alfresco.org/model/linksmodel/1.0",
    "imap" => "http://www.alfresco.org/model/imap/1.0",
    "inwf" => "http://www.alfresco.org/model/workflow/invite/nominated/1.0",
    "imwf" => "http://www.alfresco.org/model/workflow/invite/moderated/1.0",
    "wcmwf" => "http://www.alfresco.org/model/wcmworkflow/1.0",
    "act" => "http://www.alfresco.org/model/action/1.0",
    "ver" => "http://www.alfresco.org/model/versionstore/1.0",
    "ver2" => "http://www.alfresco.org/model/versionstore/2.0",
    "st" => "http://www.alfresco.org/model/site/1.0",
    "nt" => "http://www.jcp.org/jcr/nt/1.0",
    "jcr" => "http://www.jcp.org/jcr/1.0",
    "mix" => "http://www.jcp.org/jcr/mix/1.0",
    "sv" => "http://www.jcp.org/jcr/sv/1.0",
    "usr" => "http://www.alfresco.org/model/user/1.0",
    "rule" => "http://www.alfresco.org/model/rule/1.0",
  );

  /**
   * Constructor private to enforce singleton pattern
   */
  private function __construct() {
  }

  /**
   * Boilerplate singleton instantiation...
   */
  public static function getInstance() {
    if (is_null(self::$smInstance)) {
      self::$smInstance = new AlfNamespaceMap();
    }
    return self::$smInstance;
  }

  public function isShortName($shortName) {
    return ($shortName != $this->getFullName($shortName));
  }

  public function getFullName($shortName) {
    $result = $shortName;
    $index = strpos($shortName, AlfNamespaceMap::DELIMITER);
    if ($index !== false) {
      $prefix = substr($shortName, 0, $index);
      if (isset($this->_namespaceMap[$prefix]) == true) {
        $url = $this->_namespaceMap[$prefix];
        $name = substr($shortName, $index + 1);
        $name = str_replace("_", "-", $name);
        if ($name != null && strlen($name) != 0) {
          $result = "{" . $url . "}" . $name;
        }
      }
    }
    return $result;
  }

  public function getFullNames($fullNames) {
    $result = array();
    foreach($fullNames as $fullName) {
      $result[] = $this->getFullName($fullName);
    }
    return $result;
  }

  public function getShortName($fullName, $delimiter = AlfNamespaceMap::DELIMITER) {
    $result = $fullName;
    $index = strpos($fullName, "}");
    if ($index !== false) {
      $url = substr($fullName, 1, $index - 1);
      $prefix = $this->lookupPrefix($url);
      if ($prefix != null) {
        $name = substr($fullName, $index + 1);
        if ($name != null && strlen($name) != 0) {
          $name = str_replace("-", "_", $name);
          $result = $prefix . $delimiter . $name;
        }
      }
    }
    return $result;
  }

  private function lookupPrefix($value) {
    $result = null;
    foreach($this->_namespaceMap as $prefix => $url) {
      if ($url == $value) {
        $result = $prefix;
        break;
      }
    }
    return $result;
  }

  /**
   * aw@abcona.de 2009-10-30: Provide facility to load additional
   * namespace mappings
   */
  public function loadNamespaces($aPathOrFile, $merge = TRUE) {
    $filesToLoad = array();
    if (is_file($aPathOrFile)) {
      $filesToLoad[] = $aPathOrFile;
    } elseif (is_dir($aPathOrFile)) {
      $filesToLoad = glob($aPathOrFile . DIRECTORY_SEPARATOR . "*.php");
    }
    // wipe namespace map if merge not wanted
    if (!$merge) {
      $this->_namespaceMap = array();
    }
    $namespaceMap = $this->_namespaceMap;
    foreach($filesToLoad as $f) {
      // Each required file should work on $namespaceMap
      require ($f);
    }
    $this->_namespaceMap = $namespaceMap;
    // Return number of included files (in case anyone is interested in that
    return sizeof($filesToLoad);
  }

  public function getMapCount() {
    return sizeof($this->_namespaceMap);
  }

  public function getMappings() {
    $result = array();
    foreach($this->_namespaceMap as $key => $value) {
      $result[$key] = $value;
    }
    return $result;
  }
}
