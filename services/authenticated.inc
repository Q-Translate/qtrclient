<?php
/**
 * @file
 * Define authenticated web services.
 */

/**
 * Create and return the authenticated service.
 */
function _qtrClientWS_authenticated($service_name) {
  $server_url = variable_get('oauth2_login_oauth2_server', '');

  $service = new WSClientServiceDescription();
  $service->name = $service_name;
  $service->label = 'Authenticated Q-Translate Services';
  $service->type = 'rest';
  $service->url = $server_url . '/api/';

  $service->settings['authentication']['oauth2'] = oauth2_login_get_oauth2_settings();
  $service->settings += oauth2_user_wsclient_dev_settings();

  $service->operations += _qtrClientWS_authenticated_translations();

  return $service;
}

/**
 * Return definitions for operations about translations.
 */
function _qtrClientWS_authenticated_translations() {
  $operations['get_translations'] = array(
    'label' => 'get_translations',
    'url' => 'translations/@vid',
    'parameter' => array(
      'vid' => array('type' => 'text'),
      'lng' => array('type' => 'text'),
    ),
  );

  $post_operations = array(
    'search' => 'translations/search',
    'add_translation' => 'translations/add',
    'add_like' => 'translations/like',
    'del_translation' => 'translations/del',
    'submit' => 'translations/submit',
    'import_translations' => 'translations/import',
  );
  foreach ($post_operations as $name => $url) {
    $operations[$name] = array(
      'label' => $name,
      'url' => $url,
      'type' => 'POST',
      'data' => 'params',
      'parameter' => array(
        'params' => array('type' => 'array'),
      ),
    );
  }

  return $operations;
}
