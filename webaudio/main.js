// Inspiration:
// https://www.youtube.com/watch?v=3NgVlAscdcA&list=PLMPgoZdlPumc_llMSynz5BqT8dTwr5sZ2&index=1
const AudioContext = window.AudioContext || window.webkitAudioContext;

const ctx = new AudioContext();
let audio;
let playSound;

let play = false;

fetch("./sounds/Puddle Of Mudd - Blurry.mp3")
  .then((data) => data.arrayBuffer())
  .then((arrayBuffer) => ctx.decodeAudioData(arrayBuffer))
  .then((decodeAudio) => {
    audio = decodeAudio;
  });

function playback() {
  play = !play;
  if (play) {
    playSound = ctx.createBufferSource();
    playSound.buffer = audio;
    playSound.connect(ctx.destination);
    playSound.start(ctx.currentTime);
  } else {
    console.log(playSound);
    playSound.stop(0);
  }
  // https://stackoverflow.com/questions/53791472/how-to-do-time-streching-audio-playback-using-javascript-in-2019
  // const player = new Audio();
  // player.src = "./sounds/Puddle Of Mudd - Blurry.mp3";
  // player.playbackRate = 0.75;
  // player.play();
}

window.addEventListener("mousedown", playback);
