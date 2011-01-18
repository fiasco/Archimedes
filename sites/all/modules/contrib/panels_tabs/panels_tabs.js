// $Id: panels_tabs.js,v 1.1.2.5.2.4 2010/09/10 12:25:34 smoothify Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function(){ 

    // This function updates the links of pages, calendar nav, and calendar days
    // to contain the currently selected fragment. This allows you to click a
    // link of a pager in a panel, and when the page reloads, the correct tab will
    // be opened.
    updateLinks = function(id, fragment) {
      $('#tabs-'+ id +' .pager a, #tabs-'+ id +' .date-nav a, #tabs-'+ id +' .day a, ').each(function() {

        var oldURI = $(this).attr('href');
        if (oldURI.lastIndexOf('#') > 0) {
          oldURI = oldURI.substr(0, oldURI.lastIndexOf('#'));
        }
        var newURI = oldURI + fragment;
        $(this).attr('href', newURI);
      });
    };    

    // Resize tabs as configured.
    for (var id in Drupal.settings.panelsTabs) {
      var $tabs = $('#tabs-'+ id +' > ul > li');

      switch (Drupal.settings.panelsTabs[id].fillingTabs) {
        case 'equal':
          $tabs.each(function() {
            $(this)
            .css('width', (99 / $tabs.length) +'%')
            .css('overflow', 'hidden');
          });
          break;

        case 'smart':
          var tabTextLengths = new Array();
          var totalTabTextLength = 0;

          $tabs
          // Collect the length of the text on each tab and the total length.
          .each(function(i) {
            totalTabTextLength += tabTextLengths[i] = $(this).text().length;
          })
          // Use the collected text lenghts to do smart tab width scaling.
          .each(function(i) {
            $(this)
            .css('width', (tabTextLengths[i] / totalTabTextLength * 100) +'%')
            .css('overflow', 'hidden');
          });
          break;

        case 'disabled':
        default:
          // Nothing should happen if we 
          break;
      }

      // Update links when a tab is clicked.
      $('#tabs-'+ id +' > ul > li > a').each(function() {
        $(this).click(function(_id, fragment) {
          return function() { updateLinks(_id, fragment); };
        }(id, $(this).attr('href')));
      });
    }

    // Update all links on load (automatically find the name of the tabset!).
    var currentURI = "" + window.location;
    var poundIdx = currentURI.indexOf('#');
    if (poundIdx > -1) { // Ensure a fragment is set.
      var fragment = currentURI.substr(poundIdx);
      var tabsetDiv = $('a[href*="'+ fragment +'"]')
      .parent() // the parent li
      .parent() // the parent ul
      .parent(); // the parent div (the actual tabset div!)
      if (tabsetDiv.length > 0) { // Ensure the set fragment is associated with a tabset div.
        var id = tabsetDiv.attr('id').substr("tabs-".length);
        updateLinks(id, fragment);
      }
    }
  });
}
