<?php
/**
 * @file
 * Defines the administration interface for the module.
 */

/**
 * Defines the administration settings form.
 */
function ding_nyhedslisten_admin_settings_form($form, &$form_state) {
  $form = [
    '#tree' => TRUE,
  ];

  $default = variable_get('ding_nyhedslisten_settings', []);
  $default = $default + [
    'api' => [
      'key' => '',
      'secret' => '',
    ],
    'list' => [],
  ];

  $form['ding_nyhedslisten_settings'] = [
    '#type' => 'fieldset',
    '#title' => t('Nyhedslisten configuration'),
  ];

  $form['ding_nyhedslisten_settings']['api']['key'] = [
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#required' => TRUE,
    '#default_value' => $default['api']['key'],
  ];

  $form['ding_nyhedslisten_settings']['api']['secret'] = [
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#required' => TRUE,
    '#default_value' => $default['api']['secret'],
  ];

  if (!empty($default['api']['key'])) {
    $hl = new HeyloyaltyClient($default['api']['key'], $default['api']['secret']);
    $lists = $hl->getListNames();

    $form['ding_nyhedslisten_settings']['list'] = [
      '#type' => 'select',
      '#title' => t('Lists'),
      '#description' => t('Select list to use'),
      '#options' => [0 => t('- Select list -')] + $lists,
      '#default_value' => $default['list'],
    ];

    //$t0 = $hl->getList($default['list']);
    //$t = $hl->addMember($default['list'], array('email' => 'test@aarhus.dk', 'kategori' => [74586, 74587]));
  }

  return system_settings_form($form);
}
