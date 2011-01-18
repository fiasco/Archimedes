<?php
// $Id: alfresco-browser-page.tpl.php,v 1.1.2.1 2010/04/26 12:33:03 xergius Exp $

/**
 * @file alfresco-browser-page.tpl.php
 * Default theme implementation for Alfresco Browser page.
 *
 * @see template_preprocess_alfresco_browser_page()
 */
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <style type="text/css">
  #loading-mask {position:absolute;left:0;top:0;width:100%;height:100%;z-index:30000;background-color:white}
  #loading {position:absolute;left:45%;top:40%;padding:2px;z-index:30001;height:auto}
  #loading a {color:#258}
  #loading .loading-indicator {background:white;color:#444;font:bold 13px tahoma,arial,helvetica;padding:10px;margin:0;height:auto}
  #loading .loading-indicator img {float:left;margin-right:8px;vertical-align:top}
  #loading-msg {font:normal 10px arial,tahoma,sans-serif}
  </style>
</head>
<body>
<div id="loading-mask"></div>
<div id="loading">
  <div class="loading-indicator">
    <?php print $loading_img; ?><?php print $loading_msg; ?>
    <br /><span id="loading-msg"><?php print t('Initializing...'); ?></span>
  </div>
</div>
<div id="header">
<h1><?php print $header; ?></h1>
<div id="search-box" class="x-normal-editor"><input type="text" id="search" /></div>
</div>
<?php print $closure; ?>
</body>
</html>
