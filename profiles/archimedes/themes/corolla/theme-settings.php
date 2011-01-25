<?php
// $Id: theme-settings.php,v 1.6.2.4 2011/01/01 13:24:55 jarek Exp $

function corolla_settings($saved_settings) {

  $settings = theme_get_settings('corolla');

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

  $settings = array_merge($defaults, $settings);

  // Generate the form using Forms API. http://api.drupal.org/api/7
  $form['color_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Color scheme'),
    '#default_value' => $settings['color_scheme'],
    '#options' => array(
      'default' => t('Default'),
      'green1' => t('Green 1'),
      'green2' => t('Green 2'),
      'purple' => t('Purple'),
      'red' => t('Red'),
      'yellow' => t('Yellow'),
    ),
  );
  $form['base_font_size'] = array(
    '#title' => 'Base font size',
    '#type' => 'select', 
    '#default_value' => $settings['base_font_size'],
    '#options' => array(
      '9px' => '9px',
      '10px' => '10px',
      '11px' => '11px',
      '12px' => '12px',
      '13px' => '13px',
      '14px' => '14px',
      '15px' => '15px',
      '16px' => '16px',
      '100%' => '100%',
    ),
  );
  $form['sidebar_first_weight'] = array(
    '#title' => 'First sidebar position', 
    '#type' => 'select',
    '#default_value' => $settings['sidebar_first_weight'],
    '#options' => array(
      -2 => 'Far left',
      -1 => 'Left',
       1 => 'Right',
       2 => 'Far right',
    ),
  );
  $form['sidebar_second_weight'] = array(
    '#title' => 'Second sidebar position', 
    '#type' => 'select',
    '#default_value' => $settings['sidebar_second_weight'],
    '#options' => array(
      -2 => 'Far left',
      -1 => 'Left',
       1 => 'Right',
       2 => 'Far right',
    ),
  );
  $form['layout_1'] = array(
    '#title' => '1-column layout', 
    '#type' => 'fieldset',
  );
  $form['layout_1']['layout_1_min_width'] = array(
    '#type' => 'select',
    '#title' => 'Min width', 
    '#default_value' => $settings['layout_1_min_width'],
    '#options' => corolla_generate_array(200, 1400, 10, 'px'),
  );
  $form['layout_1']['layout_1_max_width'] = array(
    '#type' => 'select',
    '#title' => 'Max width', 
    '#default_value' => $settings['layout_1_max_width'],
    '#options' => corolla_generate_array(200, 1400, 10, 'px'),
  );
  $form['layout_2'] = array(
    '#title' => '2-column layout', 
    '#type' => 'fieldset',
  );
  $form['layout_2']['layout_2_min_width'] = array(
    '#type' => 'select',
    '#title' => 'Min width', 
    '#default_value' => $settings['layout_2_min_width'],
    '#options' => corolla_generate_array(200, 1400, 10, 'px'),
  );
  $form['layout_2']['layout_2_max_width'] = array(
    '#type' => 'select',
    '#title' => 'Max width', 
    '#default_value' => $settings['layout_2_max_width'],
    '#options' => corolla_generate_array(200, 1400, 10, 'px'),
  );
  $form['layout_3'] = array(
    '#title' => '3-column layout', 
    '#type' => 'fieldset',
  );
  $form['layout_3']['layout_3_min_width'] = array(
    '#type' => 'select',
    '#title' => 'Min width', 
    '#default_value' => $settings['layout_3_min_width'],
    '#options' => corolla_generate_array(200, 1400, 10, 'px'),
  );
  $form['layout_3']['layout_3_max_width'] = array(
    '#type' => 'select',
    '#title' => 'Max width', 
    '#default_value' => $settings['layout_3_max_width'],
    '#options' => corolla_generate_array(200, 1400, 10, 'px'),
  );

  return $form;
}

function corolla_generate_array($min, $max, $increment, $postfix, $unlimited = NULL) {
  $array = array();
  if ($unlimited == 'first') {
    $array['none'] = 'Unlimited';
  }
  for ($a = $min; $a <= $max; $a += $increment) {
    $array[$a . $postfix] = $a . ' ' . $postfix;
  }
  if ($unlimited == 'last') {
    $array['none'] = 'Unlimited';
  }
  return $array;
}

