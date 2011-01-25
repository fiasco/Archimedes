<?php

define('ARCHIMEDES_SERVER_THEME', 'corolla');

/**
 * Implementation of hook_profile_modules().
 */
function archimedes_profile_modules() {
  return array(
    // Core
    'color', 'comment', 'help', 'menu', 'taxonomy', 'dblog', 'search', 'path', 'update',

    // Chaos Tools
    'ctools', 'page_manager', 'panels', 'context', 'context_ui',

    // Views
    'views', 'views_ui', 'views_content', 'views_charts', 'views_groupby',

    // CCK
    'content', 'fieldgroup', 'text', 'filefield', 'imagefield',
    'optionwidgets', 'link', 'nodereference', 'nodereferrer',
    'revisionreference', 'number', 'userreference',

    // Date
    'date_api', 'date_timezone', 'date',

    // ImageAPI + ImageCache
    'imageapi', 'imagecache', 'imageapi_gd',  'imagecache_profiles', 'imagecache_ui',
    'imagecache_canvasactions', 'imagecache_coloractions',

    // Strongarm
    'strongarm',

    // Style
    'token', 'pathauto', 'diff', 'admin', 'charts_graphs', 'charts_graphs_google_charts',

    // Features
    'features', 'archimedes_server', 'archimedes_drupal_site', 'archimedes_activity_stream',
    'archimedes_server_ui',

    // Archimedes Client
    'archimedes',
  );
}

/**
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles.
 */
function archimedes_profile_details() {
  return array(
    'name' => 'Archimedes Server',
    'description' => 'Install and configure Drupal to work as an Archimedes Server.'
  );
}


/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function archimedes_profile_task_list() {
}

function archimedes_profile_tasks(&$task, $url) {
  archimedes_config_theme();
   // Update the menu router information.
  menu_rebuild();
}

/**
 * Configure theme
 */
function archimedes_config_theme() {
  // Disable garland
  db_query("UPDATE {system} SET status = 0 WHERE type = 'theme' and name = '%s'", 'garland');

  // Enable Commons theme
  db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = '%s'", ARCHIMEDES_SERVER_THEME);

  // Set Commons theme as the default
  variable_set('theme_default', ARCHIMEDES_SERVER_THEME);
  $settings = variable_get('theme_' . ARCHIMEDES_SERVER_THEME . '_settings', array());
  if (@copy(base_path() . 'profiles/archimedes/archimedes.jpg', file_directory_path() . '/archimedes_logo.jpg')) {
    $settings['logo_path'] = file_directory_path() . '/archimedes_logo.jpg';
  }
  variable_set('theme_' . ARCHIMEDES_SERVER_THEME . '_settings', $settings);

  // Insert blocks into regions
  $block = array(
    'bid' => 7,
    'module' => 'menu',
    'delta' => 'primary-links',
    'theme' => ARCHIMEDES_SERVER_THEME,
    'status' => 1,
    'weight' => 0,
    'region' => 'header_menu',
  );
  drupal_write_record('blocks', $block);

  // Refresh registry
  list_themes(TRUE);

  drupal_rebuild_theme_registry();
}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function archimedes_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'install_configure') {
    // Set default for site name field.
    $form['site_information']['site_name']['#default_value'] = t('Archimedes Monitoring');
  }
}
