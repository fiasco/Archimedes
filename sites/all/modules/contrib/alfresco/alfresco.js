// $Id: alfresco.js,v 1.2.2.2 2010/04/26 12:33:02 xergius Exp $

Drupal.behaviors.alfrescoSettings = function (context) {
  var random = String((new Date()).getTime()).replace(/\D/gi,'');
  var passwordElem = $('#edit-alfresco-credentials-password');
  if (!passwordElem.val()) {
    passwordElem.val(random);
  }
  passwordElem.focus(function() {
    if($.trim(passwordElem.val()) == random) {
      passwordElem.val('');
    }
  });
  passwordElem.blur(function() {
    if($.trim(passwordElem.val()) == '') {
      passwordElem.val(random);
    }
  });
  $('#edit-submit').click(function() {
    if ($.trim(passwordElem.val()) == random) {
      passwordElem.val('');
    }
  });
}
