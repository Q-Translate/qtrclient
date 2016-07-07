<?php
/**
 * @file
 * Export translations to a file.
 */

/**
 * Callback for qtr/export.
 *
 * Export the translations to a file.
 */
function qtrTranslations_export() {
  $output = qcl::get_block('qtrTranslations', 'export');
  return $output;
}
