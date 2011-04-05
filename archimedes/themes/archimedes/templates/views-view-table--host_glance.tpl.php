<?php
  foreach ($rows[0] as $key => $value) {
    $rows_d[] = array(array('header' => 1, 'data' => $header[$key]), $value);
  }
  print theme('table',null,$rows_d);
?>
