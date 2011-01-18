<?php
// $Id: alfresco_import.pages.inc,v 1.2.2.2 2010/04/26 12:33:02 xergius Exp $

/**
 * @file
 *
 */

/**
 *
 */
function alfresco_import_validate_item($item) {

  $errors = array();
  $reference = strval($item);
  if ($nid = alfresco_exists_node($reference)) {
    $errors[] = t('Already exists in your Drupal site. [<a href="!url">View</a>]',
      array('!url' => url('node/'. $nid)));
  }
  return $errors;
}

/**
 *
 */
function alfresco_import_form($form_state) {

  $form = array();

  $reference = isset($form_state['values']['reference']) ? $form_state['values']['reference'] : '';

  $form['space'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Alfresco Space'),
    '#weight' => -30,
  );
  // @todo Recuperar el funcionamiento con alfresco_browser
  $form['space']['reference'] = array(
    '#type'          => /*module_exists('alfresco_browser') ? 'alfresco_browser' : */'textfield',
    '#title'         => t('Space Reference'),
    '#required'      => TRUE,
    '#description'   => t('Alfresco Space Reference. Example: workspace://SpacesStore/uuid'),
    '#default_value' => $reference,
    '#attributes'    => array('style' => 'width: 95%'),
  );
  $form['space']['view'] = array(
    '#type'     => 'submit',
    '#value'    => t('View items'),
    '#submit'   => array('alfresco_import_space_form_submit'),
    '#validate' => array('alfresco_import_space_form_validate'),
  );

  if (!empty($reference)) {
    alfresco_include('soap');
    $alf_node = alfresco_soap_node_load($reference);
    if (!$alf_node) {
      $form['none_found'] = array(
        '#type'  => 'item',
        '#value' => '<em>'. t('Alfresco item does not exist.') .'</em>',
      );
      return $form;
    }

    $folder_types = variable_get('alfresco_folder_types', array('cm_folder'));
    $session = $alf_node->getSession();
    $type = $session->getNamespaceMap()->getShortName($alf_node->type);

    if (!in_array($type, $folder_types)) {
      $form['none_found'] = array(
        '#type'  => 'item',
        '#value' => t('Alfresco item "%name" is not a space.', array('%name' => $alf_node->cm_title)),
      );
      return $form;
    }

    $items = array();
    if (is_array($alf_node->children)) {
      foreach ($alf_node->children as $child) {
        $type = $session->getNamespaceMap()->getShortName($child->child->type);
        if (!in_array($type, $folder_types)) {
          $items[] = $child->child;
        }
      }
    }

    if (count($items) == 0) {
      $form['none_found'] = array(
        '#type'  => 'item',
        '#value' => t('No Alfresco content items were found on "%name"', array('%name' => $alf_node->cm_title)),
      );
      return $form;
    }

    $form['#node_type'] = 'alfresco_item';

    if (user_access('administer nodes')) {
      global $user;

      $dummy_node = new stdClass();
      $dummy_node->type = 'alfresco_item';
      $dummy_node->name = $user->name;

      $form_state = array();
      module_load_include('inc', 'node', 'node.pages');
      $node_form = node_form($form_state, $dummy_node);

      if (isset($node_form['author'])) {
        $form['author'] = $node_form['author'];
      }
      if (isset($node_form['options'])) {
        $form['options'] = $node_form['options'];
      }
    }

    if (module_exists('taxonomy') || module_exists('comment')) {
      $form['type'] = array('#type' => 'value', '#value' => $form['#node_type']);
      $form['#node'] = new stdClass();
      $form['#node']->type = $form['#node_type'];
      if (module_exists('taxonomy')) {
        taxonomy_form_alter($form, array(), $form['#node_type'] .'_node_form');
      }
      if (module_exists('comment')) {
        $form['#node']->comment = variable_get('comment_'. $form['#node_type'], COMMENT_NODE_READ_WRITE);
        comment_form_alter($form, array(), $form['#node_type'] .'_node_form');
      }
      unset($form['type']);
      unset($form['#node']);
    }

    _alfresco_import_add_table_form($form, $items, $alf_node->cm_name);

    $form['submit'] = array(
      '#type'   => 'submit',
      '#value'  => t('Import'),
      '#weight' => 30,
    );
  }

  return $form;
}

