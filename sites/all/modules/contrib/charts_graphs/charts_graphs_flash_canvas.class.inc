<?php
// $Id: charts_graphs_flash_canvas.class.inc,v 1.1.2.3 2010/07/09 21:46:59 rsevero Exp $

/**
 * @file
 *   abstract class ChartsGraphsFlashCanvas.
 *
 */

/**
 * Abstract class to be derived for Flash based charting libraries.
 */
abstract class ChartsGraphsFlashCanvas extends ChartsGraphsCanvas {

  /**
   * Possible values are "window", "opaque" and "transparent".
   */
  var $wmode;

  function get_wmode() {
    $wmodes = self::wmode_values();
    $default = 'window';
    $wmode = strtolower(empty($this->wmode) ? $default : $this->wmode);
    return isset($wmodes[$wmode]) ? $wmode : $default;
  }

  static function wmode_values() {
    return array(
      'window' => 'Window (Default)',
      'opaque' => 'Opaque',
      'transparent' => 'Transparent',
    );
  }

  /**
   * Merges the two arrays.
   *
   * The final array will be a copy of the second array with the values of the
   * first whose IDs are not present in the second.
   *
   * IDs of the second array with a value of NULL are not included but these
   * same IDs are not copied from the first array, so they effectivelly delete
   * settings from the first array.
   *
   * @param <array> $first
   * @param <array> $second
   * @return <array>
   */
  function merge_xml_values($first, $second) {
//    dpm($first, '$first');
//    dpm($second, '$second');
    $second_ids = array();
    foreach ($second as $value) {
      $second_ids[$value['#id']] = TRUE;
    }
    $final = array();
    foreach ($first as $value) {
      if (!isset($second_ids[$value['#id']])) {
        $final[] = $value;
      }
    }
    foreach ($second as $value) {
      if (isset($value['#value']) && ($value['#value'] === NULL)) {
        continue;
      }
      $final[] = $value;
    }
    return $final;
  }

  function get_xml_file_from_array($array) {
    $dom = new DOMDocument('1.0', 'utf-8');
    $dom->formatOutput = TRUE;
//    var_dump($array);
    foreach ($array as $element) {
      $this->add_dom_element_from_array($dom, $dom, $element);
    }
//    var_dump($dom->saveXML());
    return $dom->saveXML();
  }

  function add_dom_element_from_array(&$dom, &$father, $element) {
    if (isset($element['#value'])) {
      $new_element = $dom->createElement($element['#id'], (string) $element['#value']);
    }
    else {
      $new_element = $dom->createElement($element['#id']);
      if (isset($element['#cdata'])) {
        $cdata = $dom->createCDATASection((string) $element['#cdata']);
        $new_element->appendChild($cdata);
      }
    }
    if (isset($element['#children'])) {
      foreach ($element['#children'] as $child) {
        $new_child = $this->add_dom_element_from_array(
          $dom,
          $new_element,
          $child
        );
        $new_element->appendChild($new_child);
      }
    }
    if (isset($element['#attributes'])) {
      foreach ($element['#attributes'] as $attribute_name => $attribute_value) {
        $attribute = $dom->createAttribute((string) $attribute_name);
        $new_element->appendChild($attribute);
        $value = $dom->createTextNode((string) $attribute_value);
        $attribute->appendChild($value);
      }
    }
    $father->appendChild($new_element);
//    var_dump($new_element;
//    var_dump($dom->saveXML($new_element));
    return $new_element;
  }

  function encode_for_xml($text) {
    return htmlspecialchars($text, ENT_COMPAT, 'UTF-8');
  }
}
