<?php

/**
 * Implementation of hook_content_default_fields().
 */
function archimedes_moodle_site_content_default_fields() {
  $fields = array();

  // Exported field: field_db_size
  $fields['moodle-field_db_size'] = array(
    'field_name' => 'field_db_size',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'number_integer',
    'required' => '1',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '',
    'max' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_db_size][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Database Size',
      'weight' => '76',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_dbhost
  $fields['moodle-field_dbhost'] = array(
    'field_name' => 'field_dbhost',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '1',
    'multiple' => '0',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'host' => 'host',
      'drupal_module' => 0,
      'drupal' => 0,
      'drupal_theme' => 0,
      'moodle' => 0,
      'page' => 0,
      'story' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'nid' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Database Host',
      'weight' => '73',
      'description' => '',
      'type' => 'nodereference_select',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_dbname
  $fields['moodle-field_dbname'] = array(
    'field_name' => 'field_dbname',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_dbname][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Database Name',
      'weight' => '74',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_deb
  $fields['moodle-field_deb'] = array(
    'field_name' => 'field_deb',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_deb][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Debian Package',
      'weight' => '83',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_last_update
  $fields['moodle-field_last_update'] = array(
    'field_name' => 'field_last_update',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'datetime',
    'required' => '1',
    'multiple' => '0',
    'module' => 'date',
    'active' => '1',
    'granularity' => array(
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
      'hour' => 'hour',
      'minute' => 'minute',
      'second' => 'second',
    ),
    'timezone_db' => 'UTC',
    'tz_handling' => 'date',
    'todate' => '',
    'repeat' => 0,
    'repeat_collapsed' => '',
    'default_format' => 'long',
    'widget' => array(
      'default_value' => 'blank',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'input_format' => 'd/m/Y - H:i:s',
      'input_format_custom' => '',
      'increment' => 1,
      'text_parts' => array(),
      'year_range' => '-3:+3',
      'label_position' => 'above',
      'label' => 'Last Update',
      'weight' => '80',
      'description' => '',
      'type' => 'date_text',
      'module' => 'date',
    ),
  );

  // Exported field: field_mdl_mod
  $fields['moodle-field_mdl_mod'] = array(
    'field_name' => 'field_mdl_mod',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'revisionreference',
    'required' => '0',
    'multiple' => '1',
    'module' => 'revisionreference',
    'active' => '1',
    'referenceable_revision_types' => array(
      'moodle_module' => 'moodle_module',
      'drupal_module' => 0,
      'drupal' => 0,
      'drupal_theme' => 0,
      'host' => 0,
      'moodle' => 0,
      'page' => 0,
      'panel' => 0,
      'story' => 0,
    ),
    'advanced_revision_view' => '--',
    'advanced_revision_view_args' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'vid' => '0',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Modules',
      'weight' => '84',
      'description' => '',
      'type' => 'revisionreference_select',
      'module' => 'revisionreference',
    ),
  );

  // Exported field: field_mdl_num_courses
  $fields['moodle-field_mdl_num_courses'] = array(
    'field_name' => 'field_mdl_num_courses',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'number_integer',
    'required' => '0',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '',
    'max' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'label' => 'Course Count',
      'weight' => '81',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_num_users
  $fields['moodle-field_num_users'] = array(
    'field_name' => 'field_num_users',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'number_integer',
    'required' => '1',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '',
    'max' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_num_users][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'User Count',
      'weight' => '79',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_servername
  $fields['moodle-field_servername'] = array(
    'field_name' => 'field_servername',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'link',
    'required' => '1',
    'multiple' => '1',
    'module' => 'link',
    'active' => '1',
    'attributes' => array(
      'target' => 'default',
      'rel' => '',
      'class' => '',
      'title' => 'Servername',
    ),
    'display' => array(
      'url_cutoff' => '80',
    ),
    'url' => 0,
    'title' => 'none',
    'title_value' => '',
    'enable_tokens' => '',
    'validate_url' => 0,
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'url' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Servername',
      'weight' => '69',
      'description' => '',
      'type' => 'link',
      'module' => 'link',
    ),
  );

  // Exported field: field_sitedata
  $fields['moodle-field_sitedata'] = array(
    'field_name' => 'field_sitedata',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'number_integer',
    'required' => '1',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '',
    'max' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_sitedata][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Site Data Size',
      'weight' => '77',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_sitesize
  $fields['moodle-field_sitesize'] = array(
    'field_name' => 'field_sitesize',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'number_integer',
    'required' => '1',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '',
    'max' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_sitesize][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Webroot Size',
      'weight' => '78',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_thumbnail
  $fields['moodle-field_thumbnail'] = array(
    'field_name' => 'field_thumbnail',
    'type_name' => 'moodle',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => 'thumbs',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '256x192',
      'min_resolution' => '0',
      'alt' => 'Site Thumbnail',
      'custom_alt' => 0,
      'title' => '',
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'label' => 'Thumbnail',
      'weight' => '61',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_unique_hash
  $fields['moodle-field_unique_hash'] = array(
    'field_name' => 'field_unique_hash',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_unique_hash][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Unique Hash',
      'weight' => '70',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_users
  $fields['moodle-field_users'] = array(
    'field_name' => 'field_users',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'userreference',
    'required' => '0',
    'multiple' => '1',
    'module' => 'userreference',
    'active' => '1',
    'referenceable_roles' => array(
      '2' => 2,
    ),
    'referenceable_status' => '',
    'advanced_view' => '',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => 60,
      'reverse_link' => 1,
      'default_value' => array(
        '0' => array(
          'uid' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Users',
      'weight' => '75',
      'description' => '',
      'type' => 'userreference_select',
      'module' => 'userreference',
    ),
  );

  // Exported field: field_webroot
  $fields['moodle-field_webroot'] = array(
    'field_name' => 'field_webroot',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_webroot][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Web Root',
      'weight' => '72',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_webserver
  $fields['moodle-field_webserver'] = array(
    'field_name' => 'field_webserver',
    'type_name' => 'moodle',
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
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '1',
    'multiple' => '1',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'host' => 'host',
      'drupal_module' => 0,
      'drupal' => 0,
      'drupal_theme' => 0,
      'moodle' => 0,
      'page' => 0,
      'story' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'nid' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Web Server',
      'weight' => '71',
      'description' => '',
      'type' => 'nodereference_select',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_mod_version
  $fields['moodle_module-field_mod_version'] = array(
    'field_name' => 'field_mod_version',
    'type_name' => 'moodle_module',
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
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '16',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_mod_version][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Version',
      'weight' => '32',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Course Count');
  t('Database Host');
  t('Database Name');
  t('Database Size');
  t('Debian Package');
  t('Last Update');
  t('Modules');
  t('Servername');
  t('Site Data Size');
  t('Thumbnail');
  t('Unique Hash');
  t('User Count');
  t('Users');
  t('Version');
  t('Web Root');
  t('Web Server');
  t('Webroot Size');

  return $fields;
}
