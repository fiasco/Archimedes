<?php
// $Id: alfresco_browser.admin.inc,v 1.1.2.1 2010/04/26 12:33:03 xergius Exp $

/**
 * @file
 * Administrative page callbacks for the alfresco browser module.
 *
 * @todo Añadir control de acceso para las diferentes acciones del navegador.
 */

// ------------------------------------------------------------------
// Alfresco browser settings page
// ------------------------------------------------------------------

/**
 * Form builder for the alfresco browser settings page.
 *
 * @see system_settings_form()
 */
function alfresco_browser_admin_settings() {

  $form['browser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser'),
  );

  $form['browser']['alfresco_browser_home'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Home path'),
    '#description'   => t('Points to the root folder of Alfresco Repository (defaults to Company Home)'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_home', ALFRESCO_BROWSER_HOME),
  );

  $form['browser']['alfresco_browser_query_limit'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Page size'),
    '#description'   => t('The number of records to display per page (defaults to 20)'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_query_limit', 20),
    '#size'          => 8,
  );

  /*
  $form['browser']['upload'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Add content'),
    '#description'   => t('Upload content to repository from Browser'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );

  $form['browser']['upload']['alfresco_browser_upload_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable upload'),
    '#description'   => t('Enable add content to Alfresco repository from browser.'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_upload_enabled', TRUE),
  );

  $form['browser']['upload']['alfresco_browser_required_properties'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Content properties'),
    '#description'   => t('Enable add content to Alfresco repository from browser.'),
    '#options'       => array(
      'title' => t('Title'),
      'description' => t('Description'),
      'author' => t('Author'),
    ),
    '#default_value' => variable_get('alfresco_browser_required_properties', array()),
  );
  */

  $form['browser_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser cache'),
  );

  $form['browser_cache']['alfresco_browser_cache_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable cache'),
    '#description'   => t('Enable cache for the Alfresco browser to store the results of web services queries against Alfresco repository.'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_cache_enabled', TRUE),
  );

  $form['#submit'][] = 'alfresco_browser_admin_settings_submit';
  return system_settings_form($form);
}

/**
 * Validate form values for alfresco browser settings page.
 */
function alfresco_browser_admin_settings_validate($form, &$form_state) {
  alfresco_include('soap');

  if (!alfresco_valid_path($form_state['values']['alfresco_browser_home'])) {
    form_set_error('alfresco_browser_home', t('The home path you specified is not valid.'));
  }
  elseif (!alfresco_soap_node_load($form_state['values']['alfresco_browser_home'])) {
    form_set_error('alfresco_browser_home', t('The home path you specified could not be found.'));
  }

  if (!is_numeric($form_state['values']['alfresco_browser_query_limit']) || $form_state['values']['alfresco_browser_query_limit'] < 1) {
    form_set_error('alfresco_browser_query_limit', t('Page size value must be a number and greater than zero.'));
  }
}

/**
 * Process admin alfresco browser settings form submissions.
 */
function alfresco_browser_admin_settings_submit($form, &$form_state) {

  if ($form_state['values']['alfresco_browser_cache_enabled'] != variable_get('alfresco_browser_cache_enabled', TRUE)) {
    cache_clear_all(NULL, 'cache_alfresco_browser');
    drupal_set_message(t('Alfresco browser cache cleared.'));
  }
}
