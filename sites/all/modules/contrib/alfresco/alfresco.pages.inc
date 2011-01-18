<?php
// $Id: alfresco.pages.inc,v 1.2.2.2 2010/04/26 12:33:02 xergius Exp $

/**
 * @file
 * User page callbacks for the alfresco module.
 */

// ------------------------------------------------------------------
// Alfresco page
// ------------------------------------------------------------------

function alfresco_overview_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  return theme('admin_block_content', $content);
}

function alfresco_items_page() {
  //drupal_set_title(check_plain(''));
  $view = views_embed_view('alfresco_items');
  return $view;
}

// ------------------------------------------------------------------
// Alfresco download page
// ------------------------------------------------------------------

/**
 * Menu callback; transfer alfresco file using the configured download method.
 */
function alfresco_page_download($node) {

  $node_link = l(t('view'), 'node/'. $node->nid);
  $watchdog_args = array('@type' => $node->type, '%title' => $node->title);
  watchdog('alfresco', '@type: downloaded %title.', $watchdog_args, WATCHDOG_NOTICE, $node_link);

  // @todo: No incrementar contadores si el nodo no existe en el repositorio

  // We are counting content downloads.
  if (variable_get('alfresco_statistics_count_downloads', 0)) {
    // A node has been downloaded, so update the alfresco node's counters.
    db_query('UPDATE {alfresco_counter} SET daycount = daycount + 1, totalcount = totalcount + 1, timestamp = %d WHERE nid = %d', time(), $node->nid);
    // If we affected 0 rows, this is the first time downloading the node.
    if (!db_affected_rows()) {
      // We must create a new row to store counters for the new node.
      db_query('INSERT INTO {alfresco_counter} (nid, daycount, totalcount, timestamp) VALUES (%d, 1, 1, %d)', $node->nid, time());
    }
  }

  switch (variable_get('alfresco_file_downloads', ALFRESCO_FILE_DOWNLOADS_MODULE)) {
    case ALFRESCO_FILE_DOWNLOADS_MODULE:
      alfresco_file_transfer($node->reference);
      break;

    case ALFRESCO_FILE_DOWNLOADS_GUEST:
      if (isset($node->cm_content)) {
        drupal_goto($node->cm_content['url']);
      }
      break;

    case ALFRESCO_FILE_DOWNLOADS_TICKET:
      alfresco_include('soap');
      $alf_node = alfresco_soap_node_load($node->reference);
      $alf_data = $alf_node->cm_content;
      if ($alf_data) {
        drupal_goto($alf_data->url);
      }
      break;
  }

  drupal_not_found();
  exit();
}

/**
 * @see http://wiki.alfresco.com/wiki/URL_Addressability#DownloadContentServlet
 */
function alfresco_download_content($workspace, $store, $uuid) {
  $reference = "$workspace://$store/$uuid";
  alfresco_file_transfer($reference);
}

// ------------------------------------------------------------------
// Alfresco details page
// ------------------------------------------------------------------

/**
 * Menu callback; Alfresco node details.
 */
function alfresco_page_details($node) {

  alfresco_include('soap');
  $alf_node = alfresco_soap_node_load($node->reference);
  if (!$alf_node) {
    drupal_not_found();
    exit();
  }

  $output = '';

  // Type
  $details = array();
  $details[] = array($alf_node->getType());
  $output .= theme('table', array(t('Type')), $details);

  // Aspects
  $details = array();
  foreach ($alf_node->getAspects() as $key => $value) {
    if (is_string($value) || is_null($value)) {
      $details[] = array($value);
    }
  }
  $output .= theme('table', array(t('Aspects')), $details);

  // Properties
  $details = array();
  foreach ($alf_node->getProperties() as $key => $value) {
    if (is_string($value) || is_null($value)) {
      $details[] = array($key, $value);
    }
  }
  $output .= theme('table', array(t('Properties')), $details);

  // VersionHistory
  $details = array();
  foreach ($alf_node->getVersionHistory() as $key => $value) {
    if (is_string($value) || is_null($value)) {
      $details[] = array($key, $value);
    }
  }
  $output .= theme('table', NULL, $details);

  // Associations
  $details = array();
  foreach ($alf_node->getAssociations() as $key => $value) {
    if (is_string($value) || is_null($value)) {
      $details[] = array($key, $value);
    }
  }
  $output .= theme('table', NULL, $details);

  return $output;
}

