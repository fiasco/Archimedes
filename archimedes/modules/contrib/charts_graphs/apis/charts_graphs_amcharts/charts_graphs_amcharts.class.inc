<?php
// $Id: charts_graphs_amcharts.class.inc,v 1.1.2.18 2010/11/17 02:37:03 rsevero Exp $

/**
 * @file
 *   Implementation of abstract class ChartsGraphsFlashCanvas for amCharts library.
 *
 */

require_once dirname(__FILE__) . '/../../charts_graphs_flash_canvas.class.inc';

/**
 * Implementation of abstract class ChartsGraphsFlashCanvas for amCharts library.
 */
class ChartsGraphsAmcharts extends ChartsGraphsFlashCanvas {

  /**
   * Array defined by the user to be placed in the pie section of the data file
   * of the chart.
   *
   * @var <array>
   */
  var $slices = array();

  /**
   * Array defined by the user to be placed in the series subsection of the data
   * file of the chart.
   *
   * @var <array>
   */
  var $chart_series = array();

  /**
   * Array defined by the user to be placed in the graphs subsection of the data
   * file of the chart.
   *
   * @var <array>
   */
  var $chart_graphs = array();

  /**
   * Array defined by the user to be placed in the settings section of the
   * settings file of the chart.
   *
   * @var <array>
   */
  var $settings = array();

  function _preprocess_values() {
    if (in_array($this->type, array('pie', 'donut', 'pie_3d', 'donut_3d'))) {
      $this->_preprocess_values_pie();
    }
    else {
      $this->_preprocess_values_not_pie();
    }
  }

  function _preprocess_values_pie() {
    $pie_data = array();
    $keys = array_keys($this->series);
    $first_key = $keys[0];
    $first_series = $this->series[$first_key];
    $idx = 0;
    $max_val = max($first_series);
    $max_idx = -1;
    if (is_array($first_series)) {
      foreach ($first_series as $val) {
        $pie_data[$this->x_labels[$idx]] = $val;
        if ($val == $max_val) {
          $max_idx = $idx;
        }
        $idx++;
      }
    }

    $slices = array();
    if (is_array($pie_data)) {
      $idx = 0;
      foreach ($pie_data as $label => $point) {
        $slice = array(
          '#id' => 'slice',
          '#attributes' => array(
            'title' => $this->encode_for_xml($label),
          ),
          '#value' => $this->encode_for_xml($point),
        );
        if ($idx == $max_idx) {
          $slice['#attributes']['pull_out'] = 'true';
        }
        $slices[] = $slice;
        $idx++;
      }
    }

    $slices = $this->merge_xml_values($slices, $this->slices);

    $pie = array(
      '#id' => 'pie',
      '#children' => $slices,
    );
    $this->data_to_use = array($pie);
  }

  function _preprocess_values_not_pie() {
    $series = array();
    if (is_array($this->x_labels)) {
      $i = 1;
      /**
       * Making amCharts side bar graphs have the same orientation of side bar
       * graphs made with Bluff and Google Charts.
       */
      $x_labels = ($this->type == 'side_bar') ?
        array_reverse($this->x_labels) :
        $this->x_labels;
      foreach ($x_labels as $label) {
        $series[] = array(
          '#id' => 'value',
          '#value' => $this->encode_for_xml($label),
          '#attributes' => array(
            'xid' => $i,
          ),
        );
        $i++;
      }
    }

    $graphs = array();
    if (is_array($this->series)) {
      /**
       * Making amCharts side bar graphs have the same orientation of side bar
       * graphs made with Bluff and Google Charts.
       */
      $ordered_series = ($this->type == 'side_bar') ?
        array_reverse($this->series, TRUE) :
        $this->series;
      foreach ($ordered_series as $name => $graph) {
        if (!is_array($graph)) {
          continue;
        }
        $j = 1;
        $values = array();
        /**
         * Making amCharts side bar graphs have the same orientation of side bar
         * graphs made with Bluff and Google Charts.
         */
        $ordered_graph = ($this->type == 'side_bar') ?
          array_reverse($graph, TRUE) :
          $graph;
        foreach ($ordered_graph as $val) {
          $values[] = array(
            '#id' => 'value',
            '#value' => $this->encode_for_xml($val),
            '#attributes' => array(
              'xid' => $j,
            ),
          );
          $j++;
        }
        $new_graph = array(
          '#id' => 'graph',
          '#attributes' => array(
            'title' => $this->encode_for_xml($name),
            'bullet' => 'round',
          ),
          '#children' => $values,
        );
        if (strpos($this->type, 'area') !== FALSE) {
          $new_graph['#attributes']['fill_alpha'] = 50;
        }
        $graphs[] = $new_graph;
      }
    }

    $series = $this->merge_xml_values($series, $this->chart_series);
    $graphs = $this->merge_xml_values($graphs, $this->chart_graphs);
    $series = array('#id' => 'series', '#children' => $series);
    $graphs = array('#id' => 'graphs', '#children' => $graphs);

    $chart = array(
      '#id' => 'chart',
      '#children' => array($series, $graphs),
    );

    $this->data_to_use = array($chart);
  }