function alfresco_import_space_form_validate($form, &$form_state) {

  if (empty($form_state['values']['reference'])) {
    form_set_error('reference', t('You must set an Alfresco Space Reference.'));
  }
  else {
    $reference = $form_state['values']['reference'];
    if (!alfresco_valid_reference($reference)) {
      form_set_error('reference', t('Invalid Alfresco Space Reference.'));
    }
  }
}

function alfresco_import_space_form_submit($form, &$form_state) {

  $form_state['rebuild'] = TRUE;
}

function alfresco_import_form_validate($form, &$form_state) {

  if (!isset($form_state['values']['import']) || count(array_filter($form_state['values']['import'])) == 0) {
    form_set_error('import', t('You must select at least one alfresco item to import.'));
  }
}

function alfresco_import_form_submit($form, &$form_state) {

  $form_state['rebuild'] = FALSE;

  $batch = array(
    'title' => t('Importing alfresco content'),
    'progress_message' => 'Importing @current of @total.',
    'operations' => array(),
    'finished' => '_alfresco_import_batch_finished',
    'file' => drupal_get_path('module', 'alfresco_import') .'/alfresco_import.pages.inc',
  );

  foreach (array_filter($form_state['values']['import']) as $index => $true) {
    $reference = $form_state['values']['file_list'][$index];
    if (alfresco_valid_reference($reference)) {
      $options = array(
        'name'     => isset($form_state['values']['name'])     ? $form_state['values']['name']     : NULL,
        'date'     => isset($form_state['values']['date'])     ? $form_state['values']['date']     : NULL,
        'status'   => isset($form_state['values']['status'])   ? $form_state['values']['status']   : NULL,
        'promote'  => isset($form_state['values']['name'])     ? $form_state['values']['promote']  : NULL,
        'sticky'   => isset($form_state['values']['sticky'])   ? $form_state['values']['sticky']   : NULL,
        'taxonomy' => isset($form_state['values']['taxonomy']) ? $form_state['values']['taxonomy'] : NULL,
        'comment'  => isset($form_state['values']['comment'])  ? $form_state['values']['comment']  : NULL,
      );
      $args = array(
        'node_type' => $form['#node_type'],
        'title'     => isset($form_state['values']['title'][$index]) ? $form_state['values']['title'][$index] : '',
        'body'      => isset($form_state['values']['body'][$index])  ? $form_state['values']['body'][$index]  : '',
        'reference' => $reference,
        'options'   => $options,
      );
      $batch['operations'][] = array('_alfresco_import_batch_op', array($args));
    }
  }

  batch_set($batch);
}

function _alfresco_import_batch_op($args, &$context) {

  // Create the node object.
  if ($node = alfresco_create_node($args['reference'], $args['title'], $args['body'], $args['options'])) {
    $context['results']['good'][] = t('Imported %reference as <a href="!node-link">@node-title</a> @status <a href="!edit-link">[edit]</a>.', array(
      '%reference'  => $args['reference'],
      '!node-link'  => url('node/'. $node->nid),
      '@node-title' => $node->title,
      '@status'     => $node->status ? '' : t('(Unpublished)'),
      '!edit-link'  => url('node/'. $node->nid .'/edit'),
    ));
  }
  else {
    watchdog('alfresco_import', 'There was an error that prevented %reference from being imported.', array('%reference' => $args['reference']), WATCHDOG_ERROR);
    $context['results']['bad'][] = t('Error importing %reference.', array('%reference' => $args['reference']));
  }

  $context['finished'] = TRUE;
}

