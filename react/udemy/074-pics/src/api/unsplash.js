import axios from 'axios';

export default axios.create({
  baseURL: 'https://api.unsplash.com',
  headers: {
    Authorization: 'Client-ID fBOuPaU5pL8kXyyKgE4EO0XAItwCH3TbJLe4D8Bi6co'
  }
});