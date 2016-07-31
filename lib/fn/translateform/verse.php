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

  $cid = $verse['cid'];
  $nr = $verse['nr'];
  $url = url("qtr/$lng/$cid/$nr");

  $form = array(
    '#verse' => $verse,
    '#langcode' => $lng,
    'source' => array(
      'verse' => array(
        '#markup' => "<span><a href='$url' class='verse'>" . $verse['verse'] . '</a></span>',
      )),
  );

  // Translations are stored in an array.
  $translations = $verse['translations'];

  foreach ($translations as $translation) {
    // Add the translation to the list.
    $tguid = $translation['tguid'];
    $form[$tguid] = _translation($translation, $verse, $lng);
  }

  // Display a textarea for adding new translations.
  $form['new'] = _new_translation($translation['tguid'], $verse, $lng);

  return $form;
}

/**
 * Create the form fragment for adding a new translation.
 */
function _new_translation($tguid, $verse, $lng) {
  $vid = $verse['vid'];

  $form = array(
    '#theme' => 'qtrClient_translate_translation',
    'original' => array(
      '#type' => 'value',
      '#value' => array(
        'cid' => $verse['cid'],
        'nr' => $verse['nr'],
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
      '#return_value' => 'new',
      '#default_value' => '',
      '#attributes' => array('class' => array('selector')),
      '#parents' => array('verses', $vid, 'approved'),
    ),

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
function _translation($translation, $verse, $lng) {

  $qtr_user = oauth2_user_get();
  $is_approved = ($qtr_user['name'] and in_array($qtr_user['name'], array_keys($translation['likes'])));
  $vid = $verse['vid'];

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
    ),

    'declined' => array(
      '#type' => 'checkbox',
      '#title' => t('Decline'),
      '#default_value' => FALSE,
    ),
    'edit' => array(
      '#markup' => t('Edit a copy'),
      '#prefix' => '<label title="' . t('Edit a copy') . '">',
      '#suffix' => '</label>',
    ),
    'author' => array(
      '#name' => $translation['author'],
      '#uid' => $translation['uid'],
      '#time' => $translation['time'],
    ),
    'likes' => array(
      '#count' => $translation['count'],
      '#likers' => $translation['likes'],
    ),
  );

  return $form;
}
