<?php
/**
 * @file
 * Function: translateform_submit()
 */

namespace QTranslate\Client;
use \qcl;

/**
 * Submit the translate form.
 */
function translateform_submit($form, $form_state) {
  $op = $form_state['values']['op'];
  if ($op == t('Save')) {
    if (oauth2_user_is_authenticated()) {
      qcl::translateform_save($form_state['values']);
    }
    else {
      oauth2_user_authenticate($form_state, $redirection = ($form===NULL));
    }
  }
}
