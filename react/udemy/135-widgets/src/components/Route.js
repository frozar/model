import { useEffect, useState } from 'react';

const Route = ({ path, children }) => {
  // just to get our route to update, nothing more
  const [currentPath, setCurrentPath] = useState('/');

  useEffect(() => {
    const onLocationChange = () => {
      setCurrentPath(window.location.pathname);
    };

    window.addEventListener('popstate', onLocationChange);

    return () => {
      window.removeEventListener('popstate', onLocationChange);
    }
  }, [currentPath]);

  return currentPath === path
    ? children
    : null;
};

export default Route;