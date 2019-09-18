<?php
/**
 * @file
 * ding_subscription.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ding_subscription_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}
