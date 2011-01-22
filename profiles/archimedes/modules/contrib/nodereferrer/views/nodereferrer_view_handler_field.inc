<?php
// $Id: nodereferrer_view_handler_field.inc,v 1.1.2.4 2010/02/11 23:13:30 andypost Exp $
/**
 * @file
 * nodereferrer.module Views integration
 */

/**
 * We use this as a parent class for both the nodereferrer fields. This handler
 * is not meant to be used directly.
 */
class nodereferrer_view_handler_field extends views_handler_field {
  /**
   * Form to get field parameters
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = nodereferrer_field_formatter_info();
    foreach ($options as $k => $v) {
      $options[$k] = $v['label'];
    }
    $form['formatter'] = array(
      '#type' => 'select',
      '#multiple' => FALSE,
      '#title' => t('Formatter'),
      '#options' => $options,
      '#description' => t('Select the formatter to use to display the fields'),
      '#default_value' => empty($this->options['formatter']) ? '' : $this->options['formatter'],
    );

    $form['list'] = array(
      '#type' => 'select',
      '#multiple' => FALSE,
      '#title' => t('Display as'),
      '#options' => array( 'list' => 'List items', 'comma' => 'Comma separated list'),
      '#description' => t('Select how multiple values should be displayed'),
      '#default_value' => empty($this->options['list']) ? 'list' : $this->options['list'],
    );

    $form['limit'] = array(
      '#type' => 'select',
      '#multiple' => FALSE,
      '#title' => t('Limit'),
      '#options' => array(0 => t('Unlimited')) + drupal_map_assoc(range(1, 10)),
      '#description' => t('Select how many referencing nodes will be displayed'),
      '#default_value' => empty($this->options['limit']) ? 0 : $this->options['limit']
    );

    $form['randomize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Randomize'),
      '#description' => t('Check this box if you want the referrers to be displayed in random order'),
      '#default_value' => empty($this->options['randomize']) ? FALSE : $this->options['randomize']
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
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
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
   * All we need to do is ensure our table is there
   */
  function query() {
    $this->ensure_my_table();
  }

  /**
   * Given a node id, field names and content names, returns the list of nodes
   * that refer to the given node. This takes translation settings into account
   */
  function referrers($nid, $fields, $types = NULL) {
    return nodereferrer_referrers_with_translations(
      $nid, $fields, $types,
      !empty($this->options['multilingual']['referrers_of_translations']),
      !empty($this->options['multilingual']['translations_of_referrers'])
    );
  }

  /**
   * Render the field.
   *
   */
  function render($values) {
    $list = $this->get_item_list($values->nid);
    if ($this->options['formatter'] == 'count') {
      return (string)count($list);
    }
    if ($this->options['randomize']) {
      shuffle($list);
    }
    if ($this->options['limit']) {
      $list = array_slice($list, 0, $this->options['limit']);
    }
    foreach ($list as $k => $v) {
      $list[$k] = theme('nodereferrer_field_' . $this->options['formatter'], $v);
    }
    if ($this->options['list'] == 'list') {
      if (count($list)) {
        return '<ul><li>' . implode('</li><li>', array_filter($list)) . '</li></ul>';
      }
      else {
        return '';
      }
    }
    else {
      return implode(', ', $list);
    }
  }

  /**
   * This should be overriden by descendant classes
   */
  function get_node_list($nid) {
    return array();
  }

  function get_options() {
    return array();
  }
}
