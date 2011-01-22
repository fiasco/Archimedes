<?php
// $Id: reverse_node_reference_handler_relationship.inc,v 1.1.2.1 2010/05/13 14:29:19 markuspetrux Exp $

/**
 * @file
 * Handles reverse node references using a nested join and deals properly with
 * multiple values by allowing the views administrator to select deltas.
 *
 * Tipical relationships for node reference fields generate two joins that
 * look like this:
 *
 * @code
 *   FROM {node} node
 *     LEFT JOIN {content_field_table} node_data_field_table ON node.vid = node_data_field_table.vid
 *     {join-type} JOIN {node} node_node_data_field_table ON node_data_field_table.field_nid = node_node_data_field_table.nid
 * @code
 *
 * However, this kind of joins handle relationships between referrer nodes
 * and their parents. A referrer node can display information about the
 * referred node, but not the other way around. We cannot display fields
 * of the referring node from a parent node because the link goes from child
 * to parent.
 * To provide a relationship from parent to child, we need to generate a join
 * from the nid of the parent node to the reference field on the referrer node,
 * and because this is using the nid, we should do it in a way that does not
 * generate duplicate records when the referrer nodes have support for node
 * revisions enabled.
 *
 * This can be done using a nested join like the following:
 *
 * @code
 *   FROM {node} node
 *     {join-type} JOIN (
 *       {content_field_table} node_data_field_table
 *       INNER JOIN {node} node_node_data_field_table ON node_data_field_table.vid = node_node_data_field_table.vid
 *     ) ON node.nid = node_data_field_table.field_nid
 * @code
 *
 * Note that {join-type} depends on whether the relationship is
 * required (INNER) or not (LEFT).
 */
class reverse_node_reference_handler_relationship extends views_handler_relationship {
  var $content_field;

  function construct() {
    parent::construct();

    $this->content_field = content_fields($this->definition['content_field_name']);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['delta'] = array('default' => -1);

    return $options;
  }

  /**
   * Add a delta selector for multiple fields.
   */
  function options_form(&$form, &$form_state) {
    $field = $this->content_field;
    parent::options_form($form, $form_state);

    // Only add the form gadget if the field is multiple.
    if ($field['multiple']) {
      $max_delta = $field['multiple'];
      // 1 means unlimited.
      if ($max_delta == 1) {
        $max_delta = 10;
      }

      $options = array('-1' => t('All'));
      for ($i = 0; $i < $max_delta; $i++) {
        $options[$i] = $i + 1;
      }
      $form['delta'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $this->options['delta'],
        '#title' => t('Delta'),
        '#description' => t('The delta allows you to select which item in a multiple value field to key the relationship off of. Select "1" to use the first item, "2" for the second item, and so on. If you select "All", each item in the field will create a new row, which may appear to cause duplicates.'),
      );
    }
  }

  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      if (!method_exists($this->query, 'ensure_table')) {
        vpr_trace();
        exit;
      }

      // Since nested joins are not directly supported by Views, we are going
      // to do here part of the job performed by views_query::ensure_table().

      if (empty($this->relationship)) {
        $this->relationship = $this->query->base_table;
      }

      if ($this->relationship == $this->query->base_table && !empty($this->query->tables[$this->relationship][$this->table])) {
        $this->table_alias = $this->query->tables[$relationship][$this->table]['alias'];
      }
      elseif (!array_key_exists($this->relationship, $this->query->relationships)) {
        $this->table_alias = FALSE;
      }
      elseif ($this->table == $this->query->relationships[$this->relationship]['base']) {
        $this->table_alias = $this->relationship;
      }
      else {
        // Build the main join based on the views data cache.
        $main_join = $this->query->get_join_data($this->table, $this->query->relationships[$this->relationship]['base']);

        // We want to alter the join so that it uses the field column instead
        // of the vid.
        $main_join_definition = $main_join->definition;
        $main_join_definition['left_table'] = $this->relationship;
        $main_join_definition['left_field'] = $this->definition['left_field'];
        $main_join_definition['field'] = $this->definition['field'];
        if (!empty($this->options['required'])) {
          $main_join_definition['type'] = 'INNER';
        }

        $delta = isset($this->options['delta']) ? $this->options['delta'] : -1;
        if ($delta != -1) {
          if (!isset($main_join_definition['extra'])) {
            $main_join_definition['extra'] = array();
          }
          $main_join_definition['extra'][] = array(
            'field' => 'delta',
            'value' => $delta,
            'numeric' => TRUE,
          );
        }

        // Build a new join object with our new definition.
        $main_join = new reverse_node_reference_nested_join();
        $main_join->definition = $main_join_definition;
        $main_join->construct();
        $main_join->adjusted = TRUE;
        $this->table_alias = $this->query->ensure_table($this->table, $this->relationship, $main_join);

        // Let's build the nested join.
        $nested_join_definition = $this->definition;
        $nested_join_definition['table'] = $this->definition['base'];
        $nested_join_definition['left_field'] = $nested_join_definition['field'] = $nested_join_definition['base field'];
        $nested_join_definition['left_table'] = $this->table_alias;
        $nested_join_definition['type'] = 'INNER';

        $nested_join = new reverse_node_reference_nested_join();
        $nested_join->definition = $nested_join_definition;
        $nested_join->construct();
        $nested_join->adjusted = TRUE;
        $nested_alias = $nested_join_definition['table'] .'_'. $this->table;

        $this->alias = $this->query->add_relationship($nested_alias, $nested_join, $this->definition['base'], $this->relationship);

        $main_join->_set_nested_join($nested_join, $this->alias);
      }
    }
    return $this->table_alias;
  }

  function query() {
    $this->ensure_my_table();
  }
}

class reverse_node_reference_nested_join extends views_join {
  var $nested_join;
  var $nested_alias;

  function _set_nested_join($nested_join, $nested_alias) {
    $this->nested_join = $nested_join;
    $this->nested_alias = $nested_alias;
  }

  function join($table, &$query, $build_nested_join = FALSE) {
    if (!isset($this->nested_join)) {
      if ($build_nested_join) {
        return parent::join($table, $query);
      }
      return '';
    }
    $output = parent::join($table, $query);
    $nested_join = $this->nested_join->join($query->table_queue[$this->nested_alias], $query, TRUE);
    $output = preg_replace('`JOIN ({[_a-z0-9]+} [_a-z0-9]+) ON`', "JOIN (\n\\1\n{$nested_join}\n) ON", $output);
    return $output;
  }
}
