<?php
/**
 * @file
 * Display news lists subscriptions.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Ding news lists'),
  'description' => t('Display news lists subscriptions.'),
  'single' => TRUE,
  'edit form' => 'ding_subscription_newslists_edit_form',
  'render callback' => 'ding_subscription_newslists_content_type_render',
  'required context' => new ctools_context_required(t('User'), 'user'),
  'category' => t('User'),
);

/**
 * Render callback function.
 */
function ding_subscription_newslists_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->content = '';

  if (isset($context->data)) {
    $account = $context->data;

    $block->content = 'BLA BLA BLA';
  }

  return $block;
}

/**
 * Plugin settings form.
 */
function ding_subscription_newslists_edit_form($form, &$form_state) {
  return $form;
}
