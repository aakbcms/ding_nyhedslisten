<?php
/**
 * @file
 * Wrapper class for the Heyloyalty client library.
 */

require_once drupal_get_path('module', 'ding_subscription') . '/vendor/autoload.php';

use Phpclient\HLClient;
use Phpclient\HLLists;
use Phpclient\HLMembers;

/**
 * Class HeyloyaltyClient.
 *
 * Wrapper class for communicating with Heyloyalty.
 */
class HeyloyaltyClient {

  private $apiKey = '';
  private $apiSecret = '';
  private $client = NULL;

  /**
   * HeyloyaltyClient constructor.
   *
   * @param $apiKey
   * @param $apiSecret
   */
  public function __construct($apiKey, $apiSecret) {
    $this->apiKey = $apiKey;
    $this->apiSecret = $apiSecret;

    $this->getClient();
  }

  /**
   * Get all lists.
   *
   * @param bool $reset
   *   If TRUE refresh static cache.
   *
   * @return mixed
   *   Array with list objects.
   *
   * @throws \HLErrorException
   *   If error is return from Heyloyalty.
   */
  public function getLists($reset = FALSE) {
    static $lists;

    if (!isset($lists) || $reset) {
      $client = $this->getClient();
      $listsService = new HLLists($client);

      $response = $listsService->getLists();
      if (array_key_exists('response', $response)) {
        $lists = $this->jsonDecode($response['response']);
      }
    }

    return $lists;
  }

  /**
   * Get list.
   *
   * @param $listId
   *   ID of the list to get.
   *
   * @return mixed|null
   *   The list object.
   *
   * @throws \HLErrorException
   *   If error is return from Heyloyalty.
   */
  public function getList($listId) {
    static $lists;
    $lists = is_array($lists) ? $lists : [];

    if (!isset($lists[$listId])) {
      $client = $this->getClient();
      $listsService = new HLLists($client);

      $response = $listsService->getList($listId);
      if (array_key_exists('response', $response)) {
        $list = $this->jsonDecode($response['response'], TRUE);
      }

      $lists[$listId] = $list ?? NULL;
    }

    return $lists[$listId];
  }

  /**
   * List names indexed by list ID.
   *
   * @return array
   *   List names indexed by list ID.
   *
   * @throws \HLErrorException
   *   If error is return from Heyloyalty.
   */
  public function getListNames() {
    $lists = $this->getLists();
    return array_column($lists, 'name', 'id');
  }

  /**
   * Get a members object on a given list.
   *
   * @param $listId
   *   The ID of the list to get member for.
   * @param $mail
   *   The mail address for the member to look-up.
   *
   * @return mixed|null
   *   The member object with information or NULL if not found.
   *
   * @throws \HLErrorException
   *   If error is return from Heyloyalty.
   */
  public function getMember($listId, $mail) {
    $client = $this->getClient();
    $memberService = new HLMembers($client);

    $response = $memberService->getMemberByEmail($listId, $mail);
    if (array_key_exists('response', $response)) {
      $res = $this->jsonDecode($response['response'], TRUE);
    }

    return $res['members'][0] ?? NULL;
  }

  /**
   * Update member information.
   *
   * If the member don't exists it will be created.
   *
   * @param $mail
   *   The users mail.
   * @param $name
   *   The name of the user (used to create the user).
   * @param $listId
   *   The list to update the user at.
   * @param array $fields
   *   The fields and values to update.
   *
   * @return bool|mixed
   *   JSON decoded array.
   *
   * @throws \HLErrorException
   *   If error is return from Heyloyalty.
   */
  public function updateMember($mail, $name, $listId, array $fields) {
    $client = $this->getClient();
    $memberService = new HLMembers($client);
    $member = $this->getMember($listId, $mail);

    // Hack to handle empty multi-value fields.
    foreach ($fields as $field => $value) {
      if (empty($value)) {
        unset($fields[$field]);
        $fields[$field . '[]'] = '';
      }
    }

    if (is_null($member)) {
      // create a member on a list
      $fields += [
        'firstname' => $name,
        'email' => $mail,
      ];
      $response = $memberService->create($listId, $fields);
      if (array_key_exists('response', $response)) {
        $res = $this->jsonDecode($response['response']);
        if (array_key_exists('id', $res)) {
          return TRUE;
        }
        else {
          throw new HLErrorException('Unknown error in creating user subscriptions');
        }
      }
    }
    else {
      $response = $memberService->patch($listId, $member['id'], $fields);
      if (array_key_exists('response', $response) && !empty($response['response'])) {
        return $this->jsonDecode($response['response']);
      }
    }

    return TRUE;
  }

  /**
   * Decode json string from Heyloyalty.
   *
   * @param $string
   *   JSON encoded string
   * @param bool $assoc
   *   IF TRUE, returned objects will be converted into associative arrays.
   *   Default FALSE.
   *
   * @return mixed
   *   Decoded result.
   *
   * @throws \HLErrorException
   *   If error is return from Heyloyalty.
   */
  private function jsonDecode($string, $assoc = FALSE) {
    $json = json_decode($string, $assoc);

    if (array_key_exists('error', $json)) {
      if ($assoc) {
        $error = $json['error'];
      }
      else {
        $error = $json->error;
      }
      throw new HLErrorException($error);
    }

    return $json;
  }

  /**
   * Get client to communicate with Heyloyalty.
   *
   * @return \Phpclient\HLClient|null
   */
  private function getClient() {
    if (is_null($this->client)) {
      $this->client = new HLClient($this->apiKey, $this->apiSecret);
    }

    return $this->client;
  }
}

/**
 * Class HLErrorException.
 *
 * Used to indicate an error at the server or an error message was return for
 * a give request.
 */
class HLErrorException extends \Exception {}
