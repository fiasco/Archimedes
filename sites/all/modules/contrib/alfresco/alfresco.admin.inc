<?php
// $Id: alfresco.admin.inc,v 1.2.2.2 2010/04/26 12:33:02 xergius Exp $

/**
 * @file
 * Administrative page callbacks for the alfresco module.
 */

// ------------------------------------------------------------------
// Alfresco repository settings page
// ------------------------------------------------------------------

/**
 * Form builder for the alfresco settings page.
 *
 * @see alfresco_admin_settings_submit()
 * @see alfresco_admin_settings_validate()
 */
function alfresco_admin_settings() {

  drupal_add_js(drupal_get_path('module', 'alfresco') .'/alfresco.js');

  /*
  module_load_install('alfresco');
  $requirements = module_invoke('alfresco', 'requirements', 'runtime');
  $severity = drupal_requirements_severity($requirements);
  if ($severity == REQUIREMENT_ERROR) {
    drupal_set_message(t('One or more problems were detected with your Alfresco setttings. Check the <a href="@status">status report</a> for more information.', array(
      '@status' => url('admin/reports/status')
    )), 'error');
  }
  */

  // REPOSITORY INFO

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global settings'),
    '#description' => t('Alfresco global settings below will be used as site-wide defaults.'),
  );

  $form['general']['alfresco_repository_url'] = array(
    '#default_value' => variable_get('alfresco_repository_url', 'http://localhost:8080/alfresco/api'),
    '#description'   => t('The web service end point URL for the Alfresco repository. Example: http://localhost:8080/alfresco/api'),
    '#title'         => t('URL repository'),
    '#type'          => 'textfield',
    '#required'      => TRUE,
  );
  $form['general']['alfresco_credentials_username'] = array(
    '#default_value' => variable_get('alfresco_credentials_username', 'guest'),
    '#title'         => t('Username'),
    '#description'   => '',
    '#type'          => 'textfield',
  );
  $form['general']['alfresco_credentials_password'] = array(
    '#type'          => 'password',
    '#title'         => t('Password'),
    '#description'   => t('To change the current password, enter the new password in the field. Default password is <em>guest</em>'),
    '#size'          => 25,
  );

  // AUTHENTICATION METHOD

  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication'),
    //'#description' => t('NOTE: These settings have no effect on user with uid 1. The admin account always uses global account.'),
  );

  $form['authentication']['alfresco_authentication_method'] = array(
    '#type' => 'radios',
    '#title' => t('Authentication mode'),
    '#default_value' => variable_get('alfresco_authentication_method', ALFRESCO_AUTH_GLOBAL),
    '#required' => TRUE,
    '#options' => array(
      ALFRESCO_AUTH_GLOBAL => t('Global: <em>Always use the above username and password</em>'),
      ALFRESCO_AUTH_SSO => t('Single sign-on: <em>Each user to use their own Drupal account</em>'),
    ),
    '#description' => t('Choose method used to authenticate against Alfresco repository. On Single sign-on, users without Alfresco account (anonymous users included) always use the above global account.'),
  );

  // STATISTICS

  // count content downloads settings
  $form['statistics'] = array(
    '#type' => 'fieldset',
    '#title' => t('Statistics'),
  );

  $form['statistics']['alfresco_statistics_count_downloads'] = array(
    '#type' => 'radios',
    '#title' => t('Count downloads'),
    '#default_value' => variable_get('alfresco_statistics_count_downloads', 0),
    '#options' => array(
      '1' => t('Enabled'),
      '0' => t('Disabled')
    ),
    '#description' => t('Increment a counter each time Alfresco content is downloaded.'));

  $form['#validate'][] = 'alfresco_admin_settings_validate';
  $form['#submit'][] = 'alfresco_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate the alfresco admin settings form.
 */
function alfresco_admin_settings_validate($form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'alfresco') .'/alfresco.js');

  // Ensure URL is valid.
  $repository_url = $form_state['values']['alfresco_repository_url'];
  if (!empty($repository_url) && !valid_url($repository_url, TRUE)) {
    form_set_error('url', t('The URL repository is invalid. Please enter a fully-qualified URL, such as http://example.com/alfresco/api'));
  }
}

/**
 *
 */
