var loading_div = $('#loading_div')
var loaded_div = $('#loaded_div')

// default argument
function load_midi() {
    start_loading_handler();

    console.log("this just started");
    MIDI.Player.loadFile(midi_location, stop_loading_handler);
    console.log("second line of execution")
}

function start_loading_handler(){
    loading_div.hide();
    loaded_div.hide();
}
function stop_loading_handler(){
    console.log("this is working")
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
