$('#submit-btn').click(function(){
	var inputText = $('#text-input').val();
	if(inputText == '') {
		return;
	}

	var textSplit = inputText.split(' ');
	var mmlSplit = text_to_mml(textSplit);
	var mmlCmd = generate_mml_command(mmlSplit);
	generate_sound(mmlCmd);
	
	$('#text-input').val('');
	console.log(mmlCmd);
});

function generate_mml_command(mmlSplit) {
	var mmlCmd = '';
	mmlSplit.forEach(function(note) {
		var text = note.length + ' ' + note.octave + ' ' + note.note;
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
		
		mmlCmd += text;
	});
	return mmlCmd;
}

function MmlAttribute(initText) {
	this.initText = initText;
	this.length = get_duration(initText);
	this.note = get_note(initText);
	this.octave = get_octave(initText);
	this.modifiers = new Modifiers(initText);
}


function text_to_mml(textArr) {
	var mmlArr = [];
	textArr.forEach(function(text){
		mmlArr.push(new MmlAttribute(text));
	});
	
	return mmlArr;
}
 
function get_note(text) {
	return 'a';
}

function get_octave(text) {
	return 'o4';
}

function Modifiers(text) {
	this.flat =       text.indexOf('x') != -1 && text.indexOf('j') == -1;
	this.sharp =      text.indexOf('j') != -1 && text.indexOf('x') == -1;
	this.dot =        text.indexOf('k') != -1;
	this.repetition = text.indexOf('q') != -1;
	this.pitchbend =  text.indexOf('z') != -1;
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

function generate_sound(input) {
	var gen = T('PluckGen', {env:{type:'perc', r:500, lv:0.4}}).play();
	T('mml', {mml:input}, gen).start();
}

