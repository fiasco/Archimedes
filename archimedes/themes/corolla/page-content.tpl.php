<?php
// $Id: page-content.tpl.php,v 1.4.2.3 2011/01/01 13:24:54 jarek Exp $

/**
 * @file page.tpl.php
 *
 * Theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $css: An array of CSS files for the current page.
 * - $directory: The directory the theme is located in, e.g. themes/garland or
 *   themes/garland/minelli.
 * - $is_front: TRUE if the current page is the front page. Used to toggle the mission statement.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Page metadata:
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $body_classes: A set of CSS classes for the BODY tag. This contains flags
 *   indicating the current layout (multiple columns, single column), the current
 *   path, whether the user is logged in, and so on.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $mission: The text of the site mission, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $search_box: HTML to display the search box, empty if search has been disabled.
 * - $primary_links (array): An array containing primary navigation links for the
 *   site, if they have been configured.
 * - $secondary_links (array): An array containing secondary navigation links for
 *   the site, if they have been configured.
 *
 * Page content (in order of occurrance in the default page.tpl.php):
 * - $left: The HTML for the left sidebar.
 *
 * - $breadcrumb: The breadcrumb trail for the current page.
 * - $title: The page title, for use in the actual HTML content.
 * - $help: Dynamic help text, mostly for admin pages.
 * - $messages: HTML for status and error messages. Should be displayed prominently.
 * - $tabs: Tabs linking to any sub-pages beneath the current page (e.g., the view
 *   and edit tabs when displaying a node).
 *
 * - $content: The main content of the current Drupal page.
 *
 * - $right: The HTML for the right sidebar.
 *
 * Footer/closing data:
 * - $feed_icons: A string of all feed icons for the current page.
 * - $footer_message: The footer message as defined in the admin settings.
 * - $footer : The footer region.
 * - $closure: Final closing markup from any modules that have altered the page.
 *   This variable should always be output last, after all other dynamic content.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 */
?>
<?php $column++; ?>
<div id="main-wrapper" class="<?php if ($column == 1): ?> first<?php endif; ?><?php if ($column == $main_columns_number): ?> last<?php endif; ?>">
  <div id="main">
    <div id="page" class="clear-block">
      <?php if (!empty($breadcrumb)): ?><div id="breadcrumb"><?php print $breadcrumb; ?></div><?php endif; ?>
      <?php if (!empty($messages)): print $messages; endif; ?>

      <?php if (!empty($content_top)): ?>
        <div class="region region-content-top">
          <?php print $content_top; ?>
        </div>
      <?php endif; ?>

      <?php if (!empty($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>

      <?php if (!empty($highlight)): ?>
        <div class="region region-highlight">
          <?php print $highlight; ?>
        </div>
      <?php endif; ?>

      <a id="main-content"></a>
      <?php if (!empty($title)): ?>
        <h1 class="page-title"><?php print $title ?></h1>
      <?php endif; ?>

      <?php if (!empty($help)): print $help; endif; ?>

      <?php if (!empty($content)): ?>
        <div class="region region-content">
          <?php print $content; ?>
        </div>
      <?php endif; ?>

      <?php if (!empty($content_bottom)): ?>
        <div class="region region-content-bottom">
          <?php print $content_bottom; ?>
        </div>
      <?php endif; ?>
      <?php print $feed_icons; ?>
    </div> <!-- /#page -->


    <div id="closure">
      <div id="info">
        <?php print $footer_message; ?> Drupal theme by <a href="http://kiwi-themes.com">Kiwi Themes</a>.
      </div>

      <?php if ($footer_menu): ?>
        <div id="footer-menu">
          <?php print $footer_menu; ?>
        </div>
      <?php endif; ?>
    </div> <!-- /#closure -->

  </div> <!-- /#main -->
</div> <!-- /#main-wrapper -->
