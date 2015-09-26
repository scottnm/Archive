$('#submit-btn').click(function(){
	var inputText = $('#text-input').val();
	if(inputText != "") {
		generate_sound(inputText);
	}
	$('#text-input').val("");
	//alert(inputText);
});

function generate_sound(input) {
	var gen = T("PluckGen", {env:{type:"perc", r:500, lv:0.4}}).play();
	T("mml", {mml:input}, gen).start();
}