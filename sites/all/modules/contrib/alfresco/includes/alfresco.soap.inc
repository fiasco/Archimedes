<?php
// $Id: alfresco.soap.inc,v 1.1.2.1 2010/04/26 12:33:03 xergius Exp $

/**
 * @file
 * Provides functions for access to Alfresco repository through of web service.
 */

/**
 * Return the current request session object, or create a new session.
 *
 * According to the Alfresco documentation, in general it is better to create a
 * new session for a new page or page sequence rather than creating one and
 * using across many pages as this helps to prevent resource usage from
 * creaping up.
 *
 * @return AlfSession
 */
function alfresco_soap_get_session() {
  static $session = NULL;
  if (!isset($session)) {
    $session = alfresco_soap_create_session();
  }
  return $session;
}

/**
 *
 */
function alfresco_soap_clear_session() {
  // Limpiamos el ticket actual para forzar la autenticación en la siguiente petición
  unset($_SESSION['alfresco_ticket']);
  unset($_SESSION['alfresco_ticket_timestamp']);

  // Alfresco PHP API
  unset($_SESSION['sessionIds']);

  // @todo Usar la API para cerrar la sesión
}

/**
 * Authenticate against the repository and create a new session.
 *
 * @return Session
 *
 * @see http://wiki.alfresco.com/wiki/PHP_Tutorial_One
 * @see http://wiki.alfresco.com/wiki/PHP_Tutorial_One_Complete_Script
 */
function alfresco_soap_create_session() {
  $session = NULL;

  try {
    // Include the required Alfresco PHP API objects
    alfresco_include_lib('Repository');

    // The web service end point URL for the Alfresco repository
    $repository_url = variable_get('alfresco_repository_url', 'http://localhost:8080/alfresco/api');

    // Create the repository object baseed on the repository URL.  The
    // repository represents the Alfresco repository we are connecting to.
    // It can be used to authenticate users and as a session factory.
    $repository = new AlfRepository($repository_url);

    if (isset($_SESSION['alfresco_ticket']) && isset($_SESSION['alfresco_ticket_timestamp'])) {
      $timestamp = $_SESSION['alfresco_ticket_timestamp'];
      $timeout = variable_get('alfresco_ticket_timeout', 1800);

      if (time() < (intval($timestamp) + intval($timeout))) {
        $ticket = $_SESSION['alfresco_ticket'];
      }
    }

    if (!isset($ticket)) {
      // Obtenemos las credenciales de conexión al repositorio
      $credentials = alfresco_auth_credentials();
      $ticket = $repository->authenticate($credentials['user'], $credentials['pass']);

      // Guardamos el ticket para utilizarlo más tarde
      $_SESSION['alfresco_ticket'] = $ticket;
      $_SESSION['alfresco_ticket_timestamp'] = time();
    }

    // Using the ticket we can now create a session.
    $session = $repository->createSession($ticket);

    // @todo: el método createSession no valida si el ticket aún es válido
  }
  catch (SoapFault $exception) {
    alfresco_error_handler('alfresco', $exception);

    // @todo no funciona porque aunque el ticket sea inválido no falla
    // Limpiamos el ticket actual para forzar la autenticación en la siguiente petición
    //unset($_SESSION['alfresco_ticket']);
    //unset($_SESSION['alfresco_ticket_timestamp']);
  }

  return $session;
}

function alfresco_soap_verify_account($username, $password, $end_session = TRUE) {
  alfresco_include_lib('WebServiceFactory', 'Alfresco/Service/WebService');

  try {
    $repository_url = variable_get('alfresco_repository_url', 'http://localhost:8080/alfresco/api');
    $auth_service = AlfWebServiceFactory::getAuthenticationService($repository_url);

    $result = $auth_service->startSession(array(
      'username' => $username,
      'password' => $password,
    ));

    $username = $result->startSessionReturn->username;
    $ticket = $result->startSessionReturn->ticket;
    $sessionid = $result->startSessionReturn->sessionid;

    if ($ticket && $end_session) {
      $auth_service->endSession(array('ticket' => $ticket));
    }

    return $username;
  }
  catch (SoapFault $exception) {
  }
}

/**
 * Load an alfresco node object from the Alfresco repository.
 *
 * @param $reference_or_path
 *  Either the node reference of the Alfresco node or the path to the Alfresco
 *  node (trail of QNames of the child relationships to get to the node).
 * @param $reset
 *  Whether to reset the internal node cache.
 *
 * @return AlfNode
 *  A fully-populated Alfresco node object.
 */
