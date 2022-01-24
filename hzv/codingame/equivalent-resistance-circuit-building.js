const resistances = {};
const N = parseInt(readline());

for (let i = 0; i < N; i++) {
  var inputs = readline().split(" ");
  const name = inputs[0];
  const R = parseInt(inputs[1]);
  resistances[name] = R;
}

const circuit = readline();

function compute_serie(tokens_circuit) {
  let members = [];
  let nb = deal_tokens_circuit(tokens_circuit);
  while (typeof nb === "number") {
    members.push(nb);
    nb = deal_tokens_circuit(tokens_circuit);
  }

  const res_serie = members.reduce((x, y) => x + y, 0);
  return res_serie;
}

function compute_parallel(tokens_circuit) {
  let members = [];
  let nb = deal_tokens_circuit(tokens_circuit);
  while (typeof nb === "number") {
    members.push(nb);
    nb = deal_tokens_circuit(tokens_circuit);
  }

  let res_parallel = 1 / members.map((x) => 1 / x).reduce((x, y) => x + y, 0);
  return res_parallel;
}

function deal_tokens_circuit(tokens_circuit) {
  const token = tokens_circuit.shift();
  switch (token) {
    case "(": {
      return compute_serie(tokens_circuit);
      break;
    }
    case ")": {
      return token;
      break;
    }
    case "[": {
      return compute_parallel(tokens_circuit);
      break;
    }
    case "]": {
      return token;
      break;
    }
    default: {
      return resistances[token];
    }
  }
}

const tokens_circuit = circuit.split(" ");
let res = deal_tokens_circuit(tokens_circuit);

// console.error("res final", res);
console.log(res.toFixed(1));
