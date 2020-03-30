// import React from 'react';
// import logo from './logo.svg';
import rough from 'roughjs/bin/rough.js';
import './App.css';

function App() {
    // const rc = rough.canvas(document.getElementById('canvas'));
    // rc.rectangle(10, 10, 200, 200); // x, y, width, height
    // or SVG

    const svg = document.getElementById('canvas');
    const rc = rough.svg(svg);
    let node1 = rc.path("M 100 100 L 300 100 L 200 300 z", { fill: 'red' });
    svg.appendChild(node1);

    let node2 = rc.path("M 400 100 L 600 100 L 500 300 z", { fill: 'red' });
    svg.appendChild(node2);

    return null;
    //   return (
    //   <div className="App">
    //     <header className="App-header">
    //       <img src={logo} className="App-logo" alt="logo" />
    //       <p>
    //         Edit <code>src/App.js</code> and save to reload.
    //       </p>
    //       <a
    //         className="App-link"
    //         href="https://reactjs.org"
    //         target="_blank"
    //         rel="noopener noreferrer"
    //       >
    //         Learn React
    //       </a>
    //     </header>
    //   </div>
    // );
}

export default App;