function alfresco_soap_node_load($reference_or_path, $reset = NULL) {
  static $nodes = array();

  if ($reset) {
    $nodes = array();
  }

  if (!$inforef = alfresco_parse_reference($reference_or_path)) {
    if (!alfresco_valid_path($reference_or_path)) {
      return FALSE;
    }
  }

  if ($inforef) {
    $ref = $inforef['reference'];
    // Is the node statically cached?
    if (isset($nodes[$ref])) {
      return drupal_clone($nodes[$ref]);
    }
  }

  try {
    if (!$session = alfresco_soap_get_session()) {
      return FALSE;
    }

    if ($inforef) {
      // @see https://issues.alfresco.com/jira/browse/AR-1063
      $store = new AlfStore($session, $inforef['address'], $inforef['scheme']);
      $nodes = $session->query($store, '+ISNODE:T +ID:"'. $inforef['reference'] .'"');
    }
    else {
      $store = new AlfStore($session, 'SpacesStore');
      $nodes = $session->query($store, 'PATH:"'. $reference_or_path .'"');
    }

    if (count($nodes)) {
      $node = $nodes[0];

      // Force populate node details from repository
      $node->getProperties();

      if ($ref = $node->__toString()) {
        $nodes[$ref] = drupal_clone($node);
      }

      return $node;
    }
  }
  catch (SoapFault $exception) {
    alfresco_error_handler('alfresco', $exception);
  }

  return FALSE;
}

/**
 *
 * @param $reference_or_path
 * @param $session
 * @param $reset
 * @return unknown_type
 */
function alfresco_soap_node_delete($reference_or_path) {
  $result = new stdClass();

  if (!$inforef = alfresco_parse_reference($reference_or_path)) {
    if (!alfresco_valid_path($reference_or_path)) {
      $result->error = TRUE;
      return $result;
    }
  }

  try {
    if (!$session = alfresco_soap_get_session()) {
      $result->error = TRUE;
      return $result;
    }

    if ($inforef) {
      $store = new AlfStore($session, $inforef['address'], $inforef['scheme']);
      $nodes = $session->query($store, '+ISNODE:T +ID:"'. $inforef['reference'] .'"');
    }
    else {
      $store = new AlfStore($session, 'SpacesStore');
      $nodes = $session->query($store, 'PATH:"'. $reference_or_path .'"');
    }

    if (count($nodes)) {
      $node = $nodes[0];

      $statements = array(
        "delete" => array(
          "where" => array(
            "nodes" => $node->__toArray()
          )
        )
      );

      $session->repositoryService->update(array('statements' => $statements));

      $result->node = $node;
    }
  }
  catch (SoapFault $exception) {
    $error_msg = alfresco_error_handler('alfresco', $exception, TRUE);
    $result->error = !empty($error_msg) ? $error_msg : t('Imposible borrar contenido. Asegúrese que tiene los permisos nececesarios para poder borrar el contenido.');
  }

  return $result;
}

/**
 * Wrapper for Lucene API.
 *
 * Examples of valid paths:
 * - "/"
 * - "/*"
 * - "/app:*"
 * - "/app:company_home"
 * - "/app:company_home/*"
 * - "/app:company_home//*"
 * - "/app:company_home//."
 * - "//app:company_home"
 * - "//app:company_home//*"
 *
 * @see http://wiki.alfresco.com/wiki/Search
 */
function alfresco_soap_search($text, $property = '@cm:name') {

  try {
    if (!$session = alfresco_soap_get_session()) {
      return FALSE;
    }

    // Lucene requires the : to be escaped using the \ character
    $property = !empty($property) ? str_replace(':', '\:', $property) : 'ALL';
    $query = '+ISNODE:T +TYPE:"cm:content" +'. $property .':"*'. $text .'*"';

    $store = new AlfSpacesStore($session);
    $results = $session->query($store, $query);

    //$nodes = $session->query($store, 'PATH:"/app:company_home/*"');

    return $results;
  }
  catch (SoapFault $exception) {
    alfresco_error_handler('alfresco', $exception);
  }
}

