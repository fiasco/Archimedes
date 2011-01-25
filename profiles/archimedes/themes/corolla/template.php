<?php
// $Id: template.php,v 1.20.2.5 2011/01/01 13:24:55 jarek Exp $

/**
 * Initialize theme settings.
 */
if (is_null(theme_get_setting('base_font_size'))) {
  global $theme_key;
  // Save default theme settings
  $defaults = array(
    'color_scheme' => 'default',
    'base_font_size' => '12px',
    'sidebar_first_weight' => '1',
    'sidebar_second_weight' => '2',
    'layout_1_min_width' => '550px',
    'layout_1_max_width' => '800px',
    'layout_2_min_width' => '750px',
    'layout_2_max_width' => '960px',
    'layout_3_min_width' => '800px',
    'layout_3_max_width' => '1000px',
  );

  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge(theme_get_settings($theme_key), $defaults)
  );
  // Force refresh of Drupal internals
  theme_get_setting('', TRUE);
}

function corolla_preprocess_page(&$vars, $hook) {
  // Remove useless "no-sidebars" class from $body_classes variable
  if (preg_match("/no-sidebars/i", $vars['body_classes']) == true) {
    $vars['body_classes'] = str_replace('no-sidebars', '', $vars['body_classes']);
  }

  // Add $layout_classes variable
  if ( !empty($vars['sidebar_first']) && !empty($vars['sidebar_second']) ) {
    $vars['classes_array'][] = 'two-sidebars';
  }
  elseif ( !empty($vars['sidebar_first']) || !empty($vars['sidebar_second']) ) {
    $vars['classes_array'][] = 'one-sidebar';
  }
  else {
    $vars['classes_array'][] = 'no-sidebars';
  }
  $vars['layout_classes'] = implode(' ', $vars['classes_array']);

  /* Add dynamic stylesheet */
  ob_start();
  include('dynamic.css.php');
  $vars['dynamic_styles'] = ob_get_contents();
  ob_end_clean();
  
   // Add variables with weight value for each main column
  $vars['weight']['content'] = 0;
  $vars['weight']['sidebar-first'] = 'disabled';
  $vars['weight']['sidebar-second'] = 'disabled';
  if ($vars["sidebar_first"]) {
    $vars['weight']['sidebar-first'] = theme_get_setting('sidebar_first_weight');
  }
  if ($vars["sidebar_second"]) {
    $vars['weight']['sidebar-second'] = theme_get_setting('sidebar_second_weight');
  }

  // Add $main_columns_number variable (used in page-*.tpl.php files)
  $columns = 0;
  foreach (array('content', 'sidebar_first', 'sidebar_second') as $n) {
    if ($vars["$n"]) {
      $columns++;
    }
  }
  $vars['main_columns_number'] = $columns;  

}

/**
 * Overrides theme_more_link().
 */
function corolla_more_link($url, $title) {
  return '<div class="more-link">'. t('<a href="@link" title="@title">more ›</a>', array('@link' => check_url($url), '@title' => $title)) .'</div>';
}

/**
 * Overrides theme_tablesort_indicator().
 */
function corolla_tablesort_indicator($style) {
  if ($style == "asc") {
    return theme('image', path_to_theme() . '/images/tablesort-ascending.png', t('sort icon'), t('sort ascending'));
  }
  else {
    return theme('image', path_to_theme() . '/images/tablesort-descending.png', t('sort icon'), t('sort descending'));
  }
}


