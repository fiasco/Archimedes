<?php
  module_load_include('inc', 'archimedes_drupal_site', 'archimedes_drupal_site.update');
  $vocab = taxonomy_get_vocabularies($type = NULL);
  $recommended = array();
  foreach ($vocab as $v) {
    if ($v->name == 'Drupal Major Version') {
       $vid = $v->vid;
    }
  }
  if ($vid) {
    $proj = $row->node_data_field_dru_proj_field_dru_proj_value;
    $terms = taxonomy_get_tree($vid);
    foreach ($terms as $version) {
      if ($data = cache_get($proj . $version->name, 'cache_archimedes_drupal')) {
      $recom = $data->data['recommended_major'];
        foreach($data->data['releases'] as $release) {
          if ($release['version_major'] == $recom) {
            $recommended[] = $release['version'];
            break;
          }
        }
      }
    }
  }
  if (!empty($recommended)) {
    $output = implode(', ', $recommended);  
  }
  else {
    $output = t('Could not accurately determine module version status.');
  }
?>
<?php print $output; ?>
