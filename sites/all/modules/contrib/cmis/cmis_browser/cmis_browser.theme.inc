<?php 
// $Id: cmis_browser.theme.inc,v 1.1.2.2.2.11 2010/03/08 20:30:24 cbalan Exp $

function theme_cmis_browser($context) {
  // Render output
  drupal_add_css(drupal_get_path('module', 'cmis_browser') .'/css/cmis_browser.css');
  
  $contents = drupal_get_form('cmis_browser_browse_form');
  $contents .= drupal_get_form('cmis_browser_actions_form');
  $contents .= theme('cmis_browser_browse_breadcrumb', $context['bcarray']);
  $contents .= theme('cmis_browser_browse_children', $context);
  
  return $contents; 
}

/**
 * Custom theme for cmis_browser form.
 * 
 * @param $form
 */
function theme_cmis_browser_browse_form($form) {
  $header = array('', '');
  $rows = array(
    array(drupal_render($form['browse']['path']), drupal_render($form['browse']['submit'])),
  );
  
  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Custom theme for cmis_browser_browse action
 * 
 * @param $children
 */
function theme_cmis_browser_browse_children($context = array()) {
  $header = array(t('name'), t('type'), t('size'), t('author'), t('last modified'), '');
  $rows = array();
  $folder_img = theme('image', drupal_get_path('module', 'cmis_browser') .'/images/space.gif');
  $document_img = theme('image', drupal_get_path('module', 'cmis_browser') .'/images/file.png');

  foreach ($context['children'] as $child) {
    $author = $child->properties['cmis:createdBy'];
    $updated = date_format(date_create($child->properties['cmis:lastModificationDate']), 'n/j/Y g:i A');
    $actions = array( 
      l(t('properties'), 'cmis/properties', array('attributes' => array('class' => 'action properties'), 'query' => array('id' => $child->id))),
      l(t('delete'), 'cmis/delete', array('query' => array('id' => $child->id, 'return_url' => $_GET['q'])))
    );
    
    switch ($child->properties['cmis:baseTypeId']) {
      case 'cmis:folder':
        $icon = $folder_img;
        $link = l($child->properties['cmis:name'], 'cmis/browser'. $child->properties['cmis:path']);
        $mimetype = 'Space';
        $size = '';
        break;
      default:      
        $icon = $document_img;
        $link = l($child->properties['cmis:name'], 'cmis/browser', array('query' => array('id' => $child->id)));
        $mimetype = $child->properties['cmis:contentStreamMimeType'];
        $size = number_format($child->properties['cmis:contentStreamLength'] / 1000, 2, '.', ',') .' K';
    }
    
    $rows[] = array($icon .' '. $link, $mimetype, $size, $author, $updated, theme('item_list', $actions, NULL, 'ul', array('class'=>'actions')));
  }
  
  drupal_add_js('
    $(document).ready(function() {
      $("A.action.properties").each(function() {
        $(this).click(function() {
          $(this).parents("LI:first").toggleClass("expanded").toggleClass("collapsed");
          if ($(this).parents("TR:first").next().filter("TR.details").toggle().length == 0) {
            $("<td colspan=\"6\"><span class=\"load_indicator\">'. t('Loading'). '...</span></td>")
              .load(this.href+"&no_layout")
              .insertAfter($(this).parents("TR:first"))
              .wrapAll("<tr class=\"details\"></tr>")
              .before("<td></td>");
          }
          return false;
        }).parents("LI:first").toggleClass("collapsed");
      });
    });', 'inline');

  return theme('table', $header, $rows, array('class' => 'cmis_browser_browse_children')); 
}

/**
 * Theme for cmis_browser breadcrumb
 * 
 * @param $bcarray
 */
function theme_cmis_browser_browse_breadcrumb($bcarray) {
  $next_img = theme('image', drupal_get_path('module', 'cmis_browser') .'/images/next.gif');
  
  $contents .= '<div id="cmis-breadcrumb">';
  $contents .= l('Root', 'cmis/browser'. $currentpath);
  
  $currentpath = '';
  foreach ($bcarray as $space) {
    $contents .= $next_img .' ';
    $currentpath .= '/'. $space;
    $pagelink = l($space, 'cmis/browser'. $currentpath);
    $contents .= $pagelink;    
  }

  $contents .= '</div>';

  return $contents;
}

/**
 * Theme for cmis_browser_content_properties action.
 * 
 * @param $cmis_object
 */
function theme_cmis_browser_content_properties($cmis_object) {
  $output = theme('box', $cmis_object->properties['cmis:name'], $cmis_object->properties['cmis:summary']);
  
  $header = array(t('Property'), t('Value'));
  $rows = array();
  foreach ($cmis_object->properties as $property => $value) {
    $rows[] = array('<b>'. $property.'</b>', $value);
  }  

  return $output . theme('table', NULL, $rows);
}

/**
 * Theme for cmis_browser folder picker widget.
 *
 * @param $textfield_element
 */
function theme_cmis_browser_folder_picker($textfield_element) {
  drupal_add_css(drupal_get_path('module', 'cmis_browser') .'/css/cmis_browser.css');
  drupal_add_js(drupal_get_path('module', 'cmis_browser') .'/js/jquery.tree.min.js');

  drupal_add_js('
  $(document).ready(function(){
    $("#'. $textfield_element['#id'] .'-cmis-picker")
      .find("div.form-item").css("display","inline").end()
      .find("a:first").click(function() {
        $(".tree",$(this).parent()).toggle();
      }).end()
      .find(".tree").tree({
        callback: {
          onselect: function(node, tree) {
            var text_element = $("#'. $textfield_element['#id'] .'");
            text_element.attr("value", "/");
            ($(node).attr("rel")=="folder"?$(node).children():$(node))
              .parents("li").map(function(el){
                text_element.attr("value",
                  "/" + $.trim($("a:first", $(this)).text()) + text_element.attr("value"));
              }); tree.container.toggle();
          }
        },
        ui:{dots:false},
        data:{
          type: "json", async:true, opts: {url: "'. url('cmis/tree') .'"}
        },
        types:{
          "default": {draggable: false, deletable: false, renameable: false},
          "folder": {
            valid_children : [ "document" ],
            icon: {
              image: "'. url(drupal_get_path('module', 'cmis_browser')) .'/images/space.gif"
            }
          },
          "document": {
            valid_children: "none", max_children: 0, max_depth: 0,
            icon: {
              image: "'. url(drupal_get_path('module', 'cmis_browser')) .'/images/file.png"
            },
          }
        }
      }).css("width", $("#'. $textfield_element['#id'] .'").attr("offsetWidth").toString()+"px")
  });', 'inline');
  
  return '<div id="'. $textfield_element['#id'] .'-cmis-picker" style="display:block">'
    .theme('textfield', $textfield_element)
    .'<a href="#"> Tree </a>'
    .'<div class="tree" style="display:none"></div>'
  .'</div>';
}
