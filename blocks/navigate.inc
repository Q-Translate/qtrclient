<?php
/**
 * @file
 * Functions for the block: navigate
 */

/**
 * Called from hook_block_view for navigate.
 */
function qtrClient_navigate_view() {
  list($lng, $chapter, $verse) = qcl::get_args_from_path();
  if (!$lng) $lng = variable_get('qtrClient_translation_lng', 'en');
  if ($lng == 'all') $lng = 'en';

  //$block['subject'] = t('Navigate');
  $block['content'] = drupal_get_form('qtrClient_navigate_form', $lng, $chapter, $verse);

  return $block;
}

/**
 * Form callback for: qtrClient_navigate_form
 */
function qtrClient_navigate_form($form, &$form_state, $lng, $chapter, $verse) {
  $form = [
    '#attached' => [
      'css' => [ drupal_get_path('module', 'qtrClient') . '/blocks/navigate.css.less' ],
    ],

    'lng' => [
      '#type' => 'hidden',
      '#value' => $lng,
    ],
    'chapter' => [
      '#type' => 'hidden',
      '#value' => $chapter,
    ],
    'verse' => [
      '#type' => 'hidden',
      '#value' => $verse,
    ],

    'prev_chapter' => [
      '#value' => '<span class="glyphicon glyphicon-backward"></span>',
      '#type' => 'submit',
      '#attributes' => ['class' => ['btn-primary']],
    ],
    'prev_verse' => [
      '#value' => '<span class="glyphicon glyphicon-triangle-left"></span>',
      '#type' => 'submit',
      '#attributes' => ['class' => ['btn-primary']],
    ],
    'home' => [
      '#value' => '<span class="glyphicon glyphicon-home"></span>',
      '#type' => 'submit',
      '#attributes' => ['class' => ['btn-primary']],
    ],
    'next_verse' => [
      '#value' => '<span class="glyphicon glyphicon-triangle-right"></span>',
      '#type' => 'submit',
      '#attributes' => ['class' => ['btn-primary']],
    ],
    'next_chapter' => [
      '#value' => '<span class="glyphicon glyphicon-forward"></span>',
      '#type' => 'submit',
      '#attributes' => ['class' => ['btn-primary']],
    ],
  ];

  return $form;
}

/**
 * Submit callback for the form: qtrClient_navigate_form
 */
function qtrClient_navigate_form_submit($form, &$form_state) {
  // Get the parameters.
  $values = $form_state['values'];
  $operation = $values['op'];
  $lng = $values['lng'];
  $chapter = $values['chapter'];
  $verse = $values['verse'];

  if ($operation == '<span class="glyphicon glyphicon-backward"></span>') {
    $chapter -= 1;
    if ($chapter < 1) {
      $chapter = 114;
      drupal_set_message(t('Moved to the last chapter.'));
    }
    $verse = 1;
    drupal_goto("qtr/$lng/$chapter/$verse");
  }

  elseif ($operation == '<span class="glyphicon glyphicon-triangle-left"></span>') {
    $verse -= 1;
    if ($verse < 1) {
      $chapter -= 1;
      drupal_set_message(t('Moved to the previous chapter.'));
      if ($chapter < 1) {
        $chapter = 114;
        drupal_set_message(t('Moved to the last chapter.'));
      }
      $chapters = qcl::get_chapters();
      $verse = $chapters[$chapter]['verses'];
    }
    drupal_goto("qtr/$lng/$chapter/$verse");
  }

  elseif ($operation == '<span class="glyphicon glyphicon-home"></span>') {
    drupal_goto("qtr/$lng");
  }

  elseif ($operation == '<span class="glyphicon glyphicon-triangle-right"></span>') {
    $verse += 1;
    $chapters = qcl::get_chapters();
    if ($verse > $chapters[$chapter]['verses']) {
      $chapter += 1;
      drupal_set_message(t('Moved to the next chapter.'));
      if ($chapter > 114) {
        $chapter = 1;
        drupal_set_message(t('Moved to the first chapter.'));
      }
      $verse = 1;
    }
    drupal_goto("qtr/$lng/$chapter/$verse");
  }

  elseif ($operation == '<span class="glyphicon glyphicon-forward"></span>') {
    $chapter += 1;
    if ($chapter > 114) {
      $chapter = 1;
      drupal_set_message(t('Moved to the first chapter.'));
    }
    $verse = 1;
    drupal_goto("qtr/$lng/$chapter/$verse");
  }
}
