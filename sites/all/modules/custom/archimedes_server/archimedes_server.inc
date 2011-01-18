<?php

/**
 * Parse xml values through any defined plugins
 */
function archimedes_server_run_plugins($plugins, $field, $value) {
  $module_plugins = archimedes_server_get_plugins();
  $nValue = strtr($value->nodeValue, array(
    'file://' => '',
    'mailto:' => '',
  ));
  $return = array();
  switch($field['module']) {
    case 'link':
      $return['url'] = $nValue;
      break;
    default:
      $return['value'] = $nValue;
  }
  foreach ($plugins as $ns => $plugin) {
    $result = archimedes_server_plugin_invoke($module_plugins[$plugin], $plugin, $value, $ns);
    $return = array_merge($return, $result);
  }
  return $return;
}

function archimedes_server_plugin_invoke($module, $plugin, $value, $ns) {
  module_load_include('inc', $module, $module . '.plugins');
  $class = 'ArchimedesServerPlugin_' . str_replace('-', '_', $plugin);
  if (class_exists($class)) {
    $parser = new $class($ns);
    return $parser->parse($value);
  }
  return array();
}

/**
 * Get (and generate if required) list of plugins from all modules
 */
function archimedes_server_get_plugins() {
  static $plugins;
  if (!empty($plugins)) {
    return $plugins;
  }
  foreach (module_implements('archimedes_server_plugins') as $module) {
    foreach (module_invoke($module, 'archimedes_server_plugins') as $plugin) {
      $plugins[$plugin] = $module;
    }
  }
  return $plugins;
}

/**
 * Utility function to return uid from mail address and create user if required.
 */
function archimedes_server_user_save($mail) {
  $uid = '';
  if (substr($mail,0,7) == 'mailto:') {
    $mail = substr($mail,7);
  }
  if($user = user_load(array('mail' => $mail))) {
    $uid = $user->uid;
  } else {

    $user_data = array(
      'mail' => $mail,
      'name' => $mail,
      'init' => $mail,
      'status' => 0,
    );

    // If LDAP Integration is enabled. Try find a user
    // in LDAP that may have not had there account provisioned
    // yet. If we find one, provision there account for them.
    if (module_exists('ldapauth')) {
      _ldapauth_init(db_result(db_query("SELECT sid FROM {ldapauth} WHERE status = 1 ORDER BY weight")));
      global $_ldapauth_ldap;
       // If there is no bindn and bindpw - the connect will be an anonymous connect.
      $_ldapauth_ldap->connect($_ldapauth_ldap->getOption('binddn'), $_ldapauth_ldap->getOption('bindpw'));
      foreach (explode("\r\n", $_ldapauth_ldap->getOption('basedn')) as $base_dn) {
        if (empty($base_dn))
          continue;

        $name_attr = $_ldapauth_ldap->getOption('mail_attr') ? $_ldapauth_ldap->getOption('mail_attr') : LDAPAUTH_DEFAULT_USER_ATTR;
        $filter = $name_attr .'='. $mail;
        $result = $_ldapauth_ldap->search($base_dn, $filter);
        if (!$result)
          continue;

        $num_matches = $result['count'];
        // Must find exactly one user for authentication to.
        if ($num_matches != 1) {
          watchdog('ldapauth', "Error: %num_matches users found with $%filter under %base_dn.", array('%num_matches' => $num_matches, '%filter' => $filter, '%base_dn' => $base_dn), WATCHDOG_ERROR);
          continue;
        }
        $match = $result[0];
        break;
      }
      if (isset($match) && $match) {
        $user_data = array(
          'name' => $match['cn'][0],
          'pass' => user_password(20),
          'mail' => $mail,
          'init' => $mail,
          'status' => 1,
          'authname_ldapauth' => $match['cn'][0],
          'ldap_authentified' => TRUE,
          'ldap_dn' => $match['dn'],
          'ldap_config' => $_ldapauth_ldap->getOption('sid'),
        );
        $user = (object) $user_data;
        user_save($user, $user_data);
        $user = user_load(array('mail' => $mail));
        $uid = $user->uid;
        return $uid;
      }
    }
  }
  return 0;
}

/**
 * Definition of an Archimedes Server Plugin.
 */
abstract class ArchimedesServerPlugin {

  /**
   * XML Namespace.
   */
  protected $ns;

  public function __construct ($ns) {
    $this->ns = $ns;
  }

  /**
   * Parse a DOMElement value and return the CCK key values to save the value.
   */
  abstract public function parse($value);

}

