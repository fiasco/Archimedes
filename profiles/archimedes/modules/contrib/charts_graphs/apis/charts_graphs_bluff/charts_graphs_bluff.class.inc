<?php
// $Id: charts_graphs_bluff.class.inc,v 1.1.2.8 2010/11/17 23:19:02 rsevero Exp $

/**
 * @file
 *   Implementation of abstract class ChartsGraphsCanvas for Bluff library.
 *
 */

/**
 * Implementation of abstract class ChartsGraphsCanvas for Bluff library.
 */
class ChartsGraphsBluff extends ChartsGraphsCanvas {

  var $width = 450;
  var $height = 200;
  var $title = '';
  var $title_font_size = 32;

  /**
   * Parameters set directly by the user.
   *
   * @var <array>
   */
  var $parameters = array();
  
  /**
   * Optional parameter.
   * 
   * If set to TRUE, will cause Bluff to pick the opposite data orientation to
   * the one it would have picked automatically. This orientation defines if
   * each column or each row is one data serie.
   * 
   * @var <bool> 
   */
  var $transpose = NULL;

  protected function _get_translated_chart_type() {
    switch ($this->type) {
      case 'mini_bar':
        $type = 'Mini.Bar';
        break;

      case 'mini_pie':
        $type = 'Mini.Pie';
        break;

      case 'mini_side_bar':
        $type = 'Mini.SideBar';
        break;

      case 'side_bar':
        $type = 'SideBar';
        break;

      case 'stacked_side_bar':
        $type = 'SideStackedBar';
        break;

      case 'stacked_area':
        $type = 'StackedArea';
        break;

       case 'stacked_bar':
         $type = 'StackedBar';
         break;

      default:
        $type = ucfirst($this->type);
    }
    return $type;
  }

  protected function _initialize_final_parameters($chart_id, $transpose) {
    $parameters = array();
    $parameters['marker_font_size'] = 20;
    $parameters['hide_legend'] = 'true';
    $parameters['hide_title'] = 'false';
    $parameters['sort'] = 'false';
    $parameters['title_font_size'] = (int) $this->title_font_size;
    $parameters['tooltips'] = 'true';

    /**
     * Applying user defined min, max and step for y axis values.
     */
    if ($this->y_min) {
      $parameters['minimum_value'] = $this->y_min;
    }
    if ($this->y_max)   {
      $parameters['maximum_value'] = $this->y_max;
    }
    if ($this->y_step)  {
      $parameters['y_axis_increment'] = $this->y_step;
    }

    $series_colours = "['" . implode("', '", $this->series_colours) . "']";
    $marker_colour = '#aea9a9';
    $font_colour = '#000000';
    $background_colour = isset($this->colour) ?
      array($this->colour, $this->colour) :
      array(
        '#d1edf5',
        '#ffffff',
      );
    $background_colour = "['" . implode("', '", $background_colour) . "']";
    $set_theme = sprintf(
      "{
        colors: %s,
        marker_color: '%s',
        font_color: '%s',
        background_colors: %s
      }",
      $series_colours,
      $marker_colour,
      $font_colour,
      $background_colour
    );
    $parameters['set_theme'] = array($set_theme);

    $parameters['data_from_table'] = array(
      '"' . $chart_id . '"',
      ($transpose) ? 'true' : 'false'
    );