// ------------------------------------------------------------------
//
// ------------------------------------------------------------------

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing alfresco nodes.
 */
function alfresco_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    alfresco_include('soap');
    $results = alfresco_soap_search($string);
    if (is_array($results)) {
      foreach ($results as $anode) {
        $matches[$anode->getId()] = check_plain($anode->getFolderPath()) .'/'. check_plain($anode->cm_name);
      }
    }
  }

  drupal_json($matches);
}

// ------------------------------------------------------------------
//
// ------------------------------------------------------------------

/**
 * @todo: Arreglar flujo del formulario.
 *
 * @see https://issues.alfresco.com/jira/browse/ETWOONE-400
 * @see http://wiki.alfresco.com/wiki/PHP_Tutorial_Five
 * @see http://drupal.org/node/470834
 */
function alfresco_upload_form($form_state) {
  global $user;

  drupal_set_title(t('Add content in Alfresco repository'));

  // Si un fichero ha sido subido, vamos al formulario de propiedades.
  if (isset($form_state['storage']['file'])) {
    return alfresco_upload_properties_form($form_state);
  }

  // Formulario de subida de fichero.
  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  $form['upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload content'),
    '#description' => t('This dialog helps you to add content to a space.'),
  );

  $form['upload']['space'] = array(
    '#type' => 'textfield',
    '#title' => t('Space'),
    '#required' => TRUE,
    //'#autocomplete_path' => 'alfresco/autocomplete',
    '#default_value' => isset($user->alfresco['home']) ? $user->alfresco['home'] : '/app:company_home/app:guest_home',
  );

  $exceed_max_msg = t('The maximum upload size is %size (PHP settings limit).', array('%size' => format_size(file_upload_max_size())));
  $form['upload']['file'] = array(
    '#type' => 'file',
    '#title' => t('File'),
    '#description' => t('Locate content to upload.') .' '. $exceed_max_msg,
  );

  $form['buttons']['upload'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  return $form;
}

function alfresco_upload_properties_form($form_state) {

  // Recuperamos la información del fichero
  $file = $form_state['storage']['file'];

  $form['upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Uploaded content'),
  );

  $form['upload']['space'] = array(
    '#type' => 'textfield',
    '#title' => t('Space'),
    '#required' => TRUE,
    //'#autocomplete_path' => 'alfresco/autocomplete',
    //'#description' => t('Space.'),
    '#default_value' => $form_state['values']['space'],
  );

  $form['upload']['file'] = array(
    '#type' => 'item',
    '#title' => t('File'),
    '#value' => $file['filename'] .' ('. format_size($file['filesize']) .')'
  );

  $form['properties'] = array(
    '#type' => 'fieldset',
    '#title' => t('General properties'),
  );

  $form['properties']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $file['filename'],
    '#required' => TRUE,
    '#maxlength' => 255,
  );

  $form['properties']['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#default_value' => 'cm_content',
    '#options' => array(
      'cm_content' => 'Content'
    ),
  );

  /*
  $encodings = array();
  foreach (mb_list_encodings() as $encoding) {
    $encodings[$encoding] = $encoding;
  }

  $form['properties']['encoding'] = array(
    '#type' => 'select',
    '#title' => t('Encoding'),
    '#default_value' => 'UTF-8',
    '#options' => $encodings,
  );
  */

  $form['properties']['encoding'] = array(
    '#type' => 'textfield',
    '#title' => t('Encoding'),
    '#default_value' => 'UTF-8',
    '#maxlength' => 255,
    '#size' => 40,
  );

  $form['properties']['mimetype'] = array(
    '#type' => 'textfield',
    '#title' => t('Content Type'),
    '#default_value' => $file['filemime'],
    '#maxlength' => 255,
    '#size' => 40,
  );

  $type_names = node_get_types('names');
  $form['properties']['node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create the %item node on Drupal.', array('%item' => $type_names['alfresco_item'])),
    '#default_value' => FALSE,
  );

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content properties'),
    '#description' => t('Information about the content to be uploaded.'),
    '#collapsible' => TRUE,
    //'#collapsed' => TRUE,
  );

  $form['content']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => '',
    '#maxlength' => 255,
  );

  $form['content']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => '',
    '#rows' => 2,
  );

  $form['content']['author'] = array(
    '#type' => 'textfield',
    '#title' => t('Author'),
    '#default_value' => '',
    '#maxlength' => 255,
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add content'),
  );

  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('alfresco_upload_form_cancel'),
  );

  return $form;
}

