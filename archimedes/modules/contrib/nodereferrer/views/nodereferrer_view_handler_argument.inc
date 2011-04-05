<?php
// $Id: nodereferrer_view_handler_argument.inc,v 1.1.2.3 2009/12/28 09:15:55 andypost Exp $
/**
 * @file
 * nodereferrer.module Views integration
 */

/**
 * We use this as a parent class for both the nodereferrer arguments. This handler
 * is not meant to be used directly.
 */
class nodereferrer_view_handler_argument extends views_handler_argument {
  // Don't use the normal operator as this doesn't display nicely
  var $no_operator = TRUE;

  /**
   * Form to get filter parameters
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['operator'] = array(
      '#type' => 'select',
      '#title' => t('Operator'),
      '#default_value' => empty($this->options['operator']) ? 'in' : $this->options['operator'],
      '#options' => array(
        'in' => t('Return nodes that refer to given node id'),
        'not in' => t('Do not return nodes that do not refer to given node id'),
      ),
      '#required' => TRUE,
    );

    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#multiple' => TRUE,
      '#title' => $this->get_options_title(),
      '#options' => $this->get_options(),
      '#description' => t('This is optional ; if nothing is selected then all referrers will be returned'),
      '#default_value' => empty($this->options['fields']) ? array() : $this->options['fields'],
    );

    if (module_exists('translation')) {
      $form['multilingual'] = array(
        '#type' => 'fieldset',
        '#collapsible' => 'true',
        '#collapsed' => 'true',
        '#title' => t('Multilingual options'),
      );
      
      $default = 0;
      if (!empty($this->options['multilingual']['referrers_of_translations'])) {
        $default = $this->options['multilingual']['referrers_of_translations'];
      }
      $form['multilingual']['referrers_of_translations'] = array(
        '#type'  => 'checkbox',
        '#title' => t('Include referrers of translations'),
        '#description' => t('If this is checked, will also include nodes that refer to translations of the given node'),
        '#default_value' => $default,
      );

      $default = 0;
      if (!empty($this->options['multilingual']['translations_of_referrers'])) {
        $default = $this->options['multilingual']['translations_of_referrers'];
      }
      $form['multilingual']['translations_of_referrers'] = array(
        '#type'  => 'checkbox',
        '#title' => t('Include translations of referrers'),
        '#description' => t('If this is checked, will also include translations of nodes that refer to the given node. You may not need this if you synchronise your CCK field.'),
        '#default_value' => $default,
      );
    }
  }

  /**
   * Admin summary
   */
  function admin_summary() {
    return '';
  }

  /**
   * Update the query
   */
  function query() {
    $this->ensure_my_table();

    $list = $this->get_node_list();
    if (empty($list)) {
      if ($this->options['operator'] == 'in') {
        $this->query->add_where($this->options['group'], 'FALSE');
      }
    }
    else {
      $in = '(' . implode(', ', $list) . ')';

      if (!$this->options['operator']) {
        $this->options['operator'] = 'in';
      }

      $this->query->add_where(
        $this->options['group'],
        "$this->table_alias.$this->real_field " . $this->options['operator'] . ' ' . $in
      );
    }
  }

  /**
   * Given a node id, field names and content names, returns the list of nodes
   * that refer to the given node. This takes translation settings into account
   */
  function referrers($nid, $fields, $types = NULL) {
    return array_keys(nodereferrer_referrers_with_translations(
      $nid, $fields, $types,
      !empty($this->options['multilingual']['referrers_of_translations']),
      !empty($this->options['multilingual']['translations_of_referrers'])
    ));
  }

  /**
   * This should be overriden by descendant classes
   */
  function get_node_list() {
    return array();
  }

  function get_options() {
    return array();
  }
}
