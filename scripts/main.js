$('#submit-btn').click(function(){
	var inputText = $('#text-input').val();
	generate_sound(inputText);
	//alert(inputText);
});

function generate_sound(input) {
	var gen = T("PluckGen", {env:{type:"perc", r:500, lv:0.4}}).play();
	T("mml", {mml:input}, gen).start();
}