<?php
/**
 * @file
 * Export the best translations of a project into PO format.
 */

/**
 * Callback for translations/export.
 *
 * Export the best translations of a project into PO format.
 */
function qtrTranslations_export() {
  $output = qcl::get_block('qtrProject', 'export');
  return $output;
}
