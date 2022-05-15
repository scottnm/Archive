"use strict";

function RadioSeed(artist, imgUrl, id) {
	this.artist = ko.observable(artist);
	this.id = id;
	this.imgUrl = ko.observable(imgUrl);
	this.tracks = [];
	this.strength = ko.observable('strength-1');
	this.strengthNum = ko.computed(function(){
		return Number(this.strength()[this.strength().length - 1]);
	}, this);
}