<?php
// $Id: nodereferrer_view_handler_relationship.inc,v 1.1.2.4 2010/03/29 21:08:23 andypost Exp $
/**
 * @file
 * nodereferrer.module Views integration
 */

class nodereferrer_view_handler_relationship extends views_handler_relationship {
  var $content_field;

  function construct() {
    parent::construct();

    if (isset($this->definition['content_field_name'])) {
      $this->content_field = content_fields($this->definition['content_field_name']);
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['referrer_delta'] = array('default' => -1);
    $options['referrer_field'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['referrer_field'] = array(
      '#type' => 'select',
      '#title' => t('Using field'),
      '#options' => nodereferrer_nodereference_field_options(),
      '#default_value' => empty($this->options['referrer_field']) ? array() : $this->options['referrer_field'],
    );

    $options = array('-1' => t('All'));
    for ($i = 0; $i < 10; $i++) {
      $options[$i] = $i + 1;
    }
    $form['referrer_delta'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->options['referrer_delta'],
      '#title' => t('Delta'),
      '#description' => t('The delta allows you to select which item in a multiple value field to key the relationship off of. Select "1" to use the first item, "2" for the second item, and so on. If you select "All", each item in the field will create a new row, which may appear to cause duplicates.'),
    );
  }

  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      $fieldname = $this->options['referrer_field'];
      $field = content_fields($fieldname);

      $db_info = content_database_info($field);

      $join = new views_join();
      $join->construct($db_info['table'], 'node', 'nid', $db_info['columns']['nid']['column']);

      // Add delta condition.
      if (!isset($join->extra)) {
        $join->extra = array();
      }
      $delta = isset($this->options['referrer_delta']) ? $this->options['referrer_delta'] : -1;
      if ($delta != -1) {
        $join->extra[] = array(
          'field' => 'delta',
          'value' => $delta,
          'numeric' => TRUE,
        );
      }

      $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join);
    }

    return $this->table_alias;
  }
}
