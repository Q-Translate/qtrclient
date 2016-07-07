<?php
/**
 * @file
 * Import translations from PO files.
 * It is like bulk translation and voting.
 */

/**
 * Menu callback for 'qtr/import'.
 */
function qtrTranslations_import() {
  $output = qcl::get_block('qtrProject', 'import_translations');
  return $output;
}
