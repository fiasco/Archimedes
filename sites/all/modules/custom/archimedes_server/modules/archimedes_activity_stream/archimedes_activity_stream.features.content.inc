<?php

/**
 * Implementation of hook_content_default_fields().
 */
function archimedes_activity_stream_content_default_fields() {
  $fields = array();

  // Exported field: field_site_ref
  $fields['action-field_site_ref'] = array(
    'field_name' => 'field_site_ref',
    'type_name' => 'action',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '1',
    'multiple' => '1',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'drupal' => 'drupal',
      'moodle' => 'moodle',
      'action' => 0,
      'alfresco_item' => 0,
      'drupal_module' => 0,
      'drupal_theme' => 0,
      'host' => 0,
      'moodle_module' => 0,
      'page' => 0,
      'panel' => 0,
      'sla' => 0,
      'story' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_site_ref][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Site Reference',
      'weight' => '-4',
      'description' => '',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Site Reference');

  return $fields;
}
