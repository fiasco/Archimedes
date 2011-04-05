<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function archimedes_drupal_site_taxonomy_default_vocabularies() {
  return array(
    'drupal_major_version' => array(
      'name' => 'Drupal Major Version',
      'description' => '',
      'help' => '',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '0',
      'required' => '1',
      'tags' => '0',
      'module' => 'features_drupal_major_version',
      'weight' => '0',
      'type' => NULL,
      'nodes' => array(),
    ),
  );
}
