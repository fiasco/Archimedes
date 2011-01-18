<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function archimedes_activity_stream_taxonomy_default_vocabularies() {
  return array(
    'activity_tags' => array(
      'name' => 'Activity Tags',
      'description' => '',
      'help' => '',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '1',
      'required' => '0',
      'tags' => '1',
      'module' => 'features_activity_tags',
      'weight' => '0',
      'nodes' => array(
        'action' => 'action',
      ),
    ),
  );
}
