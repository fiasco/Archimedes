<?php

/**
 * Implementation of hook_strongarm().
 */
function archimedes_drupal_site_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_drupal';
  $strongarm->value = 0;

  $export['comment_anonymous_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_drupal_module';
  $strongarm->value = 0;

  $export['comment_anonymous_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_drupal_theme';
  $strongarm->value = 0;

  $export['comment_anonymous_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_drupal';
  $strongarm->value = '3';

  $export['comment_controls_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_drupal_module';
  $strongarm->value = '3';

  $export['comment_controls_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_drupal_theme';
  $strongarm->value = '3';

  $export['comment_controls_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_drupal';
  $strongarm->value = '4';

  $export['comment_default_mode_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_drupal_module';
  $strongarm->value = '4';

  $export['comment_default_mode_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_drupal_theme';
  $strongarm->value = '4';

  $export['comment_default_mode_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_drupal';
  $strongarm->value = '1';

  $export['comment_default_order_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_drupal_module';
  $strongarm->value = '1';

  $export['comment_default_order_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_drupal_theme';
  $strongarm->value = '1';

  $export['comment_default_order_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_drupal';
  $strongarm->value = '50';

  $export['comment_default_per_page_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_drupal_module';
  $strongarm->value = '50';

  $export['comment_default_per_page_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_drupal_theme';
  $strongarm->value = '50';

  $export['comment_default_per_page_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_drupal';
  $strongarm->value = '2';

  $export['comment_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_drupal_module';
  $strongarm->value = '2';

  $export['comment_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_drupal_theme';
  $strongarm->value = '2';

  $export['comment_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_drupal';
  $strongarm->value = '0';

  $export['comment_form_location_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_drupal_module';
  $strongarm->value = '0';

  $export['comment_form_location_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_drupal_theme';
  $strongarm->value = '0';

  $export['comment_form_location_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_drupal';
  $strongarm->value = '1';

  $export['comment_preview_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_drupal_module';
  $strongarm->value = '1';

  $export['comment_preview_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_drupal_theme';
  $strongarm->value = '1';

  $export['comment_preview_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_drupal';
  $strongarm->value = '1';

  $export['comment_subject_field_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_drupal_module';
  $strongarm->value = '1';

  $export['comment_subject_field_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_drupal_theme';
  $strongarm->value = '1';

  $export['comment_subject_field_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_drupal';
  $strongarm->value = array(
    'title' => '2',
    'body_field' => '5',
    'revision_information' => '8',
    'author' => '6',
    'options' => '9',
    'comment_settings' => '10',
    'menu' => '4',
    'path' => '7',
  );

  $export['content_extra_weights_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_drupal_module';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '0',
    'revision_information' => '2',
    'author' => '1',
    'options' => '3',
    'comment_settings' => '4',
    'menu' => '-2',
    'taxonomy' => '-1',
  );

  $export['content_extra_weights_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_drupal_theme';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '0',
    'revision_information' => '20',
    'author' => '20',
    'options' => '25',
    'comment_settings' => '30',
    'menu' => '-2',
    'taxonomy' => '-3',
  );

  $export['content_extra_weights_drupal_theme'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_drupal';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );

  $export['node_options_drupal'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_drupal_module';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );

  $export['node_options_drupal_module'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_drupal_theme';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );

  $export['node_options_drupal_theme'] = $strongarm;
  return $export;
}
