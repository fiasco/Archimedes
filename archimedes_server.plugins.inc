<?php

/**
 * Implementation of Archimedes Server Plugin node.
 */
class ArchimedesServerPlugin_node extends ArchimedesServerPlugin {

  public function parse($value) {
    $return = array('nid' => NULL);
    if (!$node = $this->load($value)) {
      $node = $this->create($value);
    }
    $return['nid'] = $node->nid;
    return $return;
  }

  /**
   * Overrideable method to load a node.
   */
  protected function load($value) {
    $name = trim($value->nodeValue);
    $type = $value->getAttributeNS($this->ns,'type');
    return node_load(array('title' => $name, 'type' => $type));
  }

  /**
   * Overrideable method to create a node.
   */
  protected function create($value) {
    $type = content_types($value->getAttributeNS($this->ns, 'type'));
    if (empty($type)) {
      throw new Exception("Cannot create node from " . $this->ns . ': Missing node type');
    }
    $node = new StdClass;
    $node->created = time();
    $node->status = 1; //published
    $node->promote = 0;
    $node->sticky = 0;
    $node->type = $value->getAttributeNS($this->ns, 'type');
    $node->title = $value->nodeValue;
    $node->uid = 1;
    $node->body = $value->getAttributeNS($this->ns,'body');
    foreach($type['fields'] as $field_name => $field) {
      $node->{$field_name} = $field['widget']['default_value'];
      if (!$attr = $value->getAttributeNS($this->ns, $field_name)) {
        continue;
      }
      // We can't identify which key the attribute value should accurately
      // be but we can assume its not a private key (starts with an underscore)
      // so we'll populate all of them.
      foreach ($node->{$field_name}[0] as $key => $val) {
        if (strpos($key, '_') !== 0) {
          $node->{$field_name}[0][$key] = $attr;
        }
      }
    }
    node_save($node);
    return $node;
  }
}

/**
 * Implementation of Archimedes Server Plugin node.
 */
class ArchimedesServerPlugin_user extends ArchimedesServerPlugin {

  public function parse($value) {
    $return = array('uid' => NULL);
    if($value->getAttributeNS('monitor-plugin:user','type') == 'mail') {
      $mail = trim($value->nodeValue);
      $return['uid'] = archimedes_server_user_save($mail);
    }
    return $return;
  }
}
