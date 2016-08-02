<?php
/**
 * @file
 * Function: translateform_buttons()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Get the buttons of the form as a render array.
 */
function translateform_buttons($lng, $verse = NULL) {
  // The save button will appear only when the user has
  // permissions to submit likes and translations.
  $translation_lng = variable_get('qtrClient_translation_lng', 'all');
  $enable_save = ($translation_lng == $lng or $translation_lng == 'all');
  $buttons['save'] = [
    '#type' => 'submit',
    '#value' => t('Save'),
    '#access' => $enable_save,
    '#attributes' => ['onclick' => 'this.form.target="_self"'],
  ];
  // When the user is not authenticated, clicking Save will redirect
  // to login. When we are inside an iFrame, it is better to do the
  // login on a popup window (or a new tab).
  if (!oauth2_user_is_authenticated() and inside_iframe()) {
    $buttons['save']['#attributes']['onclick'] = 'this.form.target="_blank"';
  }

  if ($verse !== NULL) {
    $buttons += qcl::translateform_meta($lng, $verse);
  }
  else {
    $buttons['save']['#attributes']['style'] = 'margin-left: 50%;';
  }

  /* --------------------------------------------------- */

  // Display/hide related references.
  $buttons['references'] = [
    '#markup' => '
        <div class="btn-group" data-toggle="buttons">
          <label class="btn btn-default">
            <input type="checkbox" id="edit-references" name="references">
            <span class="glyphicon glyphicon-share-alt"></span>
          </label>
        </div>
    ',
  ];

  // List of related sites.
  $chapter_id = $verse['cid'];
  $verse_nr = $verse['nr'];
  $reference_list = [
    'reference1' => [
      'label' => 'quran.com',
      'url' => "https://quran.com/$chapter_id/$verse_nr",
    ],
    'reference2' => [
      'label' => 'quranwow.com',
      'url' => "http://www.quranwow.com/#/ch/$chapter_id/t1/ar-allah/t2/none/a1/alafasy-64/a2/sahihinternational-64/v/$verse_nr",
    ],
    'reference3' => [
      'label' => 'tanzil.net',
      'url' => "http://tanzil.net/#$chapter_id:$verse_nr",
    ],
  ];
  $buttons['related-references'] = [
    '#type' => 'fieldset',
    '#states' => ['visible' => [':input[name="references"]' => ['checked' => TRUE],]],
    '#attributes' => ['style' => 'float: right; border: none; margin: 0px;'],
  ];
  foreach ($reference_list as $key => $reference) {
    $url = $reference['url'];
    $label = $reference['label'];
    $buttons['related-references'][$key] = [
      '#markup' => "
          <a href='$url' class='btn btn-default btn-sm' target='_blank'>
            <span class='glyphicon glyphicon-link'></span> $label
          </a>
      ",
    ];
  }

  return $buttons;
}
