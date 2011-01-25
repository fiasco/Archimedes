<?php

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
    'pathauto', 'diff', 'admin',

    // Features
    'features', 'archimedes_server', 'archimedes_drupal_site', 'archimedes_activity_stream',
    'archimedes_server_ui',
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
   // Update the menu router information.
  menu_rebuild();
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
