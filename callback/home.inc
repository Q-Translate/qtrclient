<?php
/**
 * @file
 * Functions for the menu path: 'qtr/'
 */

/**
 * Return the homepage or dashboard for translations.
 */
function qtrClient_home() {
  $block_select = qcl::get_block('qtrClient', 'select');
  $block_select['qtrClient_select']['#id'] = 'qtrclient-select-form-vertical';
  $block_search = qcl::get_block('qtrClient', 'search');
  $block_search['qtrClient_search']['#id'] = 'qtrclient-search-form-vertical';

  $output[] = array(
    // select, search, stats, topusers
    'first-row' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'select-translations' => [
        '#prefix' => '<div class="col-sm-3">',
        $block_select,
        '#suffix' => '</div>',
      ],

      'search-translations' => [
        '#prefix' => '<div class="col-sm-3">',
        $block_search,
        '#suffix' => '</div>',
      ],

      'stats' => [
        '#prefix' => '<div class="col-sm-3">',
        qcl::get_block('qtrClient', 'stats'),
        '#suffix' => '</div>',
      ],

      'topusers' => [
        '#prefix' => '<div class="col-sm-3">',
        qcl::get_block('qtrClient', 'topusers'),
        '#suffix' => '</div>',
      ],
    ],

    // export, import
    'second-row' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'export' => [
        '#prefix' => '<div class="col-sm-6">',
        qcl::get_block('qtrClient', 'export'),
        '#suffix' => '</div>',
      ],

      'import' => [
        '#prefix' => '<div class="col-sm-6">',
        qcl::get_block('qtrClient', 'import'),
        '#suffix' => '</div>',
      ],
    ],
  );

  return $output;
}
