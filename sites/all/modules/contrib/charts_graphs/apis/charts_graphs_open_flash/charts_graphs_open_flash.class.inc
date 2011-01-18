<?php
// $Id: charts_graphs_open_flash.class.inc,v 1.1.2.12 2010/11/18 01:52:04 rsevero Exp $

/**
 * @file
 *   Implementation of abstract class ChartsGraphsFlashCanvas for Open Charts
 * Flash 2 library.
 *
 */

require_once dirname(__FILE__) . '/../../charts_graphs_flash_canvas.class.inc';

/**
 * Implementation of abstract class ChartsGraphsFlashCanvas for Open Charts
 * Flash 2 library.
 */
class ChartsGraphsOpenFlash extends ChartsGraphsFlashCanvas {

  /**
   * Holds the type definition translated to Open Charts Flash 2 types.
   *
   * @var <string>
   */
  var $translated_type;

  /**
   * @param $cid
   *   cache_id from which cache to retrieve the data
   */
  function get_data_from_cache($cid = NULL) {
    $cache = cache_get($cid);
    if (!$cache) {
      drupal_not_found();
      exit();
    }
    $canvas = $cache->data;

    if (empty($canvas) || !is_object($canvas) ||
      !is_array($canvas->series) || empty($canvas->type)) {
      drupal_not_found();
      exit();
    }

    $this->title = new stdClass();
    $this->title->text = $canvas->title;
    $this->title->style = 'font-size: 20px; color:#0000ff; font-family: Verdana; text-align: center;';

    $this->type = $canvas->translated_type;

    $this->y_legend = new stdClass();
    $this->y_legend->text = $canvas->y_legend;
    $this->y_legend->style = '{color: #736AFF; font-size: 12px;}';

    /**
     * Applying background colour setting if available.
     */
    if (isset($canvas->colour) && !empty($canvas->colour)) {
      $this->bg_colour = $canvas->colour;
    }

    $y = new stdClass();
    $y->grid_colour = '#00ff00';
    $y->offset = 50;
    $this->y_axis = $y;

    $x = new stdClass();
    $x->colour = '#909090';
    $x->grid_colour = '#00ff00';

    /**
     * Some kind of bug: if labels are not PHP "strings" they do not render.
     * Sigh.
     */
    foreach ($canvas->x_labels as &$label) {
      $label = (string) $label;
    }

    $x->labels->labels = $canvas->x_labels;
    $this->x_axis = $x;

    $series_colours = array_values($canvas->series_colours);

    /**
     * Initializing $min and $max.
     */
    $val = reset($canvas->series);
    $max_value = reset($val);
    $min_value = $max_value;

    $i = 0; // for colours
    foreach ($canvas->series as $key => $val) {
      $obj = new stdClass();
      $val = $this->_preprocess_values($val);
      $obj->values = $val;
      if ($this->type == 'pie') {
        $obj->tip = '#label# #val# (#percent#)';
        $obj->{'label - colour'} = '#432BAF';
      }
      else {
        $max_value_arr = max($val);
        if ($max_value < $max_value_arr) {
          $max_value = $max_value_arr;
        }
        $min_value_arr = min($val);
        if ($min_value > $min_value_arr) {
          $min_value = $min_value_arr;
        }
      }
      $obj->text = $key;
      $obj->alpha = .5;
      $obj->type = $canvas->type;
      $obj->colour = $series_colours[$i];
      $this->elements[] = $obj;
      $i++;
    }

    if ($this->type != 'pie') {
      $y_step = abs(($max_value - $min_value) / 10);

      $this->x_axis->{'3d'} = 5;
      $this->y_axis->max = $max_value + $max_value / 10;
      if ($this->y_axis->max > 10) {
        $this->y_axis->max = (int) $this->y_axis->max;
      }
      $this->y_axis->min = $min_value;
      if ($y_step > 5) {
        $y_step = (int) $y_step;
      }
      $this->y_axis->steps = $y_step;

      /**
       * Applying user defined min, max and step for y axis values.
       */
      if (isset($canvas->y_min)) {
        $this->y_axis->min = $canvas->y_min;
      }
      if (isset($canvas->y_max)) {
        $this->y_axis->max = $canvas->y_max;
      }
      if (isset($canvas->y_step)) {
        $this->y_axis->steps = $canvas->y_step;
      }
    }
  }

  /**
   * Pie-chart has different format for $values array than bar chart etc.
   * This method deals with permutations across chart types. Sigh.
   *
   * We also remove items with no label, while we are at it, since
   * those can cause problems to Flash renderer.
   */
  function _preprocess_values($values) {
    $labels = $this->x_axis->labels->labels;
    $i = 0;

    $series_colours = array_values($this->series_colours());

    switch ($this->type) {
      case 'pie':
        $new_vals = array();
        foreach ($values as $val) {
          // An accidental empty label causes SWF to go nuts.
          if (!empty($labels[$i]) && ($labels[$i] != 'null')) {
            $obj = new stdClass();
            $obj->value = $val;
            $obj->label = $labels[$i];
            $obj->colour = $series_colours[$i];
            $new_vals[] = $obj;
          }
          $i++;
        }
        return $new_vals;

      /**
       * Default action is just filtering values with nulled labels (leftovers
       * from out joins).
       */
      default: 
        $new_vals = array();
        $new_labels = array();
        foreach ($values as $val) {
          // An accidental empty label causes SWF to go nuts.
          if (!empty($labels[$i]) && ($labels[$i] != 'null')) {
            $new_vals[] = $val;
            $new_labels[] = $labels[$i];
          }
          $i++;
        }
        $this->x_axis->labels->labels = $new_labels;
        return $new_vals;
    }
  }

  /**
   * Translate Charts and Graphs graph type to Open Charts Flash 2 types.
   *
   * It currently doesn't nothing of value but will leave it here in case some
   * brave soul decides to implement horizontal bar or area or some other graph
   * type.
   *
   * @return <string>
   */
  protected function _get_translated_chart_type() {
    switch ($this->type) {
      default:
        $type = $this->type;
    }

    return $type;
  }

  /**
   * Function that renders data.
   */
  function get_chart() {
    global $base_url;
    $unique = charts_graphs_random_hash();

    // Make current object a StdClass() for easier de-serialization
    $this->translated_type = $this->_get_translated_chart_type();
    $arr = (array) $this;
    $generic = (object) $arr;

    //Keep for at least 30 seconds;
    cache_set($unique, $generic, 'cache', time() + 30); 

    $mod_path = drupal_get_path('module', $this->getModuleName());
    $openflash_swf_uri = $base_url . '/' . $mod_path . '/open-flash-chart.swf';

    $data_URL = url('charts_graphs_open_flash/data/' . $unique, array('absolute' => TRUE));

    /** For debugging
     * $ret = drupal_http_request( $data_URL );
     * echo "<pre>".print_r ( $ret,true)."</pre>";
     * exit();
     * */
    $wmode = $this->get_wmode();

    $flashvars = array(
      'data-file' => 'SWFDATAURL',
      'preloader_color' => '#999999',
      'wmode' => $wmode);

    $args = array(
      'params' => array(
        'width' => $this->width,
        'height' => $this->height,
        'wmode' => $wmode
      ),
      'flashvars' => $flashvars
    );

    $out = swf($openflash_swf_uri, $args);
    $out = str_replace('SWFDATAURL', $data_URL, $out);

    return $out;
  }
}
