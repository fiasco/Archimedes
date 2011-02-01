<?php
  module_load_include('inc', 'archimedes_drupal_site', 'archimedes_drupal_site.update');
  if ($output) {
    $major_version = substr($output,0,1);
    if ($data = cache_get('drupal' . $major_version . '.x', 'cache_arch_drupal')) {
      $latest = reset($data->data['releases']);
      $link = $latest['download_link'];
      $latest = $latest['version'];
      if($output == $latest){
        $output = '<span class="tick">' . $output . '</span>';
      }
      else{
       $output = '<span class="old">' . $output . '</span><span class="right">' . l("Download Latest", $link) . '</span>';
      }
    }
    else {
      $output = t('Could not accurately determine Drupal version.');
    }
  }
?>
<?php print $output; ?>