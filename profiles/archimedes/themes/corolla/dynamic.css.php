/* Base font size */
html {
  font-size: <?php print theme_get_setting('base_font_size'); ?>;
}

/* Layout width */
body.two-sidebars #header-menu,
body.two-sidebars #header,
body.two-sidebars #main-columns {
  width: <?php print theme_get_setting('layout_3_width'); ?>;
  min-width:<?php print theme_get_setting('layout_3_min_width'); ?>;
  max-width: <?php print theme_get_setting('layout_3_max_width'); ?>;
}
body.one-sidebar #header-menu,
body.one-sidebar #header,
body.one-sidebar #main-columns {
  width: <?php print theme_get_setting('layout_2_width'); ?>;
  min-width: <?php print theme_get_setting('layout_2_min_width'); ?>;
  max-width: <?php print theme_get_setting('layout_2_max_width'); ?>;
}
body.no-sidebars #header-menu,
body.no-sidebars #header,
body.no-sidebars #main-columns {
  width: <?php print theme_get_setting('layout_1_width'); ?>;
  min-width: <?php print theme_get_setting('layout_1_min_width'); ?>;
  max-width: <?php print theme_get_setting('layout_1_max_width'); ?>;
}

/**
 * Color schemes
 */
<?php

$color = theme_get_setting('color_scheme');

$colors = array(
  'default' => array(
    'base' => '#ffffff',
    'background' => '#f8f8f8',
    'text' => '#2e2e2e',
    'link' => '#086782',
    'linkhover' => '#e25401',
    'linkunderline' => '#cfdde5',
    'slogan' => '#e25400',
    'headermenu' => '#2e2e2d',
    'headermenuhover' => '#e25402',
    'tab' => '#f5f4f3',
    'blocktitle' => '#779125',
    'border' => '#e1e1e1',
    'borderstrong' => '#c4c4c4',
    'fieldset' => '#fbfbfb',
    'fieldsetborder' => '#e1e1e2',
  ),
  'green1' => array(
    'base' => '#ffffff',
    'background' => '#fbfbf6',
    'text' => '#3d3d3d',
    'link' => '#899833',
    'linkhover' => '#e14601',
    'linkunderline' => '#e3eac1',
    'slogan' => '#899833',
    'headermenu' => '#8e9e35',
    'headermenuhover' => '#626c25',
    'tab' => '#fbfbf6',
    'blocktitle' => '#738b25',
    'border' => '#e7e1ce',
    'borderstrong' => '#d2c8aa',
    'fieldset' => '#fdfdfb',
    'fieldsetborder' => '#dad6c9',
  ),
  'green2' => array(
    'base' => '#ffffff',
    'background' => '#fbfbf6',
    'text' => '#3d3d3d',
    'link' => '#157a9c',
    'linkhover' => '#e14601',
    'linkunderline' => '#cfdde5',
    'slogan' => '#e14601',
    'headermenu' => '#8e9e35',
    'headermenuhover' => '#626c25',
    'tab' => '#fbfbf6',
    'blocktitle' => '#779125',
    'border' => '#e7e1ce',
    'borderstrong' => '#d2c8aa',
    'fieldset' => '#fdfdfb',
    'fieldsetborder' => '#dad6c9',
  ),
  'purple' => array(
    'base' => '#ffffff',
    'background' => '#fefafb',
    'text' => '#2e2e2e',
    'link' => '#6c0d28',
    'linkhover' => '#e25401',
    'linkunderline' => '#eac8d1',
    'slogan' => '#e25401',
    'headermenu' => '#6c0d28',
    'headermenuhover' => '#83a80e',
    'tab' => '#fbf3f6',
    'blocktitle' => '#e25401',
    'border' => '#f7d6e2',
    'borderstrong' => '#d9a3b7',
    'fieldset' => '#fefafb',
    'fieldsetborder' => '#f7d6e2',
  ),
  'red' => array(
    'base' => '#ffffff',
    'background' => '#fefbfa',
    'text' => '#2e2e2e',
    'link' => '#b9400e',
    'linkhover' => '#ef762f',
    'linkunderline' => '#faded3',
    'slogan' => '#ca592b',
    'headermenu' => '#b9400e',
    'headermenuhover' => '#7e2c0a',
    'tab' => '#fdf6f4',
    'blocktitle' => '#b9400e',
    'border' => '#e8dad5',
    'borderstrong' => '#e2bdae',
    'fieldset' => '#fefbfa',
    'fieldsetborder' => '#f7dad6',
  ),
  'yellow' => array(
    'base' => '#ffffff',
    'background' => '#f9faef',
    'text' => '#383838',
    'link' => '#8d8017',
    'linkhover' => '#de4c01',
    'linkunderline' => '#f4eebc',
    'slogan' => '#afa02f',
    'headermenu' => '#a0a465',
    'headermenuhover' => '#6c6f42',
    'tab' => '#f7f8ec',
    'blocktitle' => '#afa02f',
    'border' => '#e2e5c3',
    'borderstrong' => '#dcd093',
    'fieldset' => '#fbfbf8',
    'fieldsetborder' => '#ddd7b6',
  ),
);
?>

