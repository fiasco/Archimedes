<?php
// $Id: alfresco_handler_field_url_download.inc,v 1.2.2.2 2010/04/26 12:33:02 xergius Exp $

/**
 * @file
 *
 */

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class alfresco_handler_field_url_download extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['nid_content'] = array('table' => 'alfresco_content', 'field' => 'nid');
    $this->additional_fields['title'] = array('table' => 'node', 'field' => 'title');
    $this->additional_fields['name'] = 'cm_name';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['use_title'] = array('default' => FALSE);
    $options['display_as_link'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide link to the page being visited.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['use_title'] = array(
      '#title' => t('Use node title as filename'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['use_title']),
    );

    $form['display_as_link'] = array(
      '#title' => t('Display as download link'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['display_as_link']),
    );
  }

  function render($values) {
    if (!empty($this->options['use_title'])) {
      $value = $values->{$this->aliases['title']};
    }
    else {
      $value = $values->{$this->field_alias};
    }

    $content = $values->{$this->aliases['nid_content']};
    if (!empty($this->options['display_as_link']) && !empty($content) && user_access('download alfresco content')) {

      $nid = $values->{$this->aliases['nid']};
      $name = $values->{$this->aliases['cm_name']};
      $url = "node/$nid/download/$name";

      return l(check_plain($value), $url, array('html' => TRUE));
    }
    else {
      return $value;
    }
  }
}