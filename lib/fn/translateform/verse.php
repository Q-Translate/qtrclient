<?php
/**
 * @file
 * Function: translateform_verse()
 */

namespace QTranslate\Client;
use \qcl;

require_once __DIR__ . '/theme_functions.inc';

/**
 * Creates the form fragment for a verse and its translations.
 */
function translateform_verse($verse, $lng) {

  $form = array(
    '#verse' => $verse,
    '#langcode' => $lng,
    'source' => array(
      'verse' => array(
        '#markup' => '<span>' . $verse['verse'] . '</span>'
      )),
  );

  $form['source']['edit'] = array(
    '#markup' => t('Translate'),
    '#prefix' => '<label title="' . t('Translate') . '">',
    '#suffix' => '</label>',
  );

  // Translations are stored in an array.
  $translations = $verse['translations'];

  foreach ($translations as $translation) {
    // Add the translation to the list.
    $tguid = $translation['tguid'];
    $form[$tguid] = _translation($translation, $verse['vid'], $lng);
  }

  // Display a textarea for adding new translations.
  $form['new'] = _new_translation($translation['tguid'], $verse['vid'], $lng);

  return $form;
}

/**
 * Create the form fragment for adding a new translation.
 */
function _new_translation($tguid, $vid, $lng) {
  $form = array(
    '#theme' => 'qtrClient_translate_translation',
    'original' => array(
      '#type' => 'value',
      '#value' => array(
        'vid' => $vid,
        'tguid' => 'new',
        'lng' => $lng,
        'translation' => '',
        'count' => '0',
        'likes' => array(),
      ),
    ),

    // The 'approved' radio is used to pick the approved translation.
    'approved' => array(
      '#type' => 'radio',
      '#checked' => FALSE,
      '#theme' => 'qtrClient_translate_radio',
      '#theme_wrappers' => array(),
      '#title' => '<span data-empty="' . t('(empty)') . '"></span>',
      '#return_value' => $tguid,
      '#default_value' => '',
      '#attributes' => array('class' => array('selector')),
      '#parents' => array('verses', $vid, 'approved'),
    )

    'value' => array(
      '#type' => 'textarea',
      '#cols' => 60,
      '#rows' => 3,
      '#default_value' => t('<New translation>'),
      '#attributes' => array(
        'defaultValue' => t('<New translation>'),
      ),
    )
  );

  return $form;
}

/**
 * Create the form fragment for a translation.
 */
function _translation($translation, $vid, $lng) {

  $qtr_user = oauth2_user_get();
  $is_own = ($qtr_user['name'] and ($qtr_user['name'] == $translation['author']));
  $is_approved = ($qtr_user['name'] and in_array($qtr_user['name'], array_keys($translation['likes'])));
  $may_moderate = ($is_own or qcl::user_access('qtranslate-resolve'));

  $form = array(
    '#theme' => 'qtrClient_translate_translation',
    'original' => array(
      '#type' => 'value',
      '#value' => $translation,
    ),

    // The 'approved' radio is used to pick the approved translation.
    'approved' => array(
      '#type' => 'radio',
      '#checked' => $is_approved,
      '#theme' => 'qtrClient_translate_radio',
      '#theme_wrappers' => array(),
      '#title' => "<span>" . check_plain($translation['translation']) . '</span>',
      '#return_value' => $translation['tguid'],
      '#default_value' => $is_approved ? $translation['tguid'] : '',
      '#attributes' => array('class' => array('selector')),
      '#parents' =>  array('verses', $vid, 'approved')
    )
  );

  if ($may_moderate) {
    $form['declined'] = array(
      '#type' => 'checkbox',
      '#title' => t('Decline'),
      '#default_value' => FALSE,
    );
  }

  $form['edit'] = array(
    '#markup' => t('Edit a copy'),
    '#prefix' => '<label title="' . t('Edit a copy') . '">',
    '#suffix' => '</label>',
  );
  $form['author'] = array(
    '#name' => $translation['author'],
    '#uid' => $translation['uid'],
    '#time' => $translation['time'],
  );
  $form['likes'] = array(
    '#count' => $translation['count'],
    '#likers' => $translation['likes'],
  );

  return $form;
}
