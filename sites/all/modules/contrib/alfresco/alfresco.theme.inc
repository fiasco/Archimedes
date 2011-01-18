<?php
// $Id: alfresco.theme.inc,v 1.2.2.2 2010/04/26 12:33:02 xergius Exp $

/**
 * @file
 * Theme functions for Alfresco module.
 */

/**
 * Format the teaser for an alfresco_item node.
 */
function theme_alfresco_teaser($node) {
  $output = '';

  if (isset($node->cm_content)) {
    //$icon = alfresco_get_icon_filetype($node->cm_name);
    //$mimetype = $node->cm_content['mimetype'];
    //$attributes = array('style' => 'float: left; margin: 2px 5px 0 0');
    //$output .= '<div id="alfresco-teaser-icon" style="float: left; margin: 2px 5px 0 0">'. theme('image', $icon, $node->cm_name, $mimetype, NULL) .'</div>';
    $output .= '<div id="alfresco-teaser-size">'. format_size($node->cm_content['size']) .'</div>';
  }
  $output .= '<div id="alfresco-teaser-date">'. format_date(strtotime($node->cm_modified), 'large') .'</div>';

  return '<div id="alfresco-teaser">'. $output .'</div>';
}

/**
 * Format the alfresco_item node for display.
 */
function theme_alfresco_display($node) {

  $model = array(
    'cm_name'        => t('Name'),
    'cm_title'       => t('Title'),
    'cm_description' => t('Description'),
    'cm_content'     => t('File'),
    'cm_author'      => t('Author'),
    'cm_creator'     => t('Creator'),
    'cm_created'     => t('Created date'),
    'cm_modifier'    => t('Modifier'),
    'cm_modified'    => t('Modified date'),
  );

  $properties = array();
  foreach ($model as $property => $name) {
    if (isset($node->$property)) {
      $value = empty($node->$property) ? t('n/a') : $node->$property;

      if ($property == 'cm_name' && isset($node->cm_content) && user_access('download alfresco content')) {
        //$icon = alfresco_get_icon_filetype($node->cm_name);
        //$attributes = array('style' => 'float: left; margin: 0 5px 0 0;');
        //$icon = theme('image', $icon, $node->cm_name, $node->cm_name, $attributes);

        $url = alfresco_url_download($node);
        $properties[] = array($name, l($value, $url, array('attributes' => array('title' => t('Download, or open in browser window, the content of this item.')))));
      }
      elseif ($property == 'cm_content') {
        $content = $node->cm_content;
        $properties[] = array(t('Mimetype'), $content['mimetype']);
        $properties[] = array(t('Size'), format_size($content['size']));
        $properties[] = array(t('Encoding'), $content['encoding']);
      }
      elseif ($property == 'cm_created' || $property == 'cm_modified') {
        $properties[] = array($name, format_date($value, 'large'));
      }
      else {
        $properties[] = array($name, $value);
      }
    }
  }

  return theme('table', array(), $properties, NULL);
}

/**
 *
 * @see http://cvs.drupal.org/viewvc.py/drupal/contributions/docs/developer/examples/scaffolding_example/scaffolding_example.admin.inc?revision=1.1
 * @see http://api.drupal.org/api/function/drupal_add_tabledrag/6
 */
function theme_alfresco_properties_table($form) {

  $rows = array();
  foreach (element_children($form) as $key) {
    $row = array();

    $row[] = drupal_render($form[$key]['key']);
    $row[] = drupal_render($form[$key]['name']);
    $row[] = drupal_render($form[$key]['display']);
    $row[] = drupal_render($form[$key]['title']);
    $row[] = drupal_render($form[$key]['body']);
    $row[] = drupal_render($form[$key]['show']);

    $form[$key]['weight']['#attributes']['class'] = 'alfresco-properties-order-weight';
    $row[] = drupal_render($form[$key]['weight']);

    $rows[] = array(
      'data'  => $row,
      'class' => 'draggable',
    );
  }

  $header = array(t('Property'), t('Name'), t('Display'), t('Title'), t('Body'),  t('Show'), '');
  $output = theme('table', $header, $rows, array('id' => 'alfresco-properties-table'));
  $output .= drupal_render($form);

  drupal_add_tabledrag('alfresco-properties-table', 'order', 'self', 'alfresco-properties-order-weight');

  return $output;
}
