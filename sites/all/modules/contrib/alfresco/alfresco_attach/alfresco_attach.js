// $Id: alfresco_attach.js,v 1.1.2.1 2010/04/26 12:33:02 xergius Exp $

Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.alfresco_attachments = function() {
  var size = $('#alfresco-attachments tbody tr').size();
  if (size) {
    return Drupal.formatPlural(size, '1 attachment', '@count attachments');
  }
  else {
    return Drupal.t('No attachments');
  }
}
