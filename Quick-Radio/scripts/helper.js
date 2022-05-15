"use strict";
// used to prevent caching asynchronous calls
$.ajaxSetup({ cache: false });

var config = {
    echonestKey: 'SIKZGGF4TKV8DL8TE',
};

/**
 *  Returns a seed to shuffle a list by sorting
 *  with the returned random seed
 */
function shuffleHelper() {
    return 0.5 - Math.random();
}

/**
 * Returns the appropriate url to search for an artist
 * given an artist name
 * @param artistName the name of the artist
 */
function spotifyArtistGetUrl(artistName) {
    return 'https://api.spotify.com/v1/search?q=' +
        artistName.replace(/[' ']/g, '%20') + '&type=artist';
}

/**
 * Returns the appropriate url to search for a tracklist
 * using an artist as the radio seed
 * @param artistName the name of the artist
 * @param numSongs the number of songs to fetch from the echonest api
 */
function echonestArtistPlaylistGetUrl(artistName, numSongs) {
    return 'http://developer.echonest.com/api/v4/playlist/static?api_key=' +
            config.echonestKey+ '&artist=' + artistName +
            '&format=json&results=' + numSongs +
            '&type=artist-radio&bucket=id:spotify&bucket=tracks&limit=true';

}

/**
 * Fades the error toast on the page in and out of view
 */
function showErrorToast(msg) {
    $('#error-toast').text(msg)
        .fadeIn(500).delay(2000).fadeOut(500);
}

/**
 * Given a list of echonest track objects, prints them out
 */
function logTracks(tracklist) {
	var trackNum = 1;
	tracklist.forEach(function(entry){
		console.log('Track  #%d\t%s - %s', trackNum, entry.artist_name, entry.title);
		trackNum += 1;
	});
}

/**
 * @param promiseResolution the resolve function for the promise used for the
 *                          echonest response
 * @param totalStrength the combined strength of all of the radio seeds
 * @param data the data sent back from the json get request to the echonest api
 */
function processEchonestResponse(promiseResolution, totalStrength, data) {
	this.tracks = data['response']['songs'];
	preprocessTracks(this.tracks);
	var ratio = this.strengthNum() / totalStrength;
	var numTracks = Math.ceil(this.tracks.length * ratio);
	console.log('tracks for %s', this.artist());
	logTracks(this.tracks);
	promiseResolution(this.tracks.slice(0, numTracks));
}

/**
 * Make echonest's track data more manageable
 * @param tracklist the list of tracks to process
 */
function preprocessTracks(tracklist) {
    tracklist.forEach(function(track){
        track.default_track = track.tracks[0];
		track.spotify_id = foreignIdToSpotifyId(track.default_track.foreign_id);
	});
}

/**
 * converts an echonest foreign id to a spotify id
 */
function foreignIdToSpotifyId(foreignId) {
    var idChunks = foreignId.split(':');
    return idChunks[idChunks.length - 1];
}

/**
 * Constructs the Spotify Controller
 * @param playlist the list of echonest track objects
 */
 function constructSpotifyController(playlist) {
    var track_ids = '';
    playlist.forEach(function(track){
        track_ids += track.spotify_id+',';
    });
    track_ids = track_ids.slice(0, track_ids.length - 1);
    var embedPlayer = '<iframe src="https://embed.spotify.com/?uri=spotify:trackset:quick-radio:' + track_ids + '" frameborder="0" allowtransparency="true"></iframe>';
    $('#player-container').html(embedPlayer);
 }