var loading_div = $('#loading_div')
var loaded_div = $('#loaded_div')

// default argument
function load_midi() {
    start_loading_handler();

    MIDI.Player.loadfile('../mozart_midi.mid', stop_loading_handler);
}

function start_loading_handler(){
    loading_div.hide();
    loaded_div.hide();
    $('#play_midi').prop('disabled', true);
}
function stop_loading_handler(){
    loaded_div.show();
    loading_div.hide();
    $('#play_midi').prop('disabled',false);
    setTimeout(function(){
        loaded_div.hide();
    },5000);
}

$("#play_midi").click(function() {
    MIDI.Player.start()
});
