<?php
// $Id: nodereferrer.views.inc,v 1.1.2.3 2009/10/28 00:58:24 andypost Exp $

/**
 * @file
 * nodereferrer.module Views integration
 */

function nodereferrer_views_data_alter(&$cache) {
  $cache['node']['nodereferrer_field'] = array(
    'group' => t('Node'),
    'title' => t('Nodereferrer: by field'),
    'real field' => 'nid',
    'help' => t('Allows views to query the node\'s references by a certain node reference field.'),
    'filter' => array(
      'handler' => 'nodereferrer_view_handler_filter_field',
    ),
    'argument' => array(
      'handler' => 'nodereferrer_view_handler_argument_field',
    ),
    'field' => array(
      'handler' => 'nodereferrer_view_handler_field_field',
    ),
  );

  $cache['node']['nodereferrer_type'] = array(
    'group' => t('Node'),
    'title' => t('Nodereferrer: by node type'),
    'real field' => 'nid',
    'help' => t('Allows views to query the node\'s references by a certain node type.'),
    'filter' => array(
      'handler' => 'nodereferrer_view_handler_filter_type',
    ),
    'argument' => array(
      'handler' => 'nodereferrer_view_handler_argument_type',
    ),
    'field' => array(
      'handler' => 'nodereferrer_view_handler_field_type',
    ),
  );

  $cache['node']['nodereferer_referers'] = array(
    'group' => t('Node'),
    'relationship' => array(
      'title' => t('Referrers'),
      'label' => t('Referencing Nodes'),
      'help' => t('Add a relationship to gain access to referencing nodes.'),
      'relationship table' => 'node',
      'relationship field' => 'nid',
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'nodereferrer_view_handler_relationship',
    ),
  );
}

/**
 * Implementation of hook_views_handlers
 */
function nodereferrer_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'nodereferrer') . '/views',
    ),
    'handlers' => array(
      'nodereferrer_view_handler_filter' => array(
        'parent' => 'views_handler_filter',
      ),
      'nodereferrer_view_handler_filter_field' => array(
        'parent' => 'nodereferrer_view_handler_filter',
      ),
      'nodereferrer_view_handler_filter_type' => array(
        'parent' => 'nodereferrer_view_handler_filter',
      ),
      'nodereferrer_view_handler_argument' => array(
        'parent' => 'views_handler_argument',
      ),
      'nodereferrer_view_handler_argument_field' => array(
        'parent' => 'nodereferrer_view_handler_argument',
      ),
      'nodereferrer_view_handler_argument_type' => array(
        'parent' => 'nodereferrer_view_handler_argument',
      ),
      'nodereferrer_view_handler_field' => array(
        'parent' => 'views_handler_field',
      ),
      'nodereferrer_view_handler_field_field' => array(
        'parent' => 'nodereferrer_view_handler_field',
      ),
      'nodereferrer_view_handler_field_type' => array(
        'parent' => 'nodereferrer_view_handler_field',
      ),
      'nodereferrer_view_handler_relationship' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

/**
 * Given a node id, field names and content names, returns the list of nodes
 * that refer to the given node. This takes translation settings into account
 */
function nodereferrer_referrers_with_translations($nid, $fields, $types, $ref_of_trans, $trans_of_ref) {
  if (!module_exists('translation')) {
    return nodereferrer_referrers($nid, $fields, $types);
  }

  $ref = nodereferrer_referrers($nid, $fields, $types, $ref_of_trans);

  if ($trans_of_ref && count($ref)) {
    $in = implode(',', array_keys($ref));
    $query = "SELECT node2.nid, node2.vid, node2.title
              FROM   {node} node
              INNER JOIN {node} node2 on node2.tnid = node.tnid
              WHERE node.nid IN ($in)
                    AND
                    node.tnid > 0
    ";
    $query = db_rewrite_sql($query, 'node');
    $result = db_query($query);
    while ($value = db_fetch_array($result)) {
      $ref[$value['nid']] = $value;
    }
  }

  return $ref;
}

