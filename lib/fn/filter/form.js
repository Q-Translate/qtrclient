(function($) {

  Drupal.behaviors.qtrClient_editor = {
    attach: function (context, settings) {

      var set_direction = function() {
        if ( $('#rtl-label').hasClass('disabled') )  return;
        if ( $('#edit-rtl')[0].checked ) {
          $('#edit-words').css('direction', 'rtl');
          $('#rtl-arrow')
            .removeClass('glyphicon-arrow-right')
            .addClass('glyphicon-arrow-left');
        }
        else {
          $('#edit-words').css('direction', 'ltr');
          $('#rtl-arrow')
            .removeClass('glyphicon-arrow-left')
            .addClass('glyphicon-arrow-right');
        }
      };

      set_direction();

      $('#edit-rtl').change(set_direction);

    }
  };

})(jQuery);
