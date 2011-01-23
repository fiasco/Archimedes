<?php

/**
 * Implementation of hook_strongarm().
 */
function archimedes_server_ui_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_frontpage';
  $strongarm->value = 'site-list';

  $export['site_frontpage'] = $strongarm;
  return $export;
}
