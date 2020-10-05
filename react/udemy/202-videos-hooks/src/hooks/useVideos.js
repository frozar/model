import { useEffect, useState } from 'react';
import youtube from '../api/youtube';

const useVideos = (defaultSearchTerm) => {
  const [videos, setVideos] = useState([]);

  useEffect(() => {
    search(defaultSearchTerm);
  }, [defaultSearchTerm]);

  const search = async (term) => {
    const reponse = await youtube.get('/search', {
      params: {
        q: term
      }
    });

    setVideos(reponse.data.items);
  };

  return [videos, search];
};

export default useVideos;