function _alfresco_import_batch_finished($success, $results, $operations) {

  if (!$success) {
    drupal_set_message(t('There was a problem importing the files.'), 'error');
  }
  if (isset($results['bad']) && count($results['bad'])) {
    drupal_set_message(t('There was a problem importing files: !bad-list', array('!bad-list' => theme('item_list', $results['bad']))), 'error');
  }
  if (isset($results['good']) && count($results['good'])) {
    drupal_set_message(t('Successfully imported: !good-list', array('!good-list' => theme('item_list', $results['good']))));
  }
  watchdog('alfresco_import', 'Completed image import.');
}

function theme_alfresco_import_table($form) {

  $type = node_get_types('type', 'alfresco_item');
  $header = array(theme('table_select_header_cell'), t('Name'), t('Size')/*, check_plain($type->title_label), check_plain($type->body_label)*/);
  $rows = array();
  foreach (element_children($form['import']) as $key) {
    $filename = $form['import'][$key]['#title'];
    unset($form['import'][$key]['#title']);
    $row = array(
      array('data' => drupal_render($form['import'][$key])),
      array('data' => $filename),
      array('data' => drupal_render($form['filesize'][$key])),
    );
    /*
    if (!isset($form['errors'][$key])) {
      $row[] = array('data' => drupal_render($form['title'][$key]));
      $row[] = array('data' => drupal_render($form['body'][$key]));
    }
    else {
      $row[] = array('colspan' => 2, 'data' => drupal_render($form['errors'][$key]));
    }
    */
    $rows[] = $row;
  }
  $caption = !empty($form['#title']) ? $form['#title'] : NULL;
  return theme('table', $header, $rows, array(), $caption);
}

/**
 * Put the alfresco items into an array for the checkboxes and gather
 * additional information.
 * Make sure that there's no 0th element, because a checkbox with a zero value
 * is seen as unchecked and won't be imported.
 */
function _alfresco_import_add_table_form(&$form, $items, $space_name) {

  // Put the alfresco items into an array for the checkboxes and gather
  // additional information.
  // Make sure that there's no 0th element, because a checkbox with a zero
  // value is seen as unchecked and won't be imported.
  $index = 0;
  $itemlist = array();
  $form['files'] = array();

  foreach ($items as $item) {
    $index++;
    $itemlist[$index] = strval($item);

    $problems = alfresco_import_validate_item($item);

    // Spit out the import form elements.
    $form['files']['import'][$index] = array(
      '#type'  => 'checkbox',
      '#title' => $item->cm_name,
    );
    $form['files']['filesize'][$index] = array(
      '#type'  => 'item',
      '#value' => format_size($item->cm_content->size),
    );
    $form['files']['title'][$index] = array(
      '#type'      => 'textfield',
      '#size'      => 20,
      '#maxlength' => 255,
    );
    $form['files']['body'][$index] = array(
      '#type' => 'textfield',
      '#size' => 20,
    );

    // If there were problems don't let them import it
    if (count($problems)) {
      $form['files']['import'][$index]['#type'] = 'item';
      $form['files']['errors'][$index] = array(
        '#type' => 'markup',
        '#value' => '<em>'. implode(' ', $problems) .'</em>',
      );
      unset($form['files']['title'][$index]);
      unset($form['files']['body'][$index]);
    }
  }

  // Put the titles into an array.
  $form['files']['import']['#tree'] = TRUE;
  $form['files']['title']['#tree']  = TRUE;
  $form['files']['body']['#tree']   = TRUE;

  $form['files']['#theme']  = 'alfresco_import_table';
  $form['files']['#title']  = t('The items in this space: %space', array('%space' => $space_name));
  $form['files']['#weight'] = -10;

  // Store a copy of the list into a form value so we can compare it to what
  // they submit and not have to worry about files being added or removed
  // from the filesystem.
  $form['file_list'] = array(
    '#type' => 'value',
    '#value' => $itemlist,
  );
}
