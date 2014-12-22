// include in footer.html
//<script type="text/javascript" src="js/youtube.js"></script>
//<script src="https://apis.google.com/js/client.js?onload=googleApiClientReady"></script>

// Define some variables used to remember state.
var playlistId, nextPageToken, prevPageToken;

// After the API loads, call a function to get the uploads playlist ID.
function handleAPILoaded() {
  
}

function googleApiClientReady() {
  gapi.client.setApiKey('AIzaSyAacS6csV-G7i8JjiSq-FNZ4HSakAijso4');
  gapi.client.load('youtube', 'v3').then(requestUserUploadsPlaylistId);
  //alert("DERP");
}


// Call the Data API to retrieve the playlist ID that uniquely identifies the
// list of videos uploaded to the currently authenticated user's channel.
function requestUserUploadsPlaylistId() {
  // See https://developers.google.com/youtube/v3/docs/channels/list
  var request = gapi.client.youtube.channels.list({
    part: 'contentDetails',
    id: 'UCy0ZWRLCMAW2pokjX9or-YQ'

  });
  request.execute(function(response) {
    console.log(response.result);
    playlistId = response.result.items[0].contentDetails.relatedPlaylists.uploads;
    requestVideoPlaylist(playlistId);
  });
}

// Retrieve the list of videos in the specified playlist.
function requestVideoPlaylist(playlistId, pageToken) {
  $('.video-container').html('');
  var requestOptions = {
    playlistId: playlistId,
    part: 'snippet',
    maxResults: 1
  };
  if (pageToken) {
    requestOptions.pageToken = pageToken;
  }
  var request = gapi.client.youtube.playlistItems.list(requestOptions);
  request.execute(function(response) {
    // Only show pagination buttons if there is a pagination token for the
    // next or previous page of results.
    nextPageToken = response.result.nextPageToken;
    var nextVis = nextPageToken ? 'visible' : 'hidden';
    $('#next-button').css('visibility', nextVis);
    prevPageToken = response.result.prevPageToken
    var prevVis = prevPageToken ? 'visible' : 'hidden';
    $('#prev-button').css('visibility', prevVis);

    var playlistItems = response.result.items;
    if (playlistItems) {
      $.each(playlistItems, function(index, item) {
        displayResult(item.snippet);
        console.log(item.snippet);
      });
    } else {
      $('.video-container').html('Sorry you have no uploaded videos');
    }
  });
}

// Create a listing for a video.
function displayResult(videoSnippet) {
  var title = videoSnippet.title;
  var videoId = videoSnippet.resourceId.videoId;

  $('.video-container').append('<iframe width="560" height="315" src="//www.youtube.com/embed/' + videoId + '" frameborder="0" allowfullscreen></iframe>');
  //$('.video-container').fitVids();
}

// Retrieve the next page of videos in the playlist.
function nextPage() {
  requestVideoPlaylist(playlistId, nextPageToken);
}

// Retrieve the previous page of videos in the playlist.
function previousPage() {
  requestVideoPlaylist(playlistId, prevPageToken);
}