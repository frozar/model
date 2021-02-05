// ***** Begin: Helper functions *****
function setUnion(a, b) {
  const union = new Set([...a, ...b]);
  return union;
}

function setIntersection(a, b) {
  const intersection = new Set([...a].filter((x) => b.has(x)));
  return intersection;
}

function setDifference(a, b) {
  const difference = new Set([...a].filter((x) => !b.has(x)));
  return difference;
}
// ***** End: Helper functions *****

// const A = [0, 1, 1, 3, 3, 6, 7];
// const B = [1, 2, 3, 4, 5, 3, 5];
// const A = [0, 1];
// const B = [1, 2];
const A = [0, 1, 1, 3, 4, 5, 6, 7, 8, 8, 8];
const B = [1, 2, 4, 4, 5, 6, 7, 8, 9, 10, 11];
// const A = [0, 1, 2, 3, 4, 5, 6, 7, 7];
// const B = [2, 2, 3, 4, 5, 6, 7, 8, 9];

function highestDegree(A, B) {
  const N = A.length;

  const dict = {};
  function addElt(dict, n0, n1) {
    if (dict[n0]) {
      dict[n0].add(n1);
    } else {
      dict[n0] = new Set([n1]);
    }
  }

  for (let i = 0; i < N; ++i) {
    const n0 = A[i];
    const n1 = B[i];

    addElt(dict, n0, n1);
    addElt(dict, n1, n0);
  }

  let res = undefined;
  let maxDegree = 0;
  for (const [node, set] of Object.entries(dict)) {
    const currentDegree = set.size;
    if (maxDegree < currentDegree) {
      maxDegree = currentDegree;
      res = parseInt(node);
    }
  }

  return [res, N, dict];
}

function graphFromNode(rootNode, dict) {
  const graph = {};
  // console.log("graph", graph);

  const rootNeighbourghood = dict[rootNode];

  // Initialisation of the graph
  for (let node of rootNeighbourghood) {
    graph[node] = new Set([node]);
  }

  for (const [kNode, curNeighbourghood] of Object.entries(graph)) {
    // console.log("kNode", kNode);

    // console.log("curNeighbourghood", curNeighbourghood);
    const seedNode = parseInt(kNode);
    // console.log("seedNode", seedNode);

    const nodeToVisite = [seedNode];
    const alreadyVisited = new Set();
    while (nodeToVisite.length !== 0) {
      const curNode = nodeToVisite.shift();

      // console.log("curNode", curNode);
      const accessibleNodes = dict[curNode];
      // console.log("accessibleNodes", accessibleNodes);
      // console.log("graph[seedNode]", graph[seedNode]);

      for (let node of accessibleNodes) {
        // console.log("node", node);
        if (node !== rootNode && !alreadyVisited.has(node)) {
          curNeighbourghood.add(node);
          nodeToVisite.push(node);
        }
      }

      alreadyVisited.add(curNode);
    }

    // break;
  }

  // console.log("res", graph);
  return graph;
}

// function cut1Bridge(dict, seedNode) {
//   let maxProd = 0;

//   const rootNeighbourghood = Array.from(dict[seedNode]);
//   const graph = graphFromNode(seedNode, dict);
//   // console.log("rootNeighbourghood", rootNeighbourghood);
//   for (let i = 0; i < rootNeighbourghood.length; ++i) {
//     const n0 = rootNeighbourghood[i];
//     let rootCompSize = 1;
//     for (const [node, set] of Object.entries(graph)) {
//       const iNode = parseInt(node);
//       if (iNode !== n0) {
//         // console.log("iNode", iNode);
//         // console.log("set.size", set.size);
//         rootCompSize += set.size;
//       }
//     }
//     // console.log("rootCompSize", rootCompSize);

//     const comp0Size = graph[n0].size;

//     const prod = comp0Size * rootCompSize;
//     // console.log("comp0Size", comp0Size);
//     // console.log("rootCompSize", rootCompSize);
//     // console.log("prod", prod);

//     if (maxProd < prod) {
//       maxProd = prod;
//     }
//   }

//   return maxProd;
// }

