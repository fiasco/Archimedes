// $Id: tabs.js,v 1.12 2010/01/18 08:11:21 nedjo Exp $

Drupal.tabs = Drupal.tabs || {};

Drupal.behaviors.tabs = function (context) {
  // Set the active class to the first tab with an form error.
  $('.drupal-tabs').children('ul > li').each( function() {
    if ($($(this).find('a').attr('href')).find('div.form-item .error:first').size()) {
      $(this).addClass('error').addClass('ui-tabs-selected');
    }
  });

  var fx = {duration: Drupal.settings.tabs.speed};
  if (Drupal.settings.tabs.fade) {
    fx.opacity = 'toggle';
  }
  if (Drupal.settings.tabs.slide) {
    fx.height = 'toggle';
  }
  // Process custom tabs.
  var selected = null;
  $('.drupal-tabs:not(.tabs-processed)', context)
    .find('> ul')
    .tabs({
      spinner: Drupal.t('Loading...'),
      // Add the 'active' class when showing tabs and remove it from siblings.
      show: function(event, ui) {
        $(ui.tab).parent('li').addClass('active').siblings('li').removeClass('active');
      },
      fx: fx
    })
    .addClass('tabs')
    .each(function () {
      // Assign secondary class to nested tabsets.
      var newClass = $(this).parents('.drupal-tabs').size() > 1 ? 'secondary' : 'primary';
      $(this)
        .addClass(newClass)
        .find('>li:first')
        .addClass('first')
        .end()
        .find('>li:last')
        .addClass('last');
    })
    .after('<span class="clear"></span>')
    .end()
    .addClass('tabs-processed')
    .each(function () {
      if ($(this).is('.tabs-navigation')) {
        Drupal.tabs.tabsNavigation(this);
      }
    })
    .show();
};

Drupal.tabs.tabsNavigation = function(elt) {
  // Extract tabset name.
  var tabsetName = $(elt).get(0).id.substring(5);
  var $tabs = $('> ul', elt);
  var i = 1;
  var $tabsContent = $('div.tabs-' + tabsetName, elt);
  var count = $tabsContent.size();
  $tabsContent.each(function() {
    if ((i > 1) || (i < count)) {
      $(this).append('<span class="clear"></span><div class="tabs-nav-link-sep"></div>');
    }
    if (i > 1) {
      var previousText = '‹ ' + (Drupal.settings.tabs.navigation_titles ? $tabs.find('> li:eq(' + parseInt(i - 2) + ')').text() : Drupal.settings.tabs.previous_text);
      var link = $(document.createElement('a'))
        .append('<span>' + previousText + '</span>')
        .attr('id', 'tabs-' + tabsetName + '-previous-link-' + i)
        .addClass('tabs-nav-previous')
        .click(function() {
          var tabIndex = parseInt($(this).attr('id').substring($(this).attr('id').lastIndexOf('-') + 1)) -1;
          $tabs.tabs('select', tabIndex - 1);
          Drupal.tabs.scrollTo(elt);
          return false;
        });
      $(this).append(link);
    }
    if (i < count) {
      var nextText = (Drupal.settings.tabs.navigation_titles ? $tabs.find('> li:eq(' + parseInt(i) + ')').text() : Drupal.settings.tabs.next_text) + ' ›';
      var link = $(document.createElement('a'))
        .append('<span>' + nextText + '</span>')
        .attr('id', 'tabs-' + tabsetName + '-next-button-' + i)
        .addClass('tabs-nav-next')
        .click(function() {
          var tabIndex = parseInt($(this).attr('id').substring($(this).attr('id').lastIndexOf('-') + 1)) -1;
          $tabs.tabs('select', tabIndex + 1);
          Drupal.tabs.scrollTo(elt);
          return false;
        });
      $(this).append(link);
    }
    $tabsContent.append('<span class="clear"></span>');
    i++;
  });
};

Drupal.tabs.scrollTo = function(elt) {
  // Scroll to the top of the tab.
  var offset = $(elt).offset();
  // Only scroll upwards.
  if (offset.top - 10 < $(window).scrollTop()) {
    $('html,body').animate({scrollTop: (offset.top - 10)}, 500);
  }
};

