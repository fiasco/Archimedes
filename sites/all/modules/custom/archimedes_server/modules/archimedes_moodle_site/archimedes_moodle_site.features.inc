<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function archimedes_moodle_site_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function archimedes_moodle_site_node_info() {
  $items = array(
    'moodle' => array(
      'name' => t('Moodle Site'),
      'module' => 'features',
      'description' => t('A node representing a moodle site with the Archimedes Client module installed.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'moodle_module' => array(
      'name' => t('Moodle Module'),
      'module' => 'features',
      'description' => '',
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
