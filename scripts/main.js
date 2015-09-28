/*
 * Different average word sizes for different blocks of text should handle
 * generating music slightly different
 */
var avg_word_size = 5;


/*
 * Main code
 * processes a string of text into separate MML words and then 
 * generates audio from these MML words
 */
$('#submit-btn').click(function(){
	var text_area = $('#text-input').val();
	if(text_area == '') {
		return;
	}

	var split_text = text_area.toLowerCase().split(' ');
	avg_word_size = findAverageWordSize(split_text);
	var mml_commands = textToMml(split_text);
	var mml = generateMmlCommand(mml_commands);
	generateSound(mml, 1 - Math.pow(Math.E, -1 * text_area.length / 1000), mml_commands.length/text_area.length, 0.75);
	
	$('#text-input').val('');
	console.log(mml);//console.log("wordsize: %d\nrooms: %f\ndamp: %f\n", avg_word_size, 1 - Math.pow(Math.E, -1 * inputText.length / 1000), mmlSplit.length/inputText.length);
});


/**
 * Finds the average length of strings within an array of strings
 * @param text_arr The array of strings
 * @returns the average length
 */
function findAverageWordSize(text_arr) {
	var sum = 0;
	text_arr.forEach(function(text) {
		sum += text.length;
	});
	return sum / text_arr.length;
}

/**
 * Given an array of MmlAttribute objects, generate a unified mml string
 * @param mml_arr The array of MmlAttribute objects
 * @returns the mml command string
 */
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

/**
 * @constr
 * @param init_text a string of text that will become an mml command
 * 					e.g. 'apple' becomes an mml object of 'l4 o4 c'
 */
function MmlAttribute(init_text) {
	this.init_text = init_text;
	this.length = getDuration(init_text);
	this.modifiers = new Modifiers(init_text);
	init_text = filterWord(init_text);
	this.note = getNote(init_text);
}


/**
 * Removes numbers, punctuation, whitespace, and reserved alphabet
 * characters from a string
 * @param text The text to filter
 */
function filterWord(mml_word) {
	var alphanum_regex = /[1234567890jkqxz]/g;
	var special_regex = /[\W+]/g
	return mml_word.replace(alphanum_regex, '').replace(special_regex, '');
}


/**
 * Generate an array of Mml objects from an array of words
 * @param text_arr the array of strings/words
 */
function textToMml(text_arr) {
	var mml_arr = [];
	text_arr.forEach(function(text){
		mml_arr.push(new MmlAttribute(text));
	});
	
	return mml_arr;
}
 

 /**
  * Given a word/string, figure out what note should be played
  * e.g. 'apple' returns fourth octave's C
  */
function getNote(mml_word) {
	if (mml_word === '') {
		return '';
	}
	
	var note_map = 'abcdefghilmnoprstuvwy';
	var scale = 'cdefgab';

	var index = note_map.indexOf(mml_word[0]);
	var scale_note = scale[index % 7];
	var octave = Math.floor(index / 7) + 2 + mml_word.length % 3;
	return 'o' + octave + ' ' + scale_note;
}


/**
 * @constr
 * @struct
 * contains special modifiers on a note
 * @param mml_word The text which contains the special modifying characters
 * 		       that need to be translated into mml
 */
function Modifiers(mml_word) {
	this.flat =       mml_word.indexOf('x') != -1 && mml_word.indexOf('j') == -1;
	this.sharp =      mml_word.indexOf('j') != -1 && mml_word.indexOf('x') == -1;
	this.dot =        mml_word.indexOf('k') != -1;
	this.repetition = mml_word.indexOf('q') != -1;
	this.pitchbend =  mml_word.indexOf('z') != -1;
	this.rest =       getRest(mml_word);
}

/**
 * get the rest mml string from an input text
 * @param text the string to process
 */
function getRest(mml_word) {
	if (mml_word.indexOf('.') != -1 || mml_word.indexOf('!') != -1 || mml_word.indexOf('?') != -1) {
		return 'l2 r ';
	} else if (mml_word.indexOf(':') != -1) {
		return 'l4 r ';
	} else if (mml_word.indexOf(';') != -1) {
		return 'l8 r ';
	} else if (mml_word.indexOf(',') != -1) {
		return 'l16 r ';
	} else {
		return ' ';
	}
}


/**
 * find the duration that this text's mml note should be played
 * @param mml_word The word to process
 */
function getDuration(mml_word) {
	if (mml_word.length > 30) {
		switch(Math.floor(mml_word.length/50)) {
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
	} else if (mml_word.length > 10) {
		switch(mml_word.length) {
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
	} else {
		switch(mml_word.length) {
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
}

/**
 * Generate the sound for an mml string
 * @param input The input mml strings
 * @param reverb a TObject that specifies what reverb the mml string should be played with
 */
function generateSound(input, reverb_room_size, reverb_damp, reverb_mix) {
	var reverb = T('reverb', {
		room: reverb_room_size,
		damp: reverb_damp,
		mix: reverb_damp}
	);

	var gen = T('PluckGen', {
			wave: 'saw',
			env: {
				type: 'adsr',
				r:500
			}, 
			mul:0.25
		}, reverb).play();

	T('mml', {mml: input}, gen).on('ended', function() {
		gen.pause();
		this.stop();
	}).start();
}

