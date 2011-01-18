<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function archimedes_activity_stream_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function archimedes_activity_stream_node_info() {
  $items = array(
    'action' => array(
      'name' => t('Action'),
      'module' => 'features',
      'description' => t('Site Action to be displayed in the Activity Stream'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function archimedes_activity_stream_views_api() {
  return array(
    'api' => '2',
  );
}
