$('#submit-btn').click(function(){
	var inputText = $('#text-input').val();
	if(inputText == "") {
		return;
	}
	//split input into array
	var input = inputText.split(" ");
	//process duration of each element in array
	duration_preprocess(input);
	//join array into string
	var mmlText = input.join(" ");
	//pass to generate_sound
	generate_sound(mmlText);
	
	$('#text-input').val("");
	//alert(inputText);
});

function duration_preprocess(textChunks){
	for (var i in textChunks) {
		switch(textChunks[i].length){
			case 0:
				break;
			case 1:
				textChunks[i] = "l16 " + textChunks[i];
				break;
			case 2:
				textChunks[i] = "l12 " + textChunks[i];
				break;
			case 3:
				textChunks[i] = "l8 " + textChunks[i];
				break;	
			case 4:
				textChunks[i] = "l6 " + textChunks[i];
				break;	
			case 5:
				textChunks[i] = "l4 " + textChunks[i];
				break;	
			case 6:
			case 7:
				textChunks[i] = "l3 " + textChunks[i];
				break;
			case 8:
			case 9:
				textChunks[i] = "l2 " + textChunks[i];
				break;		
			case 10:
			case 11:
				textChunks[i] = "l1 " + textChunks[i];
				break;
			default:
				textChunks[i] = "l1 " + textChunks[i] + "&" + textChunks[i];
				break;		
		}
	}
}

function generate_sound(input) {
	var gen = T("PluckGen", {env:{type:"perc", r:500, lv:0.4}}).play();
	T("mml", {mml:input}, gen).start();
}