function alfresco_admin_settings_submit($form, &$form_state) {

  if (empty($form_state['values']['alfresco_credentials_password'])) {
    $password = base64_decode(variable_get('alfresco_credentials_password', base64_encode('guest')));
  }
  else {
    $password = $form_state['values']['alfresco_credentials_password'];
  }

  $form_state['values']['alfresco_credentials_password'] = base64_encode($password);

  //$link = array('!link' => l(t('the status report'), 'admin/reports/status'));
  //drupal_set_message(t("Check !link to see whether the connection was successful.", $link));

  // @see _alfresco_check_connection_repository()
  variable_set('alfresco_http_request_fails', FALSE);

  // Limpiamos el ticket actual para forzar la autenticación en la siguiente petición
  alfresco_include('soap');
  alfresco_soap_clear_session();

  drupal_set_message(t('Check the <a href="@status">status report</a> to see whether the connection was successful.', array('@status' => url('admin/reports/status'))));
}

// ------------------------------------------------------------------
// Alfresco display settings page
// ------------------------------------------------------------------

/**
 * Form builder for the alfresco display settings page.
 *
 * @see system_settings_form()
 */
function alfresco_admin_display() {
  $form = array();

  $form['properties'] = array(
    '#tree'        => TRUE,
    '#type'        => 'fieldset',
    '#title'       => t('Properties'),
    '#description' => t('Sorry, this settings not working (under development).'),
    '#collapsible' => FALSE,
    '#theme'       => 'alfresco_properties_table'
  );

  $mapping = array(
    'name'        => t('Name'),
    'title'       => t('Title'),
    'description' => t('Description'),
    /*
    'content'     => array(
      'mimetype'  => t('Mimetype'),
      'encoding'  => t('Encoding'),
      'size'      => t('Size'),
    ),
    */
    'author'      => t('Author'),
    'creator'     => t('Creator'),
    'created'     => t('Created date'),
    'modifier'    => t('Modifier'),
    'modified'    => t('Modified date'),
  );

  $count = count($mapping);
  for ($delta = 0; $delta < $count; $delta++) {
    $key = key($mapping);

    $property['key'] = array(
      '#type'  => 'markup',
      '#value' => 'cm:'. $key,
    );

    $property['name'] = array(
      '#type'          => 'textfield',
      '#maxlength'     => '255',
      '#default_value' => current($mapping),
      '#disabled'      => TRUE,
      //'#attributes'    => array('style' => 'width: 95%')
    );

    $property['display'] = array(
      '#type'          => 'checkbox',
      '#default_value' => TRUE,
    );

    $property['title'] = array(
      '#type'          => 'radio',
      '#name'          => 'title',
      '#default_value' => NULL,
      '#return_value'  => $key,
    );

    $property['body'] = array(
      '#type'          => 'radio',
      '#name'          => 'body',
      '#default_value' => NULL,
      '#return_value'  => $key,
    );

    $property['show'] = array(
      '#type'          => 'select',
      '#default_value' => 2,
      '#options'       => array(0 => 'Only teaser', 1 => 'Only page', 2 => 'Both'),
      //'#attributes'    => array('style' => 'width: 95%')
    );

    $property['weight'] = array(
      '#type' => 'weight',
      '#default_value' => 0,
    );

    $form['properties'][$delta] = $property;

    next($mapping);
  }

  return system_settings_form($form);
}

// ------------------------------------------------------------------
// Alfresco Namespaces settings page
// ------------------------------------------------------------------

/**
 * @todo: documentation
 *
 * @see system_settings_form()
 */