function alfresco_upload_form_validate($form, &$form_state) {

  if ($form_state['values']['op'] == t('Cancel')) {
    return;
  }

  if (empty($form_state['storage']['file'])) {
    // Subimos el fichero a Drupal

    $validators = array(
      //'file_validate_extensions' => array(),
      //'file_validate_image_resolution' => array(),
      //'file_validate_size' => array($file_limit, $user_limit),
    );

    // Save new file uploads.
    if ($file = file_save_upload('file', $validators)) {
      $form_state['storage']['file'] = (array)$file;
    }
    else {
      form_set_error('file', t('File field is required.'));
    }
  }
  else {
    // Subimos el fichero a Alfresco
    alfresco_upload_form_add($form, &$form_state);
  }
}

function alfresco_upload_form_submit($form, &$form_state) {

  if (!empty($form_state['storage']['file'])) {
    /*
    drupal_set_message(t("'!filename' was uploaded successfully.", array(
      '!filename' => $form_state['storage']['file']['filename'])
    ));
    */
    $form_state['rebuild'] = TRUE;
  }
}

/**
 *
 */
function alfresco_upload_form_add($form, &$form_state) {

  $file = $form_state['storage']['file'];

  $filepath = $file['filepath'];
  $space = $form_state['values']['space'];
  $name = $form_state['values']['name'];
  $type = $form_state['values']['type'];
  $mimetype = $form_state['values']['mimetype'];
  $encoding = $form_state['values']['encoding'];
  $create_node = $form_state['values']['node'];

  $properties = array(
    'cm_title' => $form_state['values']['title'],
    'cm_description' => $form_state['values']['description'],
    'cm_author' => $form_state['values']['author'],
  );

  alfresco_include('soap');
  $result = alfresco_soap_node_add($space, $filepath, $name, $mimetype, $encoding, $type, $properties);
  if (!isset($result->error)) {
    $name = $result->node->cm_name;
    $space = $result->space;
    if ($create_node) {
      $reference = $result->node->__toString();
      if ($node = alfresco_create_node($reference, $name)) {
        $name = l($name, 'node/'. $node->nid);
      }
    }
    drupal_set_message(t("'!name' was created under '!space' successfully.", array('!name' => $name, '!space' => $space->cm_name)));

    if (module_exists('alfresco_browser')) {
      // @todo Refactorizar
      global $user;
      cache_clear_all("user={$user->uid};node={$space->id}", 'cache_alfresco_browser');
    }

    unset($form_state['storage']);
  }
  else {
    form_set_error('name', $result->error);
  }
}

function alfresco_upload_form_cancel($form, &$form_state) {
  $form_state['storage'] = NULL;
  $form_state['rebuild'] = FALSE;
  $form_state['redirect'] = 'alfresco/upload';
}
