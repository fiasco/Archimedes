<?php
// $Id: nodereferrer_view_handler_filter_field.inc,v 1.1.2.1 2009/09/22 11:15:35 andypost Exp $
/**
 * @file
 * nodereferrer.module Views integration
 */

/**
 * Views handler to filter nodes by nodereferer fields
 */
class nodereferrer_view_handler_filter_field extends nodereferrer_view_handler_filter {

  function get_options_title() {
    return t('Using field');
  }

  function get_options() {
    return nodereferrer_nodereference_field_options();
  }

  function get_node_list() {
    $nid = (int)$this->value;
    if (empty($this->options['fields'])) {
      $fields = array_keys(nodereferrer_nodereference_field_options());
    }
    else {
      $fields = array_keys(array_filter($this->options['fields']));
    }

    return $this->referrers($nid, $fields);
  }
}
