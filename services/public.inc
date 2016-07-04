<?php
/**
 * @file
 * Define public web services.
 */

/**
 * Create and return the public service.
 */
function _qtrClientWS_public($service_name) {
  $server_url = variable_get('oauth2_login_oauth2_server', '');

  $service = new WSClientServiceDescription();
  $service->name = $service_name;
  $service->label = 'Public Q-Translate Services';
  $service->type = 'rest';
  $service->url = $server_url . '/api/';

  $service->settings += oauth2_user_wsclient_dev_settings();

  $service->operations += _qtrClientWS_public_report();
  $service->operations += _qtrClientWS_public_translations();

  return $service;
}

/**
 * Return definitions for operations about reports.
 */
function _qtrClientWS_public_report() {
  $operations['report_stats'] = array(
    'label' => 'report_stats',
    'url' => 'report/stats',
    'parameter' => array(
      'lng' => array('type' => 'text'),
    ),
  );

  $operations['report_topusers'] = array(
    'label' => 'report_topusers',
    'url' => 'report/topusers',
    'parameter' => array(
      'period' => array('type' => 'text'),
      'size' => array('type' => 'integer'),
      'lng' => array('type' => 'text'),
    ),
  );

  $operations['report_stats_1'] = array(
    'label' => 'report_stats_1',
    'url' => 'report/stats',
    'type' => 'POST',
    'data' => 'params',
    'parameter' => array(
      'params' => array('type' => 'array'),
    ),
  );

  $operations['report_topusers_1'] = array(
    'label' => 'report_topusers_1',
    'url' => 'report/topusers',
    'type' => 'POST',
    'data' => 'params',
    'parameter' => array(
      'params' => array('type' => 'array'),
    ),
  );

  return $operations;
}

/**
 * Return definitions for operations about translations.
 */
function _qtrClientWS_public_translations() {
  $operations['get_translations'] = array(
    'label' => 'get_translations',
    'url' => 'translations/@selector',
    'parameter' => array(
      'selector' => array('type' => 'text'),
    ),
  );

  $operations['get'] = array(
    'label' => 'get',
    'url' => 'translations/get',
    'type' => 'POST',
    'data' => 'params',
    'parameter' => array(
      'params' => array('type' => 'array'),
    ),
  );

  $operations['search'] = array(
    'label' => 'search',
    'url' => 'translations/search',
    'type' => 'POST',
    'data' => 'params',
    'parameter' => array(
      'params' => array('type' => 'array'),
    ),
  );

  return $operations;
}
