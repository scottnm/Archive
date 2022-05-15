"use strict";

var model = new RadioListModel();

function RadioListModel() {
	var self = this;
	self.artistInput = ko.observable();
	self.newRadioSeeds = ko.observableArray([]);
	self.newSeedPromises = [];
	self.oldRadioSeeds = ko.observableArray([]);
	self.radioSeeds = ko.computed(function(){
		return this.newRadioSeeds().concat(this.oldRadioSeeds());
	}.bind(self));
	
	self.totalStrength = ko.computed(function() {
		var sum = 0;
		self.radioSeeds().forEach(function(seed) {
			sum += seed.strengthNum();
		});
		return sum;
	});
}

/**
 * add a seed from the input fields to the seedlist
 * pre: input field must be filled in
 */
RadioListModel.prototype.addSeed = function() {
	// ensure that input is given
	if(!this.artistInput()) {
		return;
	}
	
	$.getJSON(spotifyArtistGetUrl(this.artistInput()), function(data){
		// ensure valid search query
		if (data['artists']['items'].length == 0) {
			showErrorToast(this.artistInput() + ' returned no results');
			return;
		}

		// ensure no duplicates
		var entryId = data['artists']['items'][0]['id'];
		for(var i in this.radioSeeds()) {
			var seed = this.radioSeeds()[i];
			if(seed.id === entryId) {
				showErrorToast(this.artistInput() + ' is a duplicate entry');
				return;
			}
		}

		var artistName = data['artists']['items'][0]['name'];
		var imgUrl = data['artists']['items'][0]['images'][0]['url'];
		var id = data['artists']['items'][0]['id'];
		var newSeed = new RadioSeed(artistName, imgUrl, id);
		this.newRadioSeeds.push(newSeed);
		// have to capture the total strength within this closure
		var totalStrength = this.totalStrength();
		// a promise is used so that track data can be loaded now and used later
		// during playlist generation for better performance.
		var seedPromise = new Promise(function(resolve, reject){
			$.getJSON(echonestArtistPlaylistGetUrl(this.artist(), 15),
			          processEchonestResponse.bind(this, resolve, totalStrength));
		}.bind(newSeed));
		this.newSeedPromises.push(seedPromise);
		this.artistInput('');
	}.bind(this));
};

/**
 * remove a seed from the seed list
 * triggered from seed's delete button
 * @param seed The seed to remove
 */
RadioListModel.prototype.removeSeed = function(seed) {
	this.newRadioSeeds.remove(seed);
	this.oldRadioSeeds.remove(seed);
}.bind(model);

/**
 * Take the seedlist and generate a radio-like playlist from it
 */
RadioListModel.prototype.generateRadio = function() {
	console.log('%d Generating radio for...', this.totalStrength());
	$('#loading-animation').removeClass('hidden'); // toggle off
	var totalStrength = this.totalStrength();
	var playlist = [];
	// copy the array so to prevent sync issues
	var seedPromises = [].concat(this.newSeedPromises);
	this.newSeedPromises = [];
	
	/**
	 * for each seed generate a promise that sends its sliced tracklist
	 * to the resolve
	 */
	this.oldRadioSeeds().forEach(function(seed){
		var seedPromise = new Promise(function(resolve, reject){
			$.getJSON(echonestArtistPlaylistGetUrl(seed.artist(), 15),
					  processEchonestResponse.bind(seed, totalStrength));
		});
		seedPromises.push(seedPromise);
	});
	
	/**
	 * When all api calls have come back, concatenate all of the tracklists
	 * into a playlist
	 */
	Promise.all(seedPromises)
		.then(function(tracklists) {
			tracklists.forEach(function(tracklist) {
				playlist = playlist.concat(tracklist);
			});
			
			// move the newSeeds to the oldSeeds list to prevent duplication
			this.oldRadioSeeds(this.oldRadioSeeds().concat(this.newRadioSeeds()));
			this.newRadioSeeds([]);
			
			playlist.sort(shuffleHelper);
			$('#loading-animation').addClass('hidden'); // toggle on
			console.log('Here is the finished playlist');
			logTracks(playlist);
			constructSpotifyController(playlist);
		}.bind(this));
};

/**
 * updates radio seeds strength when the strengthbar is clicked
 * in the view.
 * @param seed the seed to update
 * @event the browser event that contains the target of the click
 */
RadioListModel.prototype.updateStrength = function(seed, event) {
	if (event.target.classList.contains('strength-level') ||
		event.target.classList.contains('strength-bar-pad')) {
		seed.strength(event.target.classList[1]);
	}
};

ko.applyBindings(model);