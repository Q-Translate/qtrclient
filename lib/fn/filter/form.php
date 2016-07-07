<?php
/**
 * @file
 * Function: filter_form()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Build and return the filter form.
 */
function filter_form($form_values) {
  $form = [
    '#prefix' => '<div id="filter-form">',
    '#suffix' => '</div>',

    /* css attachment does not work
    '#attached' => [
      'css' => [
        drupal_get_path('module', 'qtrClient') . '/lib/fn/filter/style.css',
      ],
    ],
    */

    'basic' => [
      '#type' => 'fieldset',

      // buttons
      'submit' => [
        '#value' => '<span class="glyphicon glyphicon-repeat"></span>',
        '#type' => 'submit',
        '#attributes' => ['class' => ['btn-primary']],
      ],

      // advanced search checkbox
      'options' => [
        '#markup' => '
            <div class="btn-group" data-toggle="buttons" style="float: right;">
              <label class="btn btn-default">
                <input type="checkbox" id="edit-options" name="options">
                <span class="glyphicon glyphicon-option-vertical"></span>
              </label>
            </div>
        ',
      ],

      // words to be searched
      'words' => [
        '#type' => 'textfield',
        '#default_value' => $form_values['words'],
      ],
    ],

    // advanced search fieldset
    'advanced' => [
      '#type' => 'fieldset',
      '#states' => [
        'visible' => [
          ':input[name="options"]' => ['checked' => TRUE],
        ]],
    ],
  ];

  // Add form fieldset advanced.
  $form['advanced'] += _advanced($form_values);

  return $form;
}

/**
 * Return the fieldset of advanced search.
 */
function _advanced($form_values) {
  // Get a list of languages.
  $languages = qcl::get_languages();
  $language_list[''] = t('- Language -');
  foreach ($languages as $code => $lang) {
    $language_list[$code] = $lang['name'];
  }
  // Get a list of chapters.
  $chapters = qcl::get_chapters();
  $chapter_list[''] = t('- Chapter -');
  for ($c = 1; $c <=114; $c++) {
    $chapter_list["$c"] = $c . ' : ' . $chapters[$c]['tname'];
  }

  // The number of results that should be displayed per page.
  list($limit_options, $default) = qcl::filter_get_options('limit', 'assoc');

  $advanced = [
    'first-row' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      // Language of translations.
      'lng' => [
        '#type' => 'select',
        '#title' => t('Language'),
        '#description' => t('Select the language of the translations to be searched and displayed.'),
        '#options' => $language_list,
        '#default_value' => $form_values['lng'],
        '#prefix' => '<div class="col-xs-4">',
        '#suffix' => '</div>',
      ],

      // Chapter to search on.
      'chapter' => [
        '#type' => 'select',
        '#title' => t('Chapter'),
        '#description' => t('Chapter to be searched.'),
        '#options' => $chapter_list,
        '#default_value' => $form_values['chapter'],
        '#prefix' => '<div class="col-xs-4">',
        '#suffix' => '</div>',
      ],

      // Number of results that can be displayed on a page.
      'limit' => [
        '#type' => 'select',
        '#title' => t('Limit'),
        '#description' => t('The number of the results (verses) that can be displayed on a page.'),
        '#options' => $limit_options,
        '#default_value' => $form_values['limit'],
        '#prefix' => '<div class="col-xs-4">',
        '#suffix' => '</div>',
      ],
    ],

    'second-row' => [
      '#prefix' => '<div class="row">',
      '#suffix' => '</div>',

      // search mode
      'search_mode' => [
        _search_mode($form_values),
        '#prefix' => '<div class="col-sm-4">',
        '#suffix' => '</div>',
      ],

      // author
      'author' => [
        _author($form_values),
        '#prefix' => '<div class="col-sm-4">',
        '#suffix' => '</div>',
      ],

      // date
      'date' => [
        _date($form_values),
        '#prefix' => '<div class="col-sm-4">',
        '#suffix' => '</div>',
      ],
    ],
  ];

  return $advanced;
}

/**
 * Return search mode.
 */
function _search_mode($form_values) {
  list($search_mode_options, $default) = qcl::filter_get_options('mode', 'assoc');

  $params = array(
    '!url1' => 'http://dev.mysql.com/doc/refman/5.1/en/fulltext-natural-language.html',
    '!url2' => 'http://dev.mysql.com/doc/refman/5.1/en/fulltext-boolean.html',
  );
  $description = t('Search for verses or translations that contain the given words. The <emphasized>natural</emphasized> search will try to find words similar to the given one (see: <a href="!url1">Natural Language Full-Text Searches</a>). The <emphasized>boolean</emphasized> search will try to match words according to logical rules. The words can be preceded by + (plus), - (minus), etc. (for more details see: <a href="!url2">Boolean Full-Text Searches</a>).', $params);

  $search_mode = [
    '#type' => 'fieldset',
    '#title' => t('Search Mode'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,

    // mode
    'mode' => [
      '#type' => 'select',
      '#title' => t('Search Mode'),
      '#options' => $search_mode_options,
      '#default_value' => $form_values['mode'],
      '#description' => $description,
    ],
  ];

  return $search_mode;
}

/**
 * Return the author fieldset.
 */
function _author($form_values) {
  $qtr_server = variable_get('qtrClient_server');
  $lng = qcl::get_translation_lng();

  $author = [
    '#type' => 'fieldset',
    '#title' => t('Author'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,

    // Only mine.
    'only_mine' => [
      '#type' => 'checkbox',
      '#title' => t('Only Mine'),
      '#description' => t('Search only the verses with translations suggested or liked by me.'),
      '#default_value' => $form_values['only_mine'],
    ],

    // Translated by.
    'translated_by' => [
      '#type' => 'textfield',
      '#title' => t('Translated By'),
      '#description' => t('Search only the verses with translations suggested by the selected user.'),
      '#default_value' => $form_values['translated_by'],
      '#autocomplete_path' => $qtr_server . '/auto/user/' . $lng,
      '#states' => [
        'visible' => [
          ':input[name="only_mine"]' => ['checked' => FALSE],
        ]],
    ],

    // Liked by.
    'liked_by' => [
      '#type' => 'textfield',
      '#title' => t('Liked By'),
      '#description' => t('Search only the verses with translations liked by the selected user.'),
      '#default_value' => $form_values['liked_by'],
      '#autocomplete_path' => $qtr_server . '/auto/user/' . $lng,
      '#states' => [
        'visible' => [
          ':input[name="only_mine"]' => ['checked' => FALSE],
        ]],
    ],
  ];

  return $author;
}

/**
 * Return the date fieldset.
 */
function _date($form_values) {
  list($date_filter_options, $default) = qcl::filter_get_options('date_filter', 'assoc');

  $date = [
    '#type' => 'fieldset',
    '#title' => t('Date'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,

    // What to filter.
    'date_filter' => [
      '#type' => 'select',
      '#title' => t('What to Filter'),
      '#description' => t('Select what to filter by date (translations, or likes).'),
      '#options' => $date_filter_options,
      '#default_value' => $form_values['date_filter'],
    ],

    // From date.
    'from_date' => [
      '#type' => 'textfield',
      '#title' => t('From Date'),
      '#default_value' => $form_values['from_date'],
    ],

    // To date.
    'to_date' => [
      '#type' => 'textfield',
      '#title' => t('To Date'),
      '#default_value' => $form_values['to_date'],
    ],
  ];

  return $date;
}