function alfresco_admin_model() {
  $form = array();

  $model_more = t('For more info, visit <a href="@url">Data Dictionary Guide</a> on Alfresco Wiki.', array(
    '@url' => 'http://wiki.alfresco.com/wiki/Data_Dictionary_Guide')
  );

  $form['model'] = array(
    '#tree'        => TRUE,
    '#type'        => 'fieldset',
    '#title'       => t('Content Model'),
    '#description' => t('Alfresco Repository Data Dictionary is pre-populated with definitions that describe common Content constructs such as Folders, Files and meta-data schemes.') .' '. $model_more,
    '#collapsible' => FALSE,
  );

  $headers = array('Namespace', 'Prefix');
  $rows = array();

  alfresco_include_lib('NamespaceMap');
  $namespace = AlfNamespaceMap::getInstance();
  foreach ($namespace->getMappings() as $prefix => $name) {
    $rows[] = array($name, $prefix);
  }

  $form['model']['namespaces'] = array(
    '#type' => 'fieldset',
    '#title' => t('Namespaces'),
    '#description' => t('Each Content model is described in its own XML file and is identified by its defined Namespace and Name. Out-of-the-box, the Alfresco Repository is primed with several models:'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#value' => theme('table', $headers, $rows),
  );

  /*
  $form['model']['custom'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Namespaces'),
    '#description' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  */

  $form['model']['namespaces']['custom'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom namespaces'),
  );

  $headers = array('Title', 'Name', 'Parent');
  $rows = array();
  $rows[] = array('Object', 'cm:cmobject', 'sys:base');
  $rows[] = array('Folder', 'cm:folder', 'cm:cmobject');
  $rows[] = array('Content', 'cm:content', 'cm:cmobject');

  $form['model']['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Types'),
    //'#description' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#value' => theme('table', $headers, $rows),
  );

  return system_settings_form($form);
}

// ------------------------------------------------------------------
// Alfresco advanced settings page
// ------------------------------------------------------------------

/**
 * Form builder for the alfresco advanced settings page.
 */
function alfresco_admin_advanced() {

  $periods = drupal_map_assoc(array(300, 600, 900, 1200, 1500, 1800, 3600, 7200, 10800, 21600, 32400), 'format_interval');

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
  );

  $form['advanced']['alfresco_ticket_timeout'] = array(
    '#type' => 'select',
    '#title' => t('Ticket valid duration'),
    '#default_value' => variable_get('alfresco_ticket_timeout', 1800),
    '#options' => $periods,
    '#description' => t('The period for which tickets are valid (the default is 30 mins).'),
  );

  $tip = t('The namespace for imported types in dictionary-service.wsdl is wrong. For more info, visit <a href="@url">ALFCOM-1913 issue</a>.', array(
    '@url' => 'https://issues.alfresco.com/jira/browse/ALFCOM-1913')
  );

  $form['advanced']['alfresco_workaround_dictionary_service_wsdl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use workaround for incorrect targetNamespace WSDL.'),
    '#description' => $tip,
    '#default_value' => variable_get('alfresco_workaround_dictionary_service_wsdl', TRUE),
  );

  // DOWNLOAD METHOD

  $download_help = t('Select the download method of the files stored in Alfresco repository:');
  $download_tips = array(
    t('<strong>Module method</strong>, use this method if you Alfresco repository is not publicly accessible (default).'),
    t('<strong>Direct guest method</strong>, this method use <em>URL Guest Access</em>, the <em>guest</em> Alfresco user must have access to the item referenced.'),
    t('<strong>Direct ticket method</strong>, this method use <em>Ticket URL Argument</em> to provide authentication without login.'),
  );
  $download_more = t('For more info, visit <a href="@url">URL Addressability page</a> on Alfresco Wiki.', array('@url' => 'http://wiki.alfresco.com/wiki/URL_Addressability#Security'));

  $form['downloads'] = array(
    '#type' => 'fieldset',
    '#title' => t('Download method'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => $download_help . theme('item_list', $download_tips),
  );

  $form['downloads']['alfresco_file_downloads'] = array(
    '#type' => 'radios',
    '#title' => t('Choose download method'),
    '#default_value' => variable_get('alfresco_file_downloads', ALFRESCO_FILE_DOWNLOADS_MODULE),
    '#options' => array(
      ALFRESCO_FILE_DOWNLOADS_MODULE => t('Module - files are transferred by Drupal\'s alfresco module using <em>Ticket URL Argument</em> transparently (recommended).'),
      ALFRESCO_FILE_DOWNLOADS_GUEST  => t('Direct guest - files are available using Alfresco <em>URL Guest Access</em>.'),
      ALFRESCO_FILE_DOWNLOADS_TICKET => t('Direct ticket - files are available using Alfresco <em>Ticket URL Argument</em>.'),
    ),
    '#description' => $download_more,
  );

  return system_settings_form($form);
}
