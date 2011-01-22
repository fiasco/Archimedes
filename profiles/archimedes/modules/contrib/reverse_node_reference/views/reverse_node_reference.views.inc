<?php
// $Id: reverse_node_reference.views.inc,v 1.1.2.1 2010/05/13 14:29:19 markuspetrux Exp $

/**
 * @file
 * Views integration for Nodereference reverse module.
 */

/**
 * Implementation of hook_views_data_alter().
 */
function reverse_node_reference_views_data_alter(&$data) {
  $field_types = _content_field_types();
  $content_types = content_types();
  foreach (content_fields() as $field_name => $field) {
    if ($field['type'] == 'nodereference') {
      $db_info = content_database_info($field);
      $table_alias = content_views_tablename($field);

      $types = array();
      foreach ($content_types as $type) {
        if (isset($type['fields'][$field_name])) {
          $types[] = $type['name'];
        }
      }
      $label_truncated = truncate_utf8($field['widget']['label'], 10, TRUE);

      $data[$table_alias]['vid'] = array(
        'group' => t('Content reverse references'),
        'title' => t('@label (!name) - reverse reference', array('@label' => $field['widget']['label'], '!name' => $field_name)),
        'title short' => t('@label-truncated reverse reference', array('@label-truncated' => $label_truncated)),
        'help' => $field_types[$field['type']]['label'] .' ('. t('reverse reference') .') - '. t('Appears in: @types', array('@types' => implode(', ', $types))),
        'relationship' => array(
          'base' => 'node',
          'base field' => 'vid',
          'left_field' => 'nid',
          'field' => $db_info['columns']['nid']['column'],
          'handler' => 'reverse_node_reference_handler_relationship',
          'label' => t('@label reverse reference', array('@label' => $field['widget']['label'])),
          'content_field_name' => $field_name,
        ),
      );

    }
  }
}

/**
 * Implementation of hook_views_handlers().
 */
function reverse_node_reference_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'reverse_node_reference') . '/views',
    ),
    'handlers' => array(
      'reverse_node_reference_handler_relationship' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}