  function get_chart() {
    $unique = charts_graphs_random_hash();

    $am_column_type = NULL;
    $am_y_axe_type = NULL;
    $inner_radius = NULL;
    $angle_pie_3d = NULL;
    $height_pie_3d = NULL;

    switch ($this->type) {
      case 'bar':
        $am_category = 'amcolumn';
        $am_type = 'column';
        break;

      case 'stacked_bar':
        $am_category = 'amcolumn';
        $am_type = 'column';
        $am_column_type = 'stacked';
        break;

      case '100_stacked_bar':
        $am_category = 'amcolumn';
        $am_type = 'column';
        $am_column_type = '100% stacked';
        break;

      case 'bar_3d':
        $am_category = 'amcolumn';
        $am_type = 'column';
        $am_column_type = '3d column';
        break;

      case 'side_bar':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        break;

      case 'stacked_side_bar':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        $am_column_type = 'stacked';
        break;

      case '100_stacked_side_bar':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        $am_column_type = '100% stacked';
        break;

      case 'side_bar_3d':
        $am_category = 'amcolumn';
        $am_type = 'bar';
        $am_column_type = '3d column';
        break;

      case 'line':
        $am_category = 'amline';
        $am_type = 'line';
        break;

      case 'area':
        $am_category = 'amline';
        $am_type = 'line';
        break;

      case 'stacked_area':
        $am_category = 'amline';
        $am_type = 'line';
        $am_y_axe_type = 'stacked';
        break;

      case '100_stacked_area':
        $am_category = 'amline';
        $am_type = 'line';
        $am_y_axe_type = '100% stacked';
        break;

      case 'pie':
        $am_category = 'ampie';
        $am_type = 'pie';
        break;

      case 'donut':
        $am_category = 'ampie';
        $am_type = 'pie';
        $inner_radius = '30%';
        break;

      case 'pie_3d':
        $am_category = 'ampie';
        $am_type = 'pie';
        $angle_pie_3d = 30;
        $height_pie_3d = 20;
        break;

      case 'donut_3d':
        $am_category = 'ampie';
        $am_type = 'pie';
        $inner_radius = '30%';
        $angle_pie_3d = 30;
        $height_pie_3d = 20;
        break;

    }

    $width = $this->width;
    $height = $this->height;

    $data = $this->_preprocess_values();

    $settings = array(
      array('#id' => 'type', '#value' => $am_type),
      array('#id' => 'depth', '#value' => 5),
      array('#id' => 'js_enabled', '#value' => 'false'),
      array('#id' => 'redraw', '#value' => 'true'),
      array(
        '#id' => 'data_labels',
        '#children' => array(
          array(
            '#id' => 'show',
            '#cdata' => '{title}: {value}',
          ),
          array(
            '#id' => 'line_color',
            '#value' => '#FFFFFF',
          ),
          array(
            '#id' => 'line_alpha',
            '#value' => 40,
          ),
        ),
      ),
    );

    /**
     * Applying background colour setting if available.
     */
    if (isset($this->colour) && !empty($this->colour)) {
      $settings[] = array(
        '#id' => 'background',
        '#children' => array(
          array(
            '#id' => 'color',
            '#value' => $this->encode_for_xml($this->colour),
          ),
          array(
            '#id' => 'alpha',
            '#value' => 100,
          ),
        ),
      );
    }

    /**
     * Applying user defined min and max y axis values.
     */
    if ((($this->type == 'bar') || ($this->type == 'line')) &&
      (isset($this->y_min) || isset($this->y_max))) {

      $children = array(array(
        '#id' => 'strict_min_max',
        '#value' => 'true',
      ));
      if (isset($this->y_min)) {
        $children[] = array(
          '#id' => 'min',
          '#value' => $this->y_min,
        );
      }
      if (isset($this->y_max)) {
        $children[] = array(
          '#id' => 'max',
          '#value' => $this->y_max,
        );
      }
      $y_axis_limits_container = ($this->type == 'bar') ? 'value' : 'y_left';
      $settings[] = array(
        '#id' => 'values',
        '#children' => array(
          array(
            '#id' => $y_axis_limits_container,
            '#children' => $children,
          )
        )
      );
    }

    if (isset($this->y_legend)) {
      $label = array(
        array('#id' => 'x', '#value' => '2%'),
        array('#id' => 'y', '#value' => '100%'),
        array('#id' => 'align', '#value' => 'center'),
        array('#id' => 'rotate', '#value' => 'true'),
        array('#id' => 'text', '#value' => strip_tags($this->y_legend)),
      );
      $settings[] = array(
        '#id' => 'labels',
        '#children' => $label,
      );
    }

    if (isset($am_column_type)) {
      $column = array(
        '#id' => 'column',
        '#children' => array('#id' => 'type', '#value' => $am_column_type),
      );
      $settings[] = $column;
    }

    if (isset($am_y_axe_type)) {
      $y_axe_type = array(
        '#id' => 'axes',
        '#value' => array(
          '#id' => 'y_left',
          '#value' => array(
            '#id' => 'type',
            '#value' => $am_y_axe_type,
          ),
        ),
      );
      $settings[] = $y_axe_type;
    }

    if ($am_type == 'pie') {
      $pie = array(
        array('#id' => 'hover_brightness', '#value' => -20),
        array('#id' => 'gradient', '#value' => 'linear'),
        array('#id' => 'gradient_ratio', '#value' => '-10,60'),
      );
      if ($inner_radius !== NULL) {
        $pie[] = array('#id' => 'inner_radius', '#value' => $inner_radius);
      }
      if ($angle_pie_3d !== NULL) {
        $pie[] = array('#id' => 'angle', '#value' => $angle_pie_3d);
      }
      if ($height_pie_3d !== NULL) {
        $pie[] = array('#id' => 'height', '#value' => $height_pie_3d);
      }
      $colors = array_slice(
        $this->series_colours(),
        0,
        count(reset($this->series))
      );
      $pie[] = array(
        '#id' => 'colors',
        '#value' => implode(',', $colors),
      );

      $pie = array('#id' => 'pie', '#children' => $pie);
      $settings[] = $pie;
    }
    else {
      $colors = array_slice($this->series_colours(), 0, count($this->series));
      $settings[] = array(
        '#id' => 'colors',
        '#value' => implode(',', $colors),
      );
    }

    $settings = $this->merge_xml_values($settings, $this->settings);
    $settings = array('#id' => 'settings', '#children' => $settings);

    $this->settings_to_use = array($settings);

    $wmode = $this->get_wmode();

    $mod_path = drupal_get_path('module', $this->getModuleName());
    $file = url(
      "${mod_path}/downloaded/${am_category}.swf",
      array('absolute' => TRUE)
    );

    //-- Prepare URLs that javascript will retrieve data from, using cache:
    $unique = charts_graphs_random_hash();
    $arr = (array) $this;
    $tocache = new stdClass();
    $tocache->settings = $this->get_xml_file_from_array($this->settings_to_use);
    $tocache->data = $this->get_xml_file_from_array($this->data_to_use);
    cache_set($unique, $tocache, 'cache', time() + 30); //Keep for at least 30 seconds;

    $settings_url_query = sprintf('cid=%s', $unique);
    $settings_url = url(
      'charts_graphs_amcharts/getdata/settings',
      array(
        'absolute' => TRUE,
        'query' => $settings_url_query,
      )
    );

    $data_url_query = sprintf('cid=%s', $unique);
    $data_url = url(
      'charts_graphs_amcharts/getdata/data',
      array(
        'absolute' => TRUE,
        'query' => $data_url_query,
      )
    );

    $flashvars = array(
      'settings_file' => 'SWFSETTINGSURL',
      'data_file' => 'SWFDATAURL',
      'preloader_color' => '#999999',
      'wmode' => $wmode,
    );
    if (isset($this->key) && !empty($this->key)) {
      $flashvars['key'] = $this->key;
    }

    $args = array(
      'params' => array(
        'width' => $width,
        'height' => $height,
        'wmode' => $wmode,
      ),
      'flashvars' => $flashvars,
    );

    $out = swf($file, $args);
    $out = str_replace('SWFSETTINGSURL', $settings_url, $out);
    $out = str_replace('SWFDATAURL', $data_url, $out);

    return $out;
  }
}
