var avg_word_size = 5;

$('#submit-btn').click(function(){
	var text_area = $('#text-input').val();
	if(text_area == '') {
		return;
	}

	var split_text = text_area.toLowerCase().split(' ');
	avg_word_size = findAverageWordSize(split_text);
	var mml_commands = textToMml(split_text);
	var mml = generateMmlCommand(mml_commands);
	var reverb = T('reverb', {room: 1 - Math.pow(Math.E, -1 * text_area.length / 1000), damp: mml_commands.length/text_area.length, mix:0.75});
	generateSound(mml, reverb);
	
	$('#text-input').val('');
	console.log(mml);//console.log("wordsize: %d\nrooms: %f\ndamp: %f\n", avg_word_size, 1 - Math.pow(Math.E, -1 * inputText.length / 1000), mmlSplit.length/inputText.length);
});

function findAverageWordSize(text_arr) {
	var sum = 0;
	text_arr.forEach(function(text) {
		sum += text.length;
	});
	return sum / text_arr.length;
}

function generateMmlCommand(mml_arr) {
	var mml_cmd = '';
	mml_arr.forEach(function(note) {
		var mml_text = note.length + ' ' + note.note;
		if (note.modifiers.flat) {
			mml_text += '-';
		} else if (note.modifiers.sharp) {
			mml_text += '+';
		}
		
		if (note.modifiers.dot) {
			mml_text += '.';
		}
		
		if (note.modifiers.pitchbend && !note.modifiers.repetition) {
			mml_text += '*';
		} else if (note.modifiers.repetition) {
			mml_text = '[ ' + mml_text + ' ]';
		}
		
		if (!note.modifiers.pitchbend) {
			mml_text += ' ';
		}
		mml_cmd += mml_text + note.modifiers.rest;
	});
	return mml_cmd;
}

function MmlAttribute(init_text) {
	this.init_text = init_text;
	if (avg_word_size > 30) {
		this.length = getDuration30(init_text);
	} else if (avg_word_size > 10) {
		this.length = getDuration10(init_text);
	} else {
		this.length = getDuration(init_text);
	}
	this.modifiers = new Modifiers(init_text);
	init_text = filterText(init_text);
	this.note = getNote(init_text);
}

function filterText(text) {
	var alphanum_regex = /[1234567890jkqxz]/g;
	var special_regex = /[\W+]/g
	return text.replace(alphanum_regex, '').replace(special_regex, '');
}


function textToMml(text_arr) {
	var mml_arr = [];
	text_arr.forEach(function(text){
		mml_arr.push(new MmlAttribute(text));
	});
	
	return mml_arr;
}
 
function getNote(text) {
	if (text === '') {
		return '';
	}
	
	var note_map = 'abcdefghilmnoprstuvwy';
	var scale = 'cdefgab';
	var index = note_map.indexOf(text[0]);
	var scale_note = scale[index % 7];
	var octave = Math.floor(index / 7) + 2 + text.length % 3;
	var full_note = "o" + octave + " " + scale_note;
	return full_note;
}

function Modifiers(text) {
	this.flat =       text.indexOf('x') != -1 && text.indexOf('j') == -1;
	this.sharp =      text.indexOf('j') != -1 && text.indexOf('x') == -1;
	this.dot =        text.indexOf('k') != -1;
	this.repetition = text.indexOf('q') != -1;
	this.pitchbend =  text.indexOf('z') != -1;
	this.rest =       getRest(text);
}

function getRest(text) {
	if (text.indexOf('.') != -1 || text.indexOf('!') != -1 || text.indexOf('?') != -1) {
		return 'l2 r ';
	} else if (text.indexOf(':') != -1) {
		return 'l4 r ';
	} else if (text.indexOf(';') != -1) {
		return 'l8 r ';
	} else if (text.indexOf(',') != -1) {
		return 'l16 r ';
	} else {
		return ' ';
	}
}

function getDuration(text) {
	switch(text.length) {
		case 0:
			return '';
		case 1:
			return 'l16'
		case 2:
			return 'l12';
		case 3:
			return 'l8';
		case 4:
			return 'l6';
		case 5:
			return 'l4';
		case 6:
		case 7:
			return 'l3';
		case 8:
		case 9:
			return 'l2';
		default:
			return 'l1';
	}
}

function getDuration10(text) {
	switch(text.length) {
		case 0:
			return '';
		case 1:
		case 2:
			return 'l64'
		case 3:
		case 4:
			return 'l32';
		case 5:
		case 6:
			return 'l16';
		case 7:
			return 'l12';
		case 8:
			return 'l8';
		case 9:
			return 'l6';
		case 10:
			return 'l4';
		case 11:
		case 12:
		case 13:
			return 'l3';
		default:
			return 'l2';
	}
}

function getDuration30(text) {
	if (Math.floor(text.length/10) > 13) debugger;
	switch(Math.floor(text.length/50)) {
		case 0:
			return '';
		case 1:
		case 2:
			return 'l64'
		case 3:
		case 4:
			return 'l32';
		case 5:
		case 6:
			return 'l16';
		case 7:
			return 'l12';
		case 8:
			return 'l8';
		case 9:
			return 'l6';
		case 10:
			return 'l4';
		case 11:
		case 12:
		case 13:
			return 'l3';
		default:
			return 'l2';
	}
}

function generateSound(input, reverb) {
	var gen = T("PluckGen", {wave:"saw", env:{type:"adsr", r:500}, mul:0.25}, reverb).play();
	T("mml", {mml:input}, gen).on("ended", function() {
		gen.pause();
		this.stop();
	}).start();
}

