<?php

/**
 * @file
 * Theme functions.
 */

/**
 * Main theme function for translation table.
 */
function theme_qtrClient_translate_table($variables) {

  $element = $variables['element'];
  if (sizeof(element_children($element)) == 1) {
    $output = _theme_qtrClient_translate_table_1col($element);
  }
  else {
    $output = _theme_qtrClient_translate_table_2col($element);
  }

  return $output;
}

/**
 * Format the output as 1-column table.
 */
function _theme_qtrClient_translate_table_1col($element) {

  // Get the first element.
  foreach (element_children($element) as $key) { break; }
  $elem = $element[$key];

  return theme('table', [
      'attributes' => ['class' => ['qtr-table']],
      'rows' => [
        // row of the arabic verse
        [
          [
            'class' => ['source'],
            'data' => theme('qtrClient_translate_source', ['element' => $elem]),
          ],
        ],
        // row of the translations
        [
          [
            'class' => ['translation'],
            'data' => theme('qtrClient_translate_translation_list', ['element' => $elem]),
          ],
        ],
      ],
    ]);
}

/**
 * Format the output as 2-column table.
 */
function _theme_qtrClient_translate_table_2col($element) {

  $lng = $element['#lng'];
  $header = array(
    array(
      'data' => t('Verse'),
    ),
    t('Translations') . " ($lng)",
  );

  $rows = array();

  foreach (element_children($element) as $key) {
    $rows[] = array(
      array(
        'class' => array('source'),
        'data' => theme('qtrClient_translate_source', array('element' => $element[$key])),
      ),
      array(
        'class' => array('translation'),
        'data' => theme('qtrClient_translate_translation_list', array('element' => $element[$key])),
      ),
    );
  }

  return theme('table', [
      'attributes' => ['class' => ['qtr-table']],
      'header' => $header,
      'rows' => $rows,
    ]);
}

/**
 * Theme function for a source cell in the translation table.
 */
function theme_qtrClient_translate_source($variables) {
  $element = $variables['element'];
  $lng = $element['#langcode'];
  $cid = $element['#verse']['cid'];
  $nr = $element['#verse']['nr'];

  $options = array(
    'attributes' => [
      'title' => t('Link to the verse.'),
      'class' => ['btn', 'btn-default'],
      'style' => "float: left;",
    ]);
  $output = l("$cid:$nr", "qtr/$lng/$cid/$nr", $options);
  $output .= '<label class="qtr-verse filter-no-match">' . drupal_render($element['source']['verse']) . '</label>';
  return $output;
}

/**
 * Theme appropriate actions for the given verse element.
 */
function theme_qtrClient_translate_actions($variables) {
  $actions = '';

  $likes = _theme_qtrClient_translate_likes($variables['element']);
  if ($likes) {
    $actions .= '<li>' . $likes . '</li>';
  }

  $element = $variables['element'];
  foreach (['edit', 'declined'] as $type) {
    if (isset($element[$type])) {
      $actions .= '<li class="' . $type . '">' . drupal_render($element[$type]) . '</li>';
    }
  }

  if (!empty($actions)) {
    return '<ul class="actions">' . $actions . '</ul>';
  }
  else {
    return '';
  }
}

/**
 * Theme a translation.
 */
function theme_qtrClient_translate_translation($variables) {
  $element = $variables['element'];

  $element['#attributes']['class'][] = 'clearfix';
  $element['#attributes']['class'][] = 'translation';

  // Add is-selectable and is-declinable helpers for JS.
  $element['#attributes']['class'][] = 'is-selectable';
  if (isset($element['declined'])) {
    $element['#attributes']['class'][] = 'is-declinable';
  }

  switch ($element['approved']['#return_value']) {
    case 'new':
      $element['#attributes']['class'][] = 'new-translation';
      break;

    case '0':
      $element['#attributes']['class'][] = 'no-translation';
      // Fallthrough.
    default:
      if ($element['approved']['#value'] !== '') {
        $element['#attributes']['class'][] = 'is-approved default';
      }
  }

  $output = '<li' . drupal_attributes($element['#attributes']) . '>';
  $output .= theme('qtrClient_translate_actions', ['element' => $element]);
  // Add the radio box to pick the approved translation.
  $output .= drupal_render($element['approved']);

  if (isset($element['author'])) {
    $output .= _theme_qtrClient_translate_author($element['author']);
  }
  if (isset($element['value'])) {
    $output .= drupal_render($element['value']);
  }

  return $output . '</li>';
}

