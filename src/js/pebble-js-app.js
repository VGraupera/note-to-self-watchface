var note = localStorage.getItem('note') || '';

Pebble.addEventListener('showConfiguration', function(e) {
	var uri = 'https://rawgithub.com/VGraupera/note-to-self-watchface/master/html/configuration.html?' +
				'note=' + encodeURIComponent(note);
	console.log('showing configuration at uri: ' + uri);
	Pebble.openURL(uri);
});

Pebble.addEventListener('webviewclosed', function(e) {
	console.log('configuration closed');
	if (e.response) {
		var options = JSON.parse(decodeURIComponent(e.response));
		console.log('options received from configuration: ' + JSON.stringify(options));
		apiKey = options['note'];
		localStorage.setItem('note', apiKey);
	} else {
		console.log('no options received');
	}
});
