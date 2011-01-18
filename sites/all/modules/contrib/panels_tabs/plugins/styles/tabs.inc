<?php
// $Id: tabs.inc,v 1.1.2.7 2010/09/10 12:25:34 smoothify Exp $

/**
 * @file
 * Definition of the 'tabs' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Tabs'),
  'description' => t('Presents the panes in tabs.'),
  'render region' => 'panels_tabs_style_render_region',
  // Deprecated legacy callback
  'render panel' => 'panels_tabs_style_render_panel',
  'settings form'     => 'panels_tabs_style_settings_form',
  'settings validate' => 'panels_tabs_style_settings_validate',
);


//----------------------------------------------------------------------------
// Panels style plugin callbacks.

/**
 * Render region callback.
 */
function theme_panels_tabs_style_render_region($display, $owner_id, $panes, $settings) {
  $output = '';

  // Generate a unique id based on the CSS ID and the name of the panel in the
  // layout.
  $id = '';
  //If CSS ID is set.
  if($display->css_id) {
    $id = "$display->css_id-";
  }
  $pane_id = reset(array_keys($panes));
  $id .= $display->content[$pane_id]->panel;

  // Add the Javascript to the page, and save the settings for this panel.
  _panels_tabs_add_js($id, $settings['filling_tabs']);

  $tabs = array();
  $tabs[$id] = array(
    '#type' => 'tabset',
    '#tabset_name' => $id,
  );
  $index = 0;
  foreach ($panes as $pane_id => $pane_content) {
    if (!empty($pane_content)) {
      $tabs[$id][$pane_id] = array(
        '#type'    => 'tabpage',
        '#title'   => $display->content[$pane_id]->tab_title,
        '#content' => $pane_content,
        '#weight'  => $index,
      );
      $index++;
    }
  }

  // No content has been rendered
  if (empty($index)) {
    return;
  }

  // See if an optional title was added.
  if (!empty($settings['title'])) {
    $output .= theme('panels_tabs_title', $settings['title']);
  }

  $output .= tabs_render($tabs);

  return $output;
}

/**
 * Render panel callback (legacy).
 */
function theme_panels_tabs_style_render_panel($display, $owner_id, $panes, $settings) {
  $output = '';

  // Generate a unique id based on the CSS ID and the name of the panel in the
  // layout.
  $id = '';
  //If CSS ID is set.
  if($display->css_id) {
    $id = "$display->css_id-";
  }
  $pane_id = reset(array_keys($panes));
  $id .= $display->content[$pane_id]->panel;

  // Add the Javascript to the page, and save the settings for this panel.
  _panels_tabs_add_js($id, $settings['filling_tabs']);

  $tabs = array();
  $tabs[$id] = array(
    '#type' => 'tabset',
    '#tabset_name' => $id,
  );
  $index = 0;
  foreach ($panes as $pane_id => $content) {
    // Remove the title from the content. We don't want titles in both the tab
    // and the content associated with the tab.
    if ($content->content) {
      $content_without_title = drupal_clone($content);
      unset($content_without_title->title);

      $tabs[$id][$pane_id] = array(
        '#type'    => 'tabpage',
        '#title'   => $content->title,
        '#content' => theme('panels_pane', $content_without_title, $display->content[$pane_id], $display),
        '#weight'  => $index,
      );
      $index++;
    }
  }

  // No content has been rendered
  if (empty($index)) {
    return;
  }

  // See if an optional title was added.
  $output = '';
  if (!empty($settings['title'])) {
    $output .= theme('panels_tabs_title', $settings['title']);
  }

  $output .= tabs_render($tabs);

  return $output;
}

/**
 * Settings form callback.
 */
function panels_tabs_style_settings_form($settings) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Optional Title'),
    '#default_value' => (isset($settings['title'])) ? $settings['title'] : '',
  );
  $form['filling_tabs'] = array(
    '#type' => 'radios',
    '#title' => t('Horizontally filling tabs'),
    '#options' => array(
      'disabled' => t('Disabled'),
      'equal' => t('Equal width tabs'),
      'smart' => t('Equal width scaling tabs'),
    ),
    '#description' => t(
      "Horizontally filling tabs make sure the tabs consume all available
      horizontal space.<br />
      Equal width tabs: sets the <em>width</em> property, forcing each tab
      to be equally wide. If the text doesn't fit in the tab, the overflow
      will be hidden.<br />
      Horizontally filling, smart width tabs: calculates the length of the
      text in each tab and compares this to the total length of the text on
      all tabs. It then sets the width property of each tab according to the
      percentage of text the tab contains."
    ),
    '#default_value' => (isset($settings['filling_tabs'])) ? $settings['filling_tabs'] : 'disabled',
  );
  return $form;
}

/**
 * Settings form validation callback.
 */
function panels_tabs_style_settings_validate($form, &$form_state) {
  if (!in_array($form_state['values']['filling_tabs'], array('disabled', 'equal', 'smart'))) {
    form_error($form_state['values']['filling_tabs'], t('Invalid value detected.'));
  }
}
