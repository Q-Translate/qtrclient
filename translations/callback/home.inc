<?php
/**
 * @file
 * Functions for the menu path: 'qtr/'
 */

/**
 * Return the homepage or dashboard for translations.
 */
function qtrTranslations_home() {
  $block_select = qcl::get_block('qtrTranslations', 'select');
  $block_select['qtrTranslations_select']['#id'] = 'qtrtranslations-select-form-vertical';
  $block_search = qcl::get_block('qtrTranslations', 'search');
  $block_search['qtrTranslations_search']['#id'] = 'qtrtranslations-search-form-vertical';

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
        qcl::get_block('qtrTranslations', 'stats'),
        '#suffix' => '</div>',
      ],

      'topusers' => [
        '#prefix' => '<div class="col-sm-3">',
        qcl::get_block('qtrTranslations', 'topusers'),
        '#suffix' => '</div>',
      ],
    ],

    // export, import
    'second-row' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'export' => [
        '#prefix' => '<div class="col-sm-6">',
        qcl::get_block('qtrTranslations', 'export'),
        '#suffix' => '</div>',
      ],

      'import' => [
        '#prefix' => '<div class="col-sm-6">',
        qcl::get_block('qtrTranslations', 'import'),
        '#suffix' => '</div>',
      ],
    ],
  );

  return $output;
}