function cut1Bridge(A, B, graphsFromNode) {
  let maxProd = 0;
  for (let i = 0; i < A.length; ++i) {
    const nCut0 = A[i];
    const nCut1 = B[i];
    const curGraph = graphsFromNode[nCut0];
    // console.log("nCut0", nCut0);
    // console.log("nCut1", nCut1);
    // console.log("curGraph", curGraph);

    let comp0Size = 1;
    for (let node of Object.keys(curGraph)) {
      const iNode = parseInt(node);
      if (iNode !== nCut1) {
        comp0Size += curGraph[node].size;
      }
    }
    const comp1Size = curGraph[nCut1].size;

    const prod = comp0Size * comp1Size;
    // console.log("prod", prod);
    // console.log("");

    if (maxProd < prod) {
      maxProd = prod;
    }
  }
  return maxProd;
}

function cut2Bridge(A, B, graphsFromNode) {
  let maxProd = 0;

  for (let i = 0; i < A.length - 1; ++i) {
    for (let j = i + 1; j < A.length; ++j) {
      const nCut00 = A[i];
      const nCut01 = B[i];
      const nCut10 = A[j];
      const nCut11 = B[j];

      const curGraph0 = graphsFromNode[nCut00];
      const curGraph1 = graphsFromNode[nCut10];

      let setNCut00 = new Set([nCut00]);
      for (let node of Object.keys(curGraph0)) {
        const iNode = parseInt(node);
        if (iNode !== nCut01) {
          setNCut00 = setUnion(setNCut00, curGraph0[node]);
        }
      }
      const setNCut01 = curGraph0[nCut01];

      let setNCut10 = new Set([nCut10]);
      for (let node of Object.keys(curGraph1)) {
        const iNode = parseInt(node);
        if (iNode !== nCut11) {
          setNCut10 = setUnion(setNCut10, curGraph1[node]);
        }
      }
      const setNCut11 = curGraph1[nCut11];

      console.log("nCut00", nCut00);
      console.log("nCut01", nCut01);
      console.log("nCut10", nCut10);
      console.log("nCut11", nCut11);
      // console.log("setNCut00", setNCut00);
      // console.log("setNCut01", setNCut01);
      // console.log("setNCut10", setNCut10);
      // console.log("setNCut11", setNCut11);

      let electedSet0;
      let electedSet1;
      let comp0Size;
      let comp1Size;
      if (setNCut00.has(nCut10) && setNCut00.has(nCut11)) {
        electedSet0 = setNCut00;
        comp0Size = setNCut01.size;
      } else {
        electedSet0 = setNCut01;
        comp0Size = setNCut00.size;
      }

      if (setNCut10.has(nCut00) && setNCut10.has(nCut01)) {
        electedSet1 = setNCut10;
        comp1Size = setNCut11.size;
      } else {
        electedSet1 = setNCut11;
        comp1Size = setNCut10.size;
      }

      const intersectionSet = setIntersection(electedSet0, electedSet1);

      // console.log("electedSet0", electedSet0);
      // console.log("electedSet1", electedSet1);
      // console.log("intersectionSet", intersectionSet);
      const comp2Size = intersectionSet.size;

      // console.log("comp0Size", comp0Size);
      // console.log("comp1Size", comp1Size);
      // console.log("comp2Size", comp2Size);

      const prod = comp0Size * comp1Size * comp2Size;
      console.log("prod", prod);
      console.log("");

      if (maxProd < prod) {
        maxProd = prod;
      }
    }
  }
  return maxProd;
}

function solution(A, B) {
  const [rootNode, N, dict] = highestDegree(A, B);
  // console.log("rootNode", rootNode);
  // console.log("N", N);
  // console.log("dict", dict);

  if (N < 3) {
    const res = N + 1;
    return res;
  }

  const graph = graphFromNode(rootNode, dict);
  // console.log("graph", graph);

  // Don't cut bridge
  let case0Prod = 0;
  for (const [node, set] of Object.entries(graph)) {
    case0Prod += set.size;
  }
  case0Prod += 1;
  let maxProd = case0Prod;

  // Cut 1 bridge
  let graphsFromNode = {};
  for (let node = 0; node < N + 1; ++node) {
    graphsFromNode[node] = graphFromNode(node, dict);
  }
  // console.log("graphsFromNode", graphsFromNode);

  const prodCut1 = cut1Bridge(A, B, graphsFromNode);
  if (maxProd < prodCut1) {
    maxProd = prodCut1;
  }

  const prodCut2 = cut2Bridge(A, B, graphsFromNode);
  if (maxProd < prodCut2) {
    maxProd = prodCut2;
  }

  return maxProd;
}

console.log("solution(A, B)", solution(A, B).toString());
