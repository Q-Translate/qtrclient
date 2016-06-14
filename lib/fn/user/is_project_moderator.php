<?php
/**
 * @file
 * Definition of function user_is_project_moderator().
 */

namespace BTranslator\Client;
use \qcl;

/**
 * Return TRUE if the oauth2 user can moderate the given project.
 */
function user_is_project_moderator($origin, $project, $lng = NULL) {
  // If user has admin permissions, he can also moderate this project.
  if (qcl::user_is_project_admin($origin, $project, $lng))  return TRUE;

  // Check that the project language matches translation_lng of the user.
  $qtr_user = oauth2_user_get();
  if ($lng !== NULL and $lng != $qtr_user['translation_lng']) return FALSE;

  // Check whether the user is an moderator of the given project.
  if (in_array("$origin/$project", $qtr_user['moderate_projects'])) return TRUE;

  // Otherwise he cannot moderate.
  return FALSE;
}
