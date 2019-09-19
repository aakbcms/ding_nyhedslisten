<?php
/**
 * @file
 * ding_subscription.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function ding_subscription_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'user_subscriptions';
  $page->task = 'page';
  $page->admin_title = 'User subscriptions';
  $page->admin_description = '';
  $page->path = 'user/%user/subscriptions';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'external_user',
        'settings' => NULL,
        'context' => array(
          0 => 'argument_entity_id:user_1',
        ),
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'compare_users',
        'settings' => array(
          'equality' => '1',
        ),
        'context' => array(
          0 => 'argument_entity_id:user_1',
          1 => 'logged-in-user',
        ),
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'My subscriptions',
    'name' => 'navigation',
    'weight' => '8',
    'parent' => array(
      'type' => 'tab',
      'title' => 'Min side',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'user' => array(
      'id' => 1,
      'identifier' => 'User: ID',
      'name' => 'entity_id:user',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_user_subscriptions__panel_context_b1e1fbe2-9d6e-4f0d-aaf8-c61ae95cb727';
  $handler->task = 'page';
  $handler->subtask = 'user_subscriptions';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(),
      'logic' => 'and',
    ),
    'metatag_panels' => array(
      'enabled' => 1,
      'metatags' => array(
        'title' => array(
          'value' => 'Debts | [current-page:pager][site:name]',
        ),
      ),
    ),
    'name' => '',
  );
  $display = new panels_display();
  $display->layout = '25_50_25';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
      'left' => NULL,
      'right' => NULL,
      'left_sidebar' => NULL,
      'main_content' => NULL,
      'right_sidebar' => NULL,
      'attachment_4_1' => NULL,
      'attachment_4_2' => NULL,
      'attachment_4_3' => NULL,
      'attachment_4_4' => NULL,
      'attachment_3_1' => NULL,
      'attachment_3_2' => NULL,
      'attachment_3_3' => NULL,
      'attachment_2_1' => NULL,
      'attachment_2_2' => NULL,
      'attachment_1_1' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '7df426ee-6800-4124-b929-4391f02d19bd';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-bc35a6a8-ac4b-4d43-a9ce-5530d19e355f';
    $pane->panel = 'left_sidebar';
    $pane->type = 'user_menu';
    $pane->subtype = 'user_menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_entity_id:user_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'bc35a6a8-ac4b-4d43-a9ce-5530d19e355f';
    $display->content['new-bc35a6a8-ac4b-4d43-a9ce-5530d19e355f'] = $pane;
    $display->panels['left_sidebar'][0] = 'new-bc35a6a8-ac4b-4d43-a9ce-5530d19e355f';
    $pane = new stdClass();
    $pane->pid = 'new-fa468761-b092-4b33-bcb2-82329669ae91';
    $pane->panel = 'main_content';
    $pane->type = 'newslists';
    $pane->subtype = 'newslists';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_entity_id:user_1',
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'fa468761-b092-4b33-bcb2-82329669ae91';
    $display->content['new-fa468761-b092-4b33-bcb2-82329669ae91'] = $pane;
    $display->panels['main_content'][0] = 'new-fa468761-b092-4b33-bcb2-82329669ae91';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['user_subscriptions'] = $page;

  return $pages;

}
