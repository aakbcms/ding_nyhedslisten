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
    $client = $this->getClient();
    $listsService = new HLLists($client);

    $response = $listsService->getList($listId);
    if (array_key_exists('response', $response)) {
      $list = $this->jsonDecode($response['response']);
    }

    return $list ?? NULL;
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
   * Add a new member to list.
   *
   * @param $listId
   *   The ID of the list to add the member too.
   * @param $fields
   *   The fields for the member on the list.
   *
   * @return mixed
   *
   *
   * @throws \HLErrorException
   *   If error is return from Heyloyalty.
   */
  public function addMember($listId, $fields) {
    $client = $this->getClient();
    $memberService = new HLMembers($client);

    $response = $memberService->create($listId, $fields);

    // TODO: handle member exists error.

    return $this->jsonDecode($response['response']);
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
 * Class HLErrorException
 */
class HLErrorException extends \Exception {}
