(function($) {

  Drupal.behaviors.qtrClient_editor = {
    attach: function (context, settings) {

      $('#edit-direction').change(function() {
        if ( $(this)[0].checked ) {
          $('#edit-words').css('direction', 'rtl');
          $('#edit-direction').next().removeClass('glyphicon-arrow-right').addClass('glyphicon-arrow-left');
        }
        else {
          $('#edit-words').css('direction', 'ltr');
          $('#edit-direction').next().removeClass('glyphicon-arrow-left').addClass('glyphicon-arrow-right');
        }
      });

    }
  };

})(jQuery);
