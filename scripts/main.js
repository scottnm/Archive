$('#submit-btn').click(function(){
	var inputText = $('#text-input').val();
	if(inputText == '') {
		return;
	}

	var textSplit = inputText.toLowerCase().split(' ');
	var mmlSplit = text_to_mml(textSplit);
	var mmlCmd = generate_mml_command(mmlSplit);
	var reverb = T('reverb', {room: 1 - Math.pow(Math.E, -1 * inputText.length / 1000), damp: mmlSplit.length/inputText.length, mix:0.2});
	generate_sound(mmlCmd, reverb);
	
	$('#text-input').val('');
	console.log(mmlCmd);
});

function generate_mml_command(mmlSplit) {
	var mmlCmd = '';
	mmlSplit.forEach(function(note) {
		var text = note.length + ' ' + note.note;
		if (note.modifiers.flat) {
			text += '-';
		} else if (note.modifiers.sharp) {
			text += '+';
		}
		
		if (note.modifiers.dot) {
			text += '.';
		}
		
		if (note.modifiers.pitchbend && !note.modifiers.repetition) {
			text += '*';
		} else if (note.modifiers.repetition) {
			text = '[ ' + text + ' ]';
		}
		
		if (!note.modifiers.pitchbend) {
			text += ' ';
		}
		
		mmlCmd += text + note.modifiers.rest;
	});
	return mmlCmd;
}

function MmlAttribute(initText) {
	this.initText = initText;
	this.length = get_duration(initText);
	this.modifiers = new Modifiers(initText);
	initText = filterText(initText);
	this.note = get_note(initText);
}

function filterText(text) {
	var regex = /[1234567890()!?={}\/&.'",:;jkqxz]/g;
	return text.replace(regex, '');
}


function text_to_mml(textArr) {
	var mmlArr = [];
	textArr.forEach(function(text){
		mmlArr.push(new MmlAttribute(text));
	});
	
	return mmlArr;
}
 
function get_note(text) {
	if (text === '') {
		return '';
	}
	
	var notes = 'abcdefghilmnoprstuvwy';
	var scaleNotes = 'cdefgab';
	var char = text[0];
	var index = notes.indexOf(char);
	var scaleNote = scaleNotes[index % 7];
	var octave = Math.floor(index / 7) + 2 + text.length % 3;
	var note = "o" + octave + " " + scaleNote;
	return note;
}

function Modifiers(text) {
	this.flat =       text.indexOf('x') != -1 && text.indexOf('j') == -1;
	this.sharp =      text.indexOf('j') != -1 && text.indexOf('x') == -1;
	this.dot =        text.indexOf('k') != -1;
	this.repetition = text.indexOf('q') != -1;
	this.pitchbend =  text.indexOf('z') != -1;
	this.rest =       get_rest(text);
}

function get_rest(text) {
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

function get_duration(text) {
	switch(text.length) {
		case 0:
			break;
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

function generate_sound(input, reverb) {
	var gen = T("OscGen", {wave:"saw", env:{type:"adsr", r:500}, mul:0.25}).play();
	T("mml", {mml:input}, reverb, gen).on("ended", function() {
		gen.pause();
		this.stop();
	}).start();
}

