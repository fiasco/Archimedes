<?php
// $Id: alfresco.user.inc,v 1.1.2.1 2010/04/26 12:33:03 xergius Exp $

/**
 * @file
 * Provides functions for xxx
 */

/**
 * Verifica que la cuenta de Alfresco del usuario sea válida.
 *
 * @todo: es posible retrasar la verificación del usuario hasta el primer uso
 *   de acceso al repositorio de alfresco.
 */
function alfresco_user_login($edit) {
  global $user;

  // Si el usuario no tiene habilitado acceso al repositorio de Alfresco,
  // utilizará las credenciales genéricas para descargar los elementos.
  if (isset($user->alfresco['status']) && $user->alfresco['status'] == 0) {
    return;
  }

  // Autenticación contra Alfresco para validar la cuenta del usuario
  if (alfresco_auth_is_sso()) {

    // Si el repositorio no está correctamente configurado no intentamos
    // validar la cuenta del usuario contra Alfresco.
    if (variable_get('alfresco_http_request_fails', FALSE)) {
      return;
    }

    // Eliminamos la información de conexión como usuario anónimo, para forzar
    // a realizar la autenticación con las credenciales del usuario.
    alfresco_include('soap');
    alfresco_soap_clear_session();

    // Obtenemos las credenciales del usuario
    // @todo Hacer configurable que credenciales se van a usar
    if (!empty($user->alfresco['username']) && !empty($user->alfresco['password'])) {
      $username = $user->alfresco['username'];
      $password = $user->alfresco['password'];
    }
    else {
      $username = $edit['name'];
      $password = $edit['pass'];
    }

    // Guardamos las credenciales para re-autenticar cuando el ticket caduque.
    $_SESSION['alfresco_account']['username'] = $username;
    $_SESSION['alfresco_account']['password'] = base64_encode($password);

    // Comprobamos la cuenta de Alfresco del usuario
    if (alfresco_soap_verify_account($username, $password)) {
      $_SESSION['alfresco_account']['verified'] = TRUE;
    }
    else {
      $_SESSION['alfresco_account']['verified'] = FALSE;
    }
  }
}

/**
 *
 */
function alfresco_user_logout() {
  // No es necesario limpiar el ticket porque la sesión se destruye cuando un
  // usuario cierra la conexión.
  // @see user_logout()
}

/**
 *
 */
function alfresco_user_view(&$account) {

}

/**
 * @todo Crear cuenta de alfresco si no tiene y está configurado.
 */
function alfresco_user_insert(&$edit, $account) {
  alfresco_include('soap');

  // Set default alfresco user space
  $home = '/app:company_home/app:user_homes/cm:'. $account->name;     // Alfresco 3.x
  if (alfresco_soap_node_load($home)) {
    $edit['alfresco']['home'] = $home;
  }
  else {
    $home = '/app:company_home/app:user_homes/sys:'. $account->name;  // Alfresco 2.x
    if (!alfresco_soap_node_load($home)) {
      $edit['alfresco']['home'] = $home;
    }
  }
}

/**
 *
 */
function alfresco_user_update($edit, $account) {

}

/**
 *
 */
function alfresco_user_delete($account) {

}

/**
 * The user account edit form is about to be displayed.
 */
function alfresco_user_form($edit, $account, $category) {

  /*
  if (user_access('administer users') && $category == 'account') {
    $form['alfresco'] = array(
      '#type' => 'fieldset',
      '#title' => t('Alfresco settings'),
      //'#weight' => 5,
      '#collapsible' => TRUE,
    );

    $form['alfresco']['alfresco_access'] = array(
      '#type' => 'radios',
      '#title' => t('Repository access'),
      '#default_value' => isset($edit['alfresco_access']) ? $edit['alfresco_access'] : 1,
      '#options' => array(t('Disabled'), t('Enabled')),
      '#description' => t('Indica si el usuario tiene acceso directo al repositorio de Alfresco (para búscar, navegar, añadir contenido, etc).'),
    );

    return $form;
  }
  */

  if ($category == 'alfresco') {
    //drupal_add_js(drupal_get_path('module', 'alfresco') .'/alfresco.js');

    // Account information
    $form['alfresco'] = array(
      '#type' => 'fieldset',
      '#title' => t('Alfresco account information'),
      '#weight' => -10,
      '#tree' => TRUE,
    );

    if (user_access('administer users')) {
      if (alfresco_auth_is_sso()) {
        $form['alfresco']['username'] = array(
          '#type' => 'textfield',
          '#title' => t('Username'),
          '#default_value' => isset($edit['alfresco']['name']) ? $edit['alfresco']['name'] : $edit['name'],
        );
        $form['alfresco']['password'] = array(
          '#type' => 'password_confirm',
          '#size' => 25,
        );
      }
      $form['alfresco']['status'] = array(
        '#type' => 'radios',
        '#title' => t('Repository access'),
        '#default_value' => isset($edit['alfresco']['status']) ? $edit['alfresco']['status'] : 1,
        '#options' => array(t('Blocked'), t('Enabled'))
      );
    }

    $form['alfresco']['home'] = array(
      '#type' => 'textfield',
      '#title' => t('User home space'),
      '#default_value' => isset($edit['alfresco']['home']) ? $edit['alfresco']['home'] : '',
      '#description' => t('User home path on Alfresco repository.'),
    );

    return $form;
  }
}

/**
 * The user account is about to be modified.
 */
function alfresco_user_validate($edit, $account, $category) {
  if (!empty($edit['alfresco']['home'])) {
    if (!alfresco_valid_path($edit['alfresco']['home'])) {
      form_set_error('alfresco][home', t('The home path you specified is not valid.'));
    }
    else {
      alfresco_include('soap');
      if (!alfresco_soap_node_load($edit['alfresco']['home'])) {
        form_set_error('alfresco][home', t('The home path you specified could not be found.'));
      }
    }
  }
}
