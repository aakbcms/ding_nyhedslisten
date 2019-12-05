/**
 * @file
 * Handle select2 for category selection on subscriptions
 */

(function($) {
  "use strict";

  $(document).ready(function() {
    $('.category-select').select2({width: '100%', placeholder: 'Klik her for at finde kategorier'})
  });

}(jQuery));
