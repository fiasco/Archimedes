<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function archimedes_server_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function archimedes_server_imagecache_default_presets() {
  $items = array(
    'siteThumbMini' => array(
      'presetname' => 'siteThumbMini',
      'actions' => array(
        '0' => array(
          'weight' => '-10',
          'module' => 'imagecache_coloractions',
          'action' => 'imagecache_convert',
          'data' => array(
            'format' => 'image/png',
          ),
        ),
        '1' => array(
          'weight' => '-9',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '80',
            'height' => '80',
          ),
        ),
        '2' => array(
          'weight' => '-8',
          'module' => 'imagecache_canvasactions',
          'action' => 'canvasactions_roundedcorners',
          'data' => array(
            'radius' => '5',
            'independent_corners_set' => array(
              'independent_corners' => 0,
              'radii' => array(
                'tl' => '',
                'tr' => '',
                'bl' => '',
                'br' => '',
              ),
            ),
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function archimedes_server_node_info() {
  $items = array(
    'host' => array(
      'name' => t('Host'),
      'module' => 'features',
      'description' => t('A node which represents a host (either web or database) by hostname.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'sla' => array(
      'name' => t('Service Level Agreement'),
      'module' => 'features',
      'description' => t('Definition of Service Level Agreement.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function archimedes_server_views_api() {
  return array(
    'api' => '2',
  );
}
