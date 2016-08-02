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

  list($lng, $_, $_) = qcl::get_args_from_path();
  if (!$lng) $lng = variable_get('qtrClient_translation_lng', 'en');
  $qtr_server = variable_get('qtrClient_server', 'https://qtranslate.org');
  $rss = "$qtr_server/rss-feed/$lng";

  $output[] = array(
    // select, search, stats, topusers
    'row-1' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      'col-11' => [
        '#prefix' => '<div class="col-sm-6">',
        '#suffix' => '</div>',

        'row-111' => [
          '#prefix' => '<div class="row">',
          '#suffix' => '</div>',

          'col-1111' => [
            '#prefix' => '<div class="col-sm-6">',
            '#suffix' => '</div>',
            $block_select,
          ],

          'col-1112' => [
            '#prefix' => '<div class="col-sm-6">',
            '#suffix' => '</div>',
            $block_search,
          ],
        ],

        'row-112' => [
          '#prefix' => '<div class="row">',
          '#suffix' => '</div>',

          'col-1121' => [
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>',
            qcl::get_block('qtrClient', 'export'),
          ],

          'col-1122' => [
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>',
            qcl::get_block('qtrClient', 'import'),
          ],
        ],
      ],

      'col-12' => [
        '#prefix' => '<div class="col-sm-6">',
        '#suffix' => '</div>',

        'row-121' => [
          '#prefix' => '<div class="row">',
          '#suffix' => '</div>',

          'col-1211' => [
            '#prefix' => "<div class='col-sm-12'><a href='$rss' target='_blank' class='btn btn-info' style='float:right'>RSS</a>",
            '#suffix' => '</div>',
            qcl::get_block('qtrClient', 'latest'),
          ],
        ],

        'row-122' => [
          '#prefix' => '<div class="row">',
          '#suffix' => '</div>',
          '#markup' => '<hr/>',
        ],

        'row-123' => [
          '#prefix' => '<div class="row">',
          '#suffix' => '</div>',

          'col-1231' => [
            '#prefix' => '<div class="col-sm-6">',
            '#suffix' => '</div>',
            qcl::get_block('qtrClient', 'stats'),
          ],

          'col-1232' => [
            '#prefix' => '<div class="col-sm-6">',
            '#suffix' => '</div>',
            qcl::get_block('qtrClient', 'topusers'),
          ],
        ],
      ],
    ]);

  return $output;
}
