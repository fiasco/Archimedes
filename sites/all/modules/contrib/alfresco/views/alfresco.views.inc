<?php
// $Id: alfresco.views.inc,v 1.1.2.1 2010/04/26 12:33:02 xergius Exp $

/**
 * @file
 *
 * Provide views data and handlers for alfresco module.
 *
 * @see http://views-help.doc.logrus.com/help/views/api
 */

/**
 * Implementation of hook_views_data()
 */
function alfresco_views_data() {
  $data = array();

  // ----------------------------------------------------------------------
  // alfresco_node table

  $data['alfresco_node']['table']['group'] = t('Alfresco');
  $data['alfresco_node']['table']['join'] = array(
    'node' => array(
      // direct join to the node table via 'vid' field.
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revisions' => array(
      // direct join to the node_revisions table via 'vid' field.
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // reference
  $data['alfresco_node']['reference'] = array(
    'title' => t('Reference'),
    'help' => t('The Alfresco node reference.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // timestamp field
  /*
  $data['alfresco_node']['timestamp'] = array(
    'title' => t('Upload date'),
    'help' => t('The date the file was saved.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  */

  // cm_name (filename)
  $data['alfresco_node']['cm_name'] = array(
    'title' => t('Name'),
    'help' => t('The name of the item in the Alfresco repository.'),
    'field' => array(
      'handler' => 'alfresco_handler_field_url_download',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // cm_title
  $data['alfresco_node']['cm_title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the item in the Alfresco repository.'),
    'field' => array(
      'handler' => 'alfresco_handler_field_url_download',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // cm_description
  $data['alfresco_node']['cm_description'] = array(
    'title' => t('Description'),
    'help' => t('The description of the item in the Alfresco repository.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // cm_author
  $data['alfresco_node']['cm_author'] = array(
    'title' => t('Author'),
    'help' => t('The author of the item in the Alfresco repository.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // cm_author
  $data['alfresco_node']['cm_author'] = array(
    'title' => t('Author'),
    'help' => t('The author of the item in the repository.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // cm_created field
  $data['alfresco_node']['cm_created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the item was created in the repository.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // ----------------------------------------------------------------------
  // alfresco_content table

  $data['alfresco_content']['table']['group'] = t('Alfresco');
  $data['alfresco_content']['table']['join'] = array(
    'node' => array(
      'left_table' => 'alfresco_node',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revisions' => array(
      'left_table' => 'alfresco_node',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // url
  $data['alfresco_content']['url'] = array(
    'title' => t('URL'),
    'help' => t('The Guest URL of the alfresco file.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // mimetype
  $data['alfresco_content']['mimetype'] = array(
    'title' => t('Mime type'),
    'help' => t('The mime type of the alfresco file.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // size
  $data['alfresco_content']['size'] = array(
    'title' => t('Size'),
    'help' => t('The size of the alfresco file.'),
    'field' => array(
      'handler' => 'views_handler_field_file_size',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  // ----------------------------------------------------------------
  // node_counter table

  $data['alfresco_counter']['table']['group']  = t('Alfresco statistics');

  $data['alfresco_counter']['table']['join'] = array(
    // direct join to the node table via 'nid' field.
    'node' => array(
      'left_table' => 'alfresco_node',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // totalcount
  $data['alfresco_counter']['totalcount'] = array(
    'title' => t('Total downloads'),
    'help' => t('The total number of times the node has been downloaded.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // daycount
  $data['alfresco_counter']['daycount'] = array(
    'title' => t('Downloads today'),
    'help' => t('The total number of times the node has been downloaded today.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // timestamp
  $data['alfresco_counter']['timestamp'] = array(
    'title' => t('Most recent download'),
    'help' => t('The most recent time the node has been downloaded.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 * Register all of the basic handlers views uses.
 */
function alfresco_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'alfresco') . '/views',
    ),
    'handlers' => array(
      'alfresco_handler_field_url_download' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