/**
 * Uploads a file into content store and returns the content data string which
 * can be used to populate a content property.
 *
 * @param $space
 * @param $filepath
 * @param $name
 * @param $mimetype
 * @param $encoding
 * @param $properties
 *   - cm_title - Title
 *   - cm_description - Description
 *   - cm_author - Author
 * @param $replace Replace behavior when the destination file already exists.
 *   - ALFRESCO_FILE_EXISTS_REPLACE - Replace the existing file
 *   - ALFRESCO_FILE_EXISTS_RENAME - Append _{incrementing number} until the filename is unique
 *   - ALFRESCO_FILE_EXISTS_ERROR - Do nothing and return a message error (default)
 *
 * @see alf_upload_file()
 * @see http://wiki.alfresco.com/wiki/URL_Addressability#UploadContentServlet
 */
function alfresco_soap_node_add($space, $filepath, $name = NULL, $mimetype = NULL, $encoding = NULL, $type = 'cm_content', $properties = array(), $replace = ALFRESCO_FILE_EXISTS_ERROR) {
  $result = new stdClass();

  try {
    $session = alfresco_soap_get_session();
    if (!$session) {
      $result->error = t('Imposible obtener una sesión de Alfresco.');
      return $result;
    }

    $space_node = alfresco_soap_node_load($space);
    if (!$space_node) {
      $result->error = t('El espacio especificado no existe.');
      return $result;
    }

    // Get the address and the port
    $host = $session->repository->host;
    $port = $session->repository->port;

    if (empty($name)) {
      $name = basename($filepath);
    }

    $current_node = _alfresco_soap_node_exists($space_node, $name);
    if ($current_node) {
      switch ($replace) {
        case ALFRESCO_FILE_EXISTS_RENAME:
          $name = _alfresco_soap_generate_filename($space_node->getChildren(), $name);
          break;

        case ALFRESCO_FILE_EXISTS_ERROR:
          $result->error = t('The selected file %file could not be copied, because a file by that name already exists in the destination.', array('%file' => $name));
          return $result;
      }
    }

    // Write the request header onto the socket
    $url = 'http://'. $host .':'. $port .'/alfresco/upload/'. urlencode($name) .'?ticket='. $session->ticket;

    if (!empty($mimetype)) {
      // Add mimetype if specified
      $url .= '&mimetype='. $mimetype;
    }

    if (!empty($encoding)) {
      // Add encoding if specified
      $url .= '&encoding='. $encoding;
    }

    // Return the content data that can be used to update the content property.
    $response = alfresco_file_put($filepath, $url);

    if ($response->code != 200) {
      $result->error = isset($response->error) ? $response->error : $response->code;
      return $result;
    }

    if (($index = strpos($response->data, 'contentUrl')) === FALSE) {
      $result->error = t('Respuesta no esperada del servidor');
      return $result;
    }

    $content_data = substr($response->data, $index);
    if ($content_data) {
      if ($replace == ALFRESCO_FILE_EXISTS_REPLACE && $current_node) {
        $new_node = $current_node;
      }
      else {
        $new_node = $space_node->createChild($type, 'cm_contains', 'cm_'. $name);
      }
      $new_node->cm_name = $name;
      $new_node->cm_content = $content_data;

      if (is_array($properties) && count($properties)) {
        foreach ($properties as $key => $value) {
          $new_node->$key = $value;
        }
      }

      $result->space = $space_node;
      $result->node = $new_node;

      $session->save();
    }
  }
  catch (SoapFault $exception) {
    $error_msg = alfresco_error_handler('alfresco', $exception, TRUE);
    $result->error = !empty($error_msg) ? $error_msg : t('Imposible añadir el contenido. Por favor, compruebe que tiene los permisos necesarios para añadir contenido al espacio.');
  }

  return $result;
}

function _alfresco_soap_node_exists($space, $name) {
  // @todo: No recorrer todos los hijos
  foreach ($space->getChildren() as $child) {
    $child_node = $child->getChild();
    if ($child_node->cm_name === $name) {
      return $child_node;
    }
  }
}

function _alfresco_soap_generate_filename($children, $basename) {
  $alt_name = $basename;

  // Destination file already exists, generate an alternative.
  if ($pos = strrpos($basename, '.')) {
    $name = substr($basename, 0, $pos);
    $ext = substr($basename, $pos);
  }
  else {
    $name = $basename;
  }

  $counter = 1;
  do {
    $alt_name = $name .'_'. $counter++ . $ext;

    $exists = FALSE;
    foreach ($children as $child) {
      $child_node = $child->getChild();
      if ($child_node->cm_name === $alt_name) {
        $exists = TRUE;
        break;
      }
    }

  } while ($exists);

  return $alt_name;
}
