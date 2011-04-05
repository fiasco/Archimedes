<?php

/**
 * Implementation of Archimedes Server Plugin drupal-module.
 */
class ArchimedesServerPlugin_drupal_module extends ArchimedesServerPlugin_node {

  /**
   * The name of the cck field the module version is held in.
   */
  private $version_field = 'field_mod_version';

  public function parse($value) {
    $this->cache_update($value);
    $return = array('vid' => NULL);
    if (!$node = $this->load($value)) {
      $node = $this->create($value);
    }
    $return['vid'] = $node->vid;
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
      $tid = '';
      switch (substr($version,0,1)) {
        case '5':
          $tid = taxonomy_get_term_by_name('5.x');
          break;
        case '6':
          $tid = taxonomy_get_term_by_name('6.x');
          break;
        case '7':
          $tid = taxonomy_get_term_by_name('7.x');
          break;
      }
      taxonomy_node_save($node,$tid);
      return $node;
    }
    return FALSE;
  }

  protected function create($value) {
    if ($node = parent::create($value)) {
      $version = $value->getAttributeNS($this->ns, $this->version_field);
      $tid = '';
      switch (substr($version,0,1)) {
        case '5':
          $tid = taxonomy_get_term_by_name('5.x');
          break;
        case '6':
          $tid = taxonomy_get_term_by_name('6.x');
          break;
        case '7':
          $tid = taxonomy_get_term_by_name('7.x');
          break;
      }
      taxonomy_node_save($node,$tid);
      return $node;
    }
    return FALSE;
  }

  protected function cache_update($value) {
    $project = $value->getAttributeNS($this->ns, 'field_dru_proj');
    if (empty($project)) {
      return;
    }
    $version = $value->getAttributeNS($this->ns, $this->version_field);
    $core = substr($version,0,1) . '.x';
    $key = $project . $core;
    if (!cache_get($key,'cache_archimedes_drupal')) {
      module_load_include('inc', 'update', 'update.fetch');

      $data = array();
      $url = $value->getAttributeNS($this->ns, 'field_mod_url');
      $url = $url ? $url : UPDATE_DEFAULT_URL;

      $url .= '/' . $project . '/' . $core;

      archimedes_drupal_site_archimedes_cache_set($url,$key,$project);
    }
  }
}