/* Background */
body {
  background-color: <?php print $colors[$color]['background']; ?>;
}
#header-menu .content > ul > li.active-trail {
  background-color: <?php print $colors[$color]['background']; ?>;
}
/* Text (black) */
body,
#site-name a,
#header-menu .content li.active a,
h1.page-title,
.node h2.node-title,
.node h1.node-title a,
.node h2.node-title a,
.comment h3.title a,
.tabs ul.tabs li a,
pre,
code,
samp,
var,
table.update tr,
table.system-status-report tr {
  color: <?php print $colors[$color]['text']; ?>;
}
#site-name a::-moz-selection {
  background-color: <?php print $colors[$color]['text']; ?>;
}
#site-name a::selection {
  background-color: <?php print $colors[$color]['text']; ?>;
}
.node-title a::-moz-selection {
  background-color: <?php print $colors[$color]['text']; ?>;
}
.node-title a::selection {
  background-color: <?php print $colors[$color]['text']; ?>;
}
/* Link (blue) */
a,
a.active,
li a.active {
  color: <?php print $colors[$color]['link']; ?>;
}
legend {
  background-color: <?php print $colors[$color]['link']; ?>;
}
/* Link hovered (orange) */
a:hover,
a:focus,
a.active:hover,
a.active:focus,
li a.active:hover,
li a.active:focus {
  color: <?php print $colors[$color]['linkhover']; ?>;
  border-bottom-color: <?php print $colors[$color]['linkhover']; ?>;
}
.node h1.node-title a:hover,
.node h2.node-title a:hover {
  color: <?php print $colors[$color]['linkhover']; ?>;
}
/* Link underline (light blue) */
a {
  border-bottom: 1px solid <?php print $colors[$color]['linkunderline']; ?>;
}
/* Header menu (black) */
#header-menu-wrapper {
  background-color: <?php print $colors[$color]['headermenu']; ?>;
}
/* Header menu hovered (orange) */
#header-menu .content li a:hover,
#header-menu .content li a:active,
#header-menu .content li a:focus {
  background-color: <?php print $colors[$color]['headermenuhover']; ?>;
}
/* Slogan (orange) */
#site-slogan {
  color: <?php print $colors[$color]['slogan']; ?>;
}
#site-slogan::selection {
  background-color: <?php print $colors[$color]['headermenuhover']; ?>;
}
#site-slogan::-moz-selection {
  background-color: <?php print $colors[$color]['headermenuhover']; ?>;
}
.poll .bar .foreground {
  background-color: <?php print $colors[$color]['headermenuhover']; ?>;
}
/* Border (gray) */
#page {
  border: 1px solid <?php print $colors[$color]['border']; ?>;
}
/* Border strong (dark gray) */
#sidebar-first input,
#sidebar-second input {
  border: 1px solid <?php print $colors[$color]['borderstrong']; ?>;
}
.tabs ul.tabs li a {
  border: 1px solid <?php print $colors[$color]['borderstrong']; ?>;
}
/* Tab */
.tabs ul.tabs li a {
  background-color: <?php print $colors[$color]['tab']; ?>;
}
ul.vertical-tabs-list li a {
  background-color: <?php print $colors[$color]['tab']; ?>;
}
/* Block title (green) */
.block h2,
h2 {
  color: <?php print $colors[$color]['blocktitle']; ?>;
}
.block h2::selection {
  color: #fff;
  background-color: <?php print $colors[$color]['blocktitle']; ?>;
}
.block h2::-moz-selection {
  background-color: <?php print $colors[$color]['blocktitle']; ?>;
  color: #fff;
}
/* Fieldset (gray) */
fieldset {
  background-color: <?php print $colors[$color]['fieldset']; ?>;
}
/* Fieldset border (dark gray) */
fieldset {
  border: 1px solid <?php print $colors[$color]['fieldsetborder']; ?>;
}

