<?php
/**
 * @file
 * Defines the administration interface for the module.
 */

/**
 * Defines the administration settings form.
 */
function ding_subscription_admin_settings_form($form, &$form_state) {
  $form = [
    '#tree' => TRUE,
  ];

  $default = variable_get('ding_subscription_settings', []);
  $default = $default + [
    'api' => [
      'key' => '',
      'secret' => '',
    ],
    'lists' => [],
  ];

  $form['ding_subscription_settings'] = [
    '#type' => 'fieldset',
    '#title' => t('Subscription configuration'),
  ];

  $form['ding_subscription_settings']['api']['key'] = [
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#required' => TRUE,
    '#default_value' => $default['api']['key'],
  ];

  $form['ding_subscription_settings']['api']['secret'] = [
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#required' => TRUE,
    '#default_value' => $default['api']['secret'],
  ];

  if (!empty($default['api']['key'])) {
    $hl = new HeyloyaltyClient($default['api']['key'], $default['api']['secret']);
    $lists = $hl->getListNames();

    $form['ding_subscription_settings']['lists'] = [
      '#type' => 'fieldset',
      '#title' => t('List selection'),
      '#prefix' => '<div id="lists-fieldset-wrapper">',
      '#suffix' => '</div>',
    ];

    if (empty($form_state['num_lists'])) {
      $form_state['num_lists'] = count($default['lists']);
    }

    for ($i = 0; $i < $form_state['num_lists']; $i++) {
      $form['ding_subscription_settings']['lists'][$i] = [
        '#type' => 'fieldset',
        '#title' => t('List !id', array('!id' => $i + 1)),
      ];

      $form['ding_subscription_settings']['lists'][$i]['id'] = [
        '#type' => 'select',
        '#title' => t('Lists'),
        '#description' => t('Select list to use'),
        '#options' => [0 => t('- Select list -')] + $lists,
        '#default_value' => $default['lists'][$i]['id'],
      ];

      if ($default['lists'][$i]['id'] != 0) {
        $list = $hl->getList($default['lists'][$i]['id']);
        $options = array_column($list['fields'], 'label', 'id');
        $form['ding_subscription_settings']['lists'][$i]['field'] = [
          '#type' => 'select',
          '#title' => t('Field'),
          '#description' => t('Select field(s) to use'),
          '#options' => [0 => t('- Select field -')] + $options,
          '#default_value' => $default['lists'][$i]['field'] ?? 0,
        ];

        $fields = array_column($list['fields'], null, 'id');
        if (isset($default['lists'][$i]['field']) && isset($fields[$default['lists'][$i]['field']]) && array_key_exists('options', $fields[$default['lists'][$i]['field']])) {
          $form['ding_subscription_settings']['lists'][$i]['types'] = [
            '#type' => 'select',
            '#title' => t('Field'),
            '#description' => t('Select field to use'),
            '#options' => $fields[$default['lists'][$i]['field']]['options'],
            '#multiple' => TRUE,
            '#default_value' => $default['lists'][$i]['types'] ?? [],
          ];

          $form['ding_subscription_settings']['lists'][$i]['label'] = [
            '#type' => 'textfield',
            '#title' => t('Label'),
            '#default_value' => $default['lists'][$i]['label'] ?? '',
          ];

          $form['ding_subscription_settings']['lists'][$i]['description'] = [
            '#type' => 'textarea',
            '#title' => t('Description'),
            '#default_value' => $default['lists'][$i]['description'] ?? '',
            '#rows' => 5,
          ];
        }
      }
    }

    $form['ding_subscription_settings']['lists']['add-list'] = array(
      '#type' => 'submit',
      '#value' => t('Add one more'),
      '#submit' => array(
        'ding_subscription_admin_settings_form_add_one',
      ),
      '#ajax' => array(
        'callback' => 'ding_subscription_admin_settings_form_add_one_callback',
        'method' => 'replace',
        'wrapper' => 'lists-fieldset-wrapper',
      ),
    );
  }

  $form['#submit'][] = 'ding_subscription_admin_settings_form_submit';

  return system_settings_form($form);
}

function ding_subscription_admin_settings_form_add_one($form, &$form_state) {
  $form_state['num_lists']++;
  $form_state['rebuild'] = TRUE;
}

function ding_subscription_admin_settings_form_add_one_callback($form, $form_state) {
  return $form['ding_subscription_settings']['lists'];
}

function ding_subscription_admin_settings_form_submit($form, &$form_state) {
  // Remove non selected list configurations.
  foreach ($form_state['values']['ding_subscription_settings']['lists'] as $id => $data) {
    if (isset($data['id']) && $data['id'] == '0') {
      unset($form_state['values']['ding_subscription_settings']['lists'][$id]);
    }
  }

  // Fix indexes.
  $form_state['values']['ding_subscription_settings']['lists'] = array_values($form_state['values']['ding_subscription_settings']['lists']);

  // Remove add one more button value.
  array_pop($form_state['values']['ding_subscription_settings']['lists']);
}