/**
 * Theme a radio button to pick the approved translation.
 */
function theme_qtrClient_translate_radio($variables) {
  $element = $variables['element'];
  $type = $element['#type'];
  _form_set_class($element, array('form-radio'));
  $output = '<input type="' . $type . '" ';
  $output .= 'id="' . $element['#id'] . '" ';
  $output .= 'name="' . $element['#name'] . '" ';
  $output .= 'value="' . $element['#return_value'] . '" ';
  $output .= $element['#checked'] ? ' checked="checked" ' : ' ';
  $output .= drupal_attributes($element['#attributes']) . ' />';

  if (isset($element['#title'])) {
    $output .= '<label class="qtr-verse" for="' . $element['#id'] . '">' . $element['#title'] . '</label>';
  }
  return $output;
}

/**
 * Theme a list of translations for a verse.
 */
function theme_qtrClient_translate_translation_list($variables) {
  $element = $variables['element'];
  $output = '<ul>';
  foreach (element_children($element) as $child) {
    if ((strlen($child) == 40) || ($child == 'new')) {
      $output .= drupal_render($element[$child]);
    }
  }
  $output .= '</ul>';
  return $output;
}

function _theme_qtrClient_translate_likes($element) {
  if (!isset($element['likes'])) return '';
  if ($element['approved']['#return_value'] == 'new')  return '';

  $count = $element['likes']['#count'];
  $likers = $element['likes']['#likers'];

  // Convert likers to an html list.
  $qtr_server = variable_get('qtrClient_server');
  $list = array();
  foreach ($likers as $name => $liker) {
    $url = $qtr_server . '/user/' . $liker['uid'];
    $list[] = "<li><a href='$url' target='_blank'>$name</a></li>";
  }
  $content_html = '<ul>' . implode('', $list) . '</ul>';
  $content_html = strtr($content_html, "'", '"');

  // Use a different button type for the liked translation.
  $qtr_user = oauth2_user_get();
  $liked = ($qtr_user['name'] and in_array($qtr_user['name'], array_keys($likers)));
  $btn_type = ($liked ? 'btn-success' : 'btn-info');

  $output = "<div class='likes'><span class='btn $btn_type btn-sm' data-toggle='popover' data-trigger='click' data-placement='bottom auto' data-html='true' data-content='$content_html'>+ $count</span></div>";

  return $output;
}

/**
 * Display information about the author of the verse.
 */
function _theme_qtrClient_translate_author($author) {
  if ($author['#name'] != '' && $author['#name'] != 'admin') {
    $qtr_server = variable_get('qtrClient_server');
    $name = l($author['#name'], $qtr_server . '/user/' . $author['uid']);
    if ($author['#time']) {
      $time = format_interval(REQUEST_TIME - strtotime($author['#time']));
      $msg = t('<strong>!author</strong> (!time ago)', ['!author' => $name, '!time' => $time]);
    }
    else {
      $msg = t('<strong>!author</strong>', ['!author' => $name]);
    }
  }

  // Use different classes for the translations of the current user.
  $qtr_user = oauth2_user_get();
  $is_own = ($qtr_user['name'] and ($qtr_user['name'] == $author['#name']));
  $classes = ($is_own ? 'author is-own' : 'author');

  return "<div class='$classes'>$msg</div>";
}
