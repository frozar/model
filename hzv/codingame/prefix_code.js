// To debug
function dbg() {
    console.error('DBG:', ...Object.values(arguments));
}

const n = parseInt(readline());
const codex = {};
for (let i = 0; i < n; i++) {
    var inputs = readline().split(' ');
    const b = inputs[0];
    const c = parseInt(inputs[1]);
    codex[b] = c;
}
const s = readline();

let i = 0;
let res = "";
while (i < s.length) {
    let notFound = true;
    for (key of Object.keys(codex)) {
        if (key === s.slice(i, i + key.length)) {
            res += String.fromCharCode(codex[key]);
            i += key.length;
            notFound = false;
            break;
        }
    }

    if (notFound) {
        res = i;
        break;
    }
}
// Write an answer using console.log()
// To debug: console.error('Debug messages...');

if (Number.isInteger(res)) {
    console.log('DECODE FAIL AT INDEX', res);
}
else {
    console.log(res);
}
