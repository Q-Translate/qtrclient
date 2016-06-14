<?php
/**
 * @file
 * Submit functions for qtrVocabulary_form.
 */

/**
 * Submit callback for: qtrVocabulary_form
 */
function qtrVocabulary_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $op = $form_values['op'];

  if ($op == '<span class="glyphicon glyphicon-repeat"></span>') {
    qtrVocabulary_form_go($form_state);
  }
  elseif ($op == t('Save')) {
    _qtrVocabulary_form_save($form, $form_state);
  }
  elseif ($op == t('Next')) {
    $form_state['values']['search_term'] = '';
    qtrVocabulary_form_go($form_state);
  }
  elseif ($op == t('Add Term')) {
    _qtrVocabulary_form_add($form, $form_state);
  }
  elseif ($op == t('Delete Term')) {
    _qtrVocabulary_form_delete($form, $form_state);
  }
  else {
    // Do nothing.
  }
}

/**
 * Go to the selected term.
 */
function qtrVocabulary_form_go($form_state) {
  $form_values = $form_state['values'];
  $term = $form_values['search_term'];
  $vocabulary = $form_values['vocabulary'];
  $query = $form_values['get_query'];
  unset($query['q']);
  $url = "vocabulary/$vocabulary";
  if (trim($term) != '') {
    $url .= "/$term";
  }
  // Redirect.
  drupal_goto($url, array('query' => $query));
}

/**
 * The button 'Save' has been clicked.
 */
function _qtrVocabulary_form_save($form, $form_state) {
  // Make sure that user is authenticated.
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = $form===NULL);
    return;
  }

  // Save the values selected on the form.
  qcl::translateform_save($form_state['values']);

  // Redirect to the same term.
  qtrVocabulary_form_go($form_state);
}

/**
 * The button 'Add Term' has been clicked.
 */
function _qtrVocabulary_form_add($form, $form_state) {
  // Make sure that user is authenticated.
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = $form===NULL);
  }

  // Check access rights.
  if (!qcl::user_access('qtranslator-import')) {
    drupal_set_message(t("You don't have enough rights for adding new terms."), 'error');
    return;
  }

  // Add the new term.
  try {
    $term =  $form_state['values']['search_term'];
    $vocabulary =  $form_state['values']['vocabulary'];
    $qtr = wsclient_service_load('qtr');
    $result = $qtr->add_string(array(
                'origin' => 'vocabulary',
                'project' => $vocabulary,
                'tplname' => $vocabulary,
                'string' => $term,
                'context' => $vocabulary,
                'notify' => TRUE,
              ));
    if ($result['sguid']) {
      // Redirect to the new term.
      qtrVocabulary_form_go($form_state);
    }
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * The button 'Delete Term' has been clicked.
 */
function _qtrVocabulary_form_delete($form, $form_state) {
  // Make sure that user is authenticated.
  if (!oauth2_user_is_authenticated()) {
    oauth2_user_authenticate($form_state, $redirection = $form===NULL);
  }

  // Check access rights.
  if (!qcl::user_access('qtranslator-admin')) {
    drupal_set_message(t('Only an admin can delete vocabulary terms.'), 'error');
    return;
  }

  // Get the sguid of the string;
  foreach ($form_state['values']['strings'] as $sguid => $string) {
    break;
  }

  // Delete the term.
  $qtr = wsclient_service_load('qtr');
  try {
    $term =  $form_state['values']['search_term'];
    $qtr->del_string(array('sguid' => $sguid));
    drupal_set_message(t('Term "!term" was deleted.', array('!term' => $term)));
    qtrVocabulary_form_go($form_state);
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}
