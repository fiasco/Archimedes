// $Id: alfresco_browser.js,v 1.1.2.1 2010/04/26 12:33:03 xergius Exp $

Drupal.behaviors.attachBrowser = function (context) {
  var popup = null;
  var $browseButton = $("#edit-alfresco-browser-button"); 
  
  $(document).ready(function() {
    $browseButton.click(function() {
      if (popup == null || popup.closed) {
        popup = window.open(
          Drupal.settings.alfrescoBrowserUrl,
          Drupal.settings.alfrescoBrowserName,
          Drupal.settings.alfrescoBrowserFeatures);
      }
      if (popup) {
        popup.focus();
      }
      return false;
    });
  });
};
