<?php

/**
 * Implementation of Archimedes Server Plugin moodle-module.
 */
class ArchimedesServerPlugin_moodle_module extends ArchimedesServerPlugin_node {

  private $version_field = 'field_mod_version'; // The name of the cck field the module version is held in.

  public function parse($value) {
    $return = array('vid' => NULL);
    if (!$node = $this->load($value)) {
      $node = $this->create($value);
    }
    $return['vid'] = $node->vid;
    $return['instances'] = $value->getAttributeNS($this->ns, 'instances'); // currently only works for modified node revision module
    return $return;
  }

  protected function load($value) {
    if ($node = parent::load($value)) {
      $version = $value->getAttributeNS($this->ns, $this->version_field);
      if ($node->{$this->version_field}[0]['value'] == $version) {
        return $node;
      }
      if ($vid = db_result(db_query("SELECT vid FROM {content_{$this->version_field}} WHERE nid = %d AND {$this->version_field}_value = '%s'", $node->nid, $version))) {    
        return node_load(array('nid' => $node->nid), $vid);
      }
      $node->{$this->version_field}[0]['value'] = $version;
      $node->revision = TRUE;
      node_save($node);
      return $node;
    }
    return FALSE;
  }
}
