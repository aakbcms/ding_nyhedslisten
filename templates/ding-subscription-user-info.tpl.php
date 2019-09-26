<?php
/**
 * @file
 * Default template file for ding-subscription-user-info theme function.
 */

$t=1;
?>
<ul>
  <?php foreach ($items as $item): ?>
  <li>
    <strong><?php echo $item['label'] ?></strong>:
    <span class="<?php echo $item['status'] ? 'subscribed' : 'not-subscribed' ?>">
      <?php echo empty($item['selection']) ?  t('Not subscribed') : implode(', ', $item['selection'])?>
    </span>
  </li>
  <?php endforeach; ?>
</ul>
