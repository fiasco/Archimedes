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
require_once ('Alfresco/Service/WebService/WebServiceFactory.php');
require_once ('Alfresco/Service/BaseObject.php');
require_once ('Alfresco/Service/Node.php');

/**
 *
 * Version history class.
 *
 * @author Roy Wetherall
 */
class AlfVersionHistory extends AlfBaseObject {

  /** Node to which this version history relates */
  private $_node;

  /** Array of versions */
  private $_versions;

  /**
   * Constructor
   *
   * @param	$node	the node that this version history apples to
   */
  public function __construct($node) {
    $this->_node = $node;
    $this->_versions = array();
    $this->populateVersionHistory();
  }

  /**
   * Get the node that this version history relates to
   */
  public function getNode() {
    return $this->_node;
  }

  /**
   * Get a list of the versions in the version history
   */
  public function getVersions() {
    return $this->_versions;
  }

  /**
   * Get a particular version
   */
  public function getVersion($versionLabel) {
    return $this->_versions[$versionLabel];
  }

  /**
   * Populate the version history
   */
  private function populateVersionHistory() {
    // Use the web service API to get the version history for this node
    $client = AlfWebServiceFactory::getAuthoringService($this->_node->session->repository->connectionUrl, $this->_node->session->ticket);
    $result = $client->getVersionHistory(array(
      "node" => $this->_node->__toArray()
    ));
    //var_dump($result);
    if (isset($result->getVersionHistoryReturn->versions)) {
      $versions = (array)$result->getVersionHistoryReturn->versions;
      // TODO populate the version history from the result of the web service call
      foreach($versions as $versionInfo) {
        $commentaries = array();
        foreach($versionInfo->commentaries as $commentary) {
          $key = $commentary->name;
          $isMultiValue = $commentary->isMultiValue;
          if (!$isMultiValue) {
            $value = $commentary->value;
            // print "$key -> $value\n";
            $commentaries[$key] = $value;
          } else {
            print "** MultiValue?? **\n";
          }
        }
        // print "Version: " . $versionInfo->label . " D:" . $commentaries['description'] . " REF:" . $commentaries['frozenNodeRef'] . "\n";
        $version = new AlfVersion($this->_node->session, $this->_node->session->getStore($commentaries['store-identifier'], $commentaries['store-protocol']) , $commentaries['node-uuid']);
        $this->_versions[$versionInfo->label] = $version;
      }
    }
  }
}
