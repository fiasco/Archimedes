<?php
// $Id: charts_graphs_canvas.class.inc,v 1.1.2.3 2010/11/17 02:37:04 rsevero Exp $

/**
 * @file
 *   abstract class ChartsGraphsCanvas.
 *
 */

/**
 * Each graphing library implementation should create a derivative of this
 * class.
 */
abstract class ChartsGraphsCanvas {

  var $title;
  var $height;
  var $width;
  var $type;

  /**
   * Text presented next to the left Y axis.
   *
   * @var <string>
   */
  var $y_legend;

  /**
   * Identifies each value in a data series. The same identification is used for
   * all data series.
   *
   * @var <array>
   */
  var $x_labels;

  /**
   * An array of series of values. Each element in a series (which is also an
   * array in itself) is a numeric value of "measurement".
   *
   * <p>e.g.:
   * <pre>
   *  $canvas->series = array(
   *    'Some Value' => array(9,6,7,9,5,7,6,9,7),
   *    'Page Views' => array(6,7,9,5,7,6,9,7,3),
   *  );
   * </pre>
   */
  var $series;

  /**
   * Holds the colours to be used for each data seire.
   *
   * @var array
   */
  var $series_colours;

  // Should not set those from client code, only from an implementation.
  protected $modulename;

  // The name of the implementing module.
  protected $unique_id;

  function getModuleName() {
    return $this->modulename;
  }

  function getUnique_ID() {
    return $this->unique_id;
  }

  /**
   * Function that renders data.
   */
  abstract function get_chart();

  /*
   * Function that preprocesses generalized data structure and saves it in the
   * form suitable for specific charting implementation.
   *
   * @param $rows an array of arrays of data in the following format:
   * <pre>
   *  array(
   *    'Some Value' => array(9,6,7,9,5,7,6,9,7),
   *    'Page Views' => array(6,7,9,5,7,6,9,7,3),
   * );
   *
   * @param $x_labels an array of x-axis labels
   */
  function set_data($rows, $x_labels) {
    $this->series = $rows;
    $this->x_labels = $x_labels;
  }

  /**
   * Constructor function.
   *
   * <p>ATTENTION: This function should NOT be called directly.
   * You should always instantiate a chart implementation
   * class using charts_graphs_get_graph() factory function.
   *
   * <p>CAUTION: PHP does not implicitely call parent constructors. DO NOT
   * override this function, or if you do: make sure to call it from the
   * child implementation with: parent::__construct();
   * within the child constructor.
   */
  function __construct($modulename) {
    $this->modulename = $modulename;
    $this->unique_id = charts_graphs_chart_id_generator();

    // Set some default values
    $this->width = 500;
    $this->height = 250;
    $this->title = t('A Drupal Chart');
    $this->series_colours = array(
      '#0000FF',
      '#FF0000',
      '#FFFF00',
      '#87907D',
      '#21B6A8',
      '#177F75',
      '#B6212D',
      '#7F171F',
      '#B67721',
      '#7F5417',
      '#FF8000',
      '#FFC080',
      '#FFDFBF',
      '#FFC080',
      '#FFCC00',
      '#FFE500',
      '#FFF9BF',
      '#78C0E9',
      '#179CE8',
      '#30769E',
      '#C8E9FC',
      '#ECF8FF',
      '#00CCFF',
      '#4086AA',
      '#91C3DC',
    );
  }

  function series_colours() {
    if (!is_array($this->series_colours)) {
      $colours = explode(',', $this->series_colours);
      $this->series_colours = array();
      foreach ($colours as $colour) {
        $this->series_colours[] = trim($colour);
      }
    }
    return $this->series_colours;
  }
}
