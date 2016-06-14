<?php
/**
 * @file
 * Function: user_access()
 */

namespace BTranslator\Client;
use \qcl;

/**
 * Check whether the current user has the given permission (on the server).
 *
 * @param string $permission
 *   The name of the permission (like 'qtranslate-*').
 *
 * @return bool
 *   TRUE or FALSE
 */
function user_access($permission) {
  // If the user has not been authenticated yet by oauth2,
  // he doesn't have any permissions.
  if (!oauth2_user_is_authenticated())  return FALSE;

  // Check the given permission on the list of permissions.
  $qtr_user = oauth2_user_get();
  if (!isset($qtr_user['permissions']))  return FALSE;
  return in_array($permission, $qtr_user['permissions']);
}
