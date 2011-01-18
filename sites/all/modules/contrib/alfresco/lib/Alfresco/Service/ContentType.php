<?php
/*
 * Copyright 2009 abcona e.K. (www.abcona.de)
 * Licensed to the public under the terms of the GPL v3
 *
 * A ContentType is a read-only peek into Alfresco's content model;
 * these are much simpler objects unrelated to Node objects and only
 * retrieved via the DictionaryService API.
 * Root class is usually 'cm:content', but for any serious deployment a own root class
 * for a particular companys content model should be defined.
 *
 */
require_once 'Alfresco/Service/BaseObject.php';

class AlfContentType extends AlfBaseObject {

  private $_session;
  // Bean properties

  private $_name;

  private $_title;

  private $_superClass;

  public function __construct($session, $name, $title, $superClass) {
    if (!$session) die("No Session");
    $this->_session = $session;
    $this->_name = $name;
    $this->_title = $title;
    $this->_superClass = $superClass;
  }

  public function getChildren() {
    $result = array();
    $nsm = $this->_session->getNamespaceMap();
    $ds = $this->_session->dictionaryService;
    $soapArgs = array(
      'types' => array(
        'names' => $nsm->getShortName($this->_name, ':') ,
        'followSubClass' => true,
        'followSuperClass' => false
    ) ,
      'aspects' => array(
        'followSubClass' => false,
        'followSuperClass' => false
    ) ,
    );
    $soapRC = $ds->getClasses($soapArgs);
    // TODO: Error handling
    // Make sure result is in an array
    $classList = $soapRC->getClassesReturn;
    if (!is_array($classList)) {
      $classList = array(
      $classList
      );
    }
    foreach($classList as $clazz) {
      // UCKYUCK: Obviously, the call to getClasses() with followSubClass=true
      // recursively yields ALL subordinate content types, but we only need it
      // "one level down" and prune all other results.
      // TODO: Store the tree of content types all at once and then serve
      // all subsequent requests from that cache (perhaps in database).
      if (($clazz->name !== $this->_name) && ($clazz->superClass == $this->_name)) {
        $result[] = new AlfContentType($this->_session, $clazz->name, isset($clazz->title) ? $clazz->title : null, isset($clazz->superClass) ? $clazz->superClass : null);
      }
    }
    return $result;
  }

  public function getParent() {
    $nsm = $this->_session->getNamespaceMap();
    $ds = $this->_session->dictionaryService;
    $soapArgs = array(
      'types' => array(
        'names' => $nsm->getShortName($this->_superClass, ':') ,
        'followSubClass' => false,
        'followSuperClass' => false
    ) ,
      'aspects' => array(
        'followSubClass' => false,
        'followSuperClass' => false
    ) ,
    );
    $soapRC = $ds->getClasses($soapArgs);
    // TODO: Error handling
    $clazz = $soapRC->getClassesReturn;
    // var_dump($clazz);
    return new AlfContentType($this->_session, $clazz->name, isset($clazz->title) ? $clazz->title : null, isset($clazz->superClass) ? $clazz->superClass : null);
  }

  public function getName() {
    return $this->_name;
  }

  public function getTitle() {
    return $this->_title;
  }

  public function getSuperClass() {
    return $this->_superClass;
  }
}
