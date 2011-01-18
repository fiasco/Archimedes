/* $Id: README.txt,v 1.1.2.4.2.6 2010/08/03 09:43:17 rsevero Exp $ */

-- SUMMARY --

Charts and Graphs is a API for developers. It can easily be extended by
third-party modules that want to add their own charting implementations. It does
nothing by itself. It should only be installed if some other module requires it.

See Views Charts <http://drupal.org/project/views_charts> for usage of a
user-centric application as the Charts and Graphs module is a framework +
collection of implementation modules (under the api folder). Please read
README/INSTALL files of the implementations since their installation may involve
downloading flash components or third-party libraries. Drupal guidelines does
not always allow packaging those with Drupal code.

For a full description of the module, visit the project page:
  http://drupal.org/project/charts_graphs

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/charts_graphs


-- REQUIREMENTS --

None


-- SUPPORT CHARTING LIBRARIES --

* amCharts <http://www.amcharts.com/> - Commercial / Free with an ad link

* Bluff <http://bluff.jcoglan.com/> - Beautiful Graphics in Javascript - MIT and
  GPL license

* Google Charts <http://code.google.com/apis/charttools/index.html> - Is free to
  use. Google ask you to contact them if you plan on producing more than 250,000
  API calls per day, see Google Chart Usage Policy at
  <http://code.google.com/apis/chart/docs/making_charts.html>.

* Open Flash Chart 2 <http://teethgrinder.co.uk/open-flash-chart-2/> - LGPL
  license


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* There is additional information available at <http://drupal.org/node/681660>.
  Bluff and Google Charts can be used right away. amCharts and Open Fharts Flash
  2 require additional downloads/configuration to work.


-- UPGRADE --

* On version 6.x-2.0-RC2 a settings page was introduced in Charts and Graphs. To
  get access to it you should uninstall Charts and Graphs and reinstall it when
  upgrading from a pre 6.x-2.0-RC2 version to 6.x-2.0-RC2 or later.

* On version 6.x-2.x the main "chart_graphs_get_graph" function has been
  renamed to "charts_graphs_get_graph" so it has the correct prefix. Fix your
  code.

* To upgrade from any 6.x-1.x version to 6.x-2.x first disable all Charts and
  Graphs module and submodules (charting libraries).

* Then move all files related to Charts and Graphs to a temporary directory:
  mv sites/all/modules/charts_graphs /tmp

* Untar the new release at the modules directory.

* Copy any downloaded file for some specific charting library from the temporary
  directory to the new module.

* Remove the old module:
 rm -rf /tmp/charts_graphs


-- CONFIGURATION --

* The configuration options are available at admin/settings/charts_graphs. They
  deal only with the appearance or not of a few warnings at the "Status reports"
  page.


-- TROUBLESHOOTING --

Empty.


-- FAQ --

Q: Empty

A: Empty.
