import axios from 'axios';

const KEY = 'AIzaSyBBv0EZu-YVm4yDRKnzY9E3pRzd_VzBURg';

export default axios.create({
  baseURL: 'https://www.googleapis.com/youtube/v3',
  params: {
    part: 'snippet',
    type: 'video',
    maxResults: 5,
    key: KEY
  }
});