    if (is_array($this->parameters)) {
      foreach ($this->parameters as $user_parameter_key => $user_parameter) {
        $parameters[$user_parameter_key] = $user_parameter;
      }
    }
    $this->final_parameters = $parameters;
  }

  protected function _get_encoded_parameters() {
    $output = "\n";
    foreach ($this->final_parameters as $key => $value) {
      if (is_array($value)) {
        $function_parameters = '';
        foreach ($value as $val) {
          $function_parameters .= sprintf(', %s', $val);
        }
        if (strlen($function_parameters) > 0) {
          $function_parameters = substr($function_parameters, 2);
        }
        $output .= sprintf("\tg.%s(%s);\n", $key, $function_parameters);
      }
      else {
        $output .= sprintf("\tg.%s = %s;\n", $key, $value);
      }
    }
    return $output;
  }

  /**
   * Function that renders data.
   */
  public function get_chart() {
    $bluff_js_files = $this->get_bluff_js_files();

    foreach ($bluff_js_files as $file_path) {
      drupal_add_js($file_path);
    }

    $bluff_path = drupal_get_path('module', 'charts_graphs_bluff');
    drupal_add_css($bluff_path . '/charts_graphs_bluff.css');

    $x_labels = $this->x_labels;
    $series = $this->series;
    $chart_id = 'bluffchart-' . charts_graphs_chart_id_generator();
    $table = array();

    $table[] = sprintf(<<<TABLE
      <table id="%s" class="bluff-data-table">
        <caption>%s</caption>
        <thead>
          <tr>
            <th scope="col"></th>
TABLE
      ,
        $chart_id,
        $this->title
    );

    $cols = array_keys($series);
    foreach ($cols as $col) {
      $table[] = sprintf("<th scope='col'>%s</th>\n", $col);
    }

    $table[] = "</tr></thead><tbody>\n";

    foreach ($x_labels as $key => $label) {
      $table[] = "<tr>\n";
      $cols = array($label);

      foreach ($series as $serie) {
        $cols[] = $serie[$key];
      }

      $table[] = sprintf("<th scope='row'>%s</th>\n", array_shift($cols));

      foreach ($cols as $col) {
        $table[] = sprintf("<td>%s</td>\n", (string) $col);
      }

      $table[] = "</tr>\n";
    }

    $table[] = "</tbody></table>\n";

    $is_pie_chart = ($this->type == 'pie');

    $transpose = isset($this->transpose) ? $this->transpose : $is_pie_chart;

    $this->_initialize_final_parameters($chart_id, $transpose);
    $html = implode('', $table);

    $javascript = '
      <canvas id="%chart_id-graph" width="%width" height="%height"></canvas>
      <script type="text/javascript">
        var ChartsAndGraphs = ChartsAndGraphs || {};

        ChartsAndGraphs.init = function() {
          var g = new Bluff.%type("%chart_id-graph", "%widthx%height");
          ';
    $javascript .= $this->_get_encoded_parameters();
    $javascript .= '
          g.draw();

          var g_labels = %json_encode;
          var legend = ["<ul class=\"bluff-legend\">"];

          for (var i = 0, j = 0, color; i < g_labels.length; i++, j++) {
            if (g.colors[j]) {
              color = g.colors[j]
            }
            else {
              g.colors[(0)];
              j = 0;
            }
            legend.push("<li>");
            legend.push("<div style=\"background-color: " + color + "\"><\/div>" + g_labels[i]);
            legend.push("<\/li>");
          }

          legend.push("<\/ul>");

          $("#%chart_id-graph")
            .parent("div.bluff-wrapper")
            .append(legend.join(""))
            .css({height: "auto"});
        }

        $(window).load(ChartsAndGraphs.init);

        Drupal.behaviors.ChartsAndGraphs_init = function(context) {
          ChartsAndGraphs.init();
        }
      </script>';

    $javascript = strtr(
      $javascript,
      array(
        '%chart_id' => $chart_id,
        '%type' => $this->_get_translated_chart_type(),
        '%width' => $this->width,
        '%height' => $this->height,
        '%json_encode' => json_encode($is_pie_chart ? $x_labels : array_keys($series)),
    ));

    return $html . $javascript;
  }

  /**
   * Cache list of javascript files for performance.
   */
  function get_bluff_js_files() {
    static $js_files = NULL;

    if (is_array($js_files)) {
      return $js_files;
    }

    $bluff_path = drupal_get_path('module', 'charts_graphs_bluff');
    $bluff_files = array_map('basename', glob(dirname(__FILE__) . '/bluff/*.js'));

    rsort($bluff_files);

    $js_files = array();

    foreach ($bluff_files as $bluff_file) {
      $file_path = sprintf('%s/bluff/%s', $bluff_path, $bluff_file);
      $js_files[] = $file_path;
    }

    return $js_files;
  }
}
