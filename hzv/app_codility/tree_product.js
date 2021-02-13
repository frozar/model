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
// const A = [0, 1, 2, 3, 4];
// const B = [1, 2, 3, 4, 5];
// const A = [0, 1];
// const B = [1, 2];
// const A = [0, 1, 1, 3, 4, 5, 6, 7, 8, 8, 8];
// const B = [1, 2, 4, 4, 5, 6, 7, 8, 9, 10, 11];
// const A = [0, 1, 2, 3, 4, 5, 6, 7, 7];
// const B = [2, 2, 3, 4, 5, 6, 7, 8, 9];
// const A = [0, 0, 0, 1, 1, 2, 3, 7, 7, 8, 8];
// const B = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11];
// const A = [0, 0, 2, 3, 3, 1, 6, 6, 6];
// const B = [1, 2, 3, 4, 5, 6, 7, 8, 9];
// const A = [0, 0, 0, 0, 0, 1, 1, 1, 1];
// const B = [1, 2, 3, 4, 5, 6, 7, 8, 9];
// const A = [0, 0, 0, 3, 3, 1, 1, 7, 7];
// const B = [1, 2, 3, 4, 5, 6, 7, 8, 9];
// const A = [0, 0, 0, 3];
// const B = [1, 2, 3, 4];

function computeAdjacentLists(A, B) {
  const adjacentLists = {};
  function addElt(adjacentLists, n0, n1) {
    if (adjacentLists[n0]) {
      adjacentLists[n0].add(n1);
    } else {
      adjacentLists[n0] = new Set([n1]);
    }
  }

  for (let i = 0; i < A.length; ++i) {
    const n0 = A[i];
    const n1 = B[i];

    addElt(adjacentLists, n0, n1);
    addElt(adjacentLists, n1, n0);
  }

  return adjacentLists;
}

function walkDepthFirst(seedNode, adjacentLists, func, funcArg) {
  // Depth first walkthrough the graph from a seedNode
  const nodeToVisite = [seedNode];
  const alreadyVisited = new Set();
  while (nodeToVisite.length !== 0) {
    const curNode = nodeToVisite.shift();
    const nodeNeighbourghood = adjacentLists[curNode];

    for (let neighbourgNode of nodeNeighbourghood) {
      // console.log("node", node);
      if (neighbourgNode !== curNode && !alreadyVisited.has(neighbourgNode)) {
        func(curNode, neighbourgNode, funcArg);
        nodeToVisite.push(neighbourgNode);
      }
    }
    alreadyVisited.add(curNode);
  }
}

function computeRootNode(adjacentLists) {
  const leaves = new Set();
  for (const [node, set] of Object.entries(adjacentLists)) {
    if (set.size === 1) {
      leaves.add(node);
    }
  }
  // console.log("leaves", leaves);

  const distToLeaves = {};
  const nodes = Object.keys(adjacentLists);
  // Init distToLeaves
  for (node in nodes) {
    // console.log("node", node);
    // distToLeaves[node] = nodes.length;
    distToLeaves[node] = undefined;
  }

  let treeHeight = 0;
  for (leaf of leaves) {
    distToLeaves[leaf] = 0;

    // Depth first walkthrough the graph from a leaf
    walkDepthFirst(leaf, adjacentLists, (curNode, neighbourgNode) => {
      if (distToLeaves[neighbourgNode] === undefined) {
        treeHeight = Math.max(treeHeight, distToLeaves[curNode] + 1);
        distToLeaves[neighbourgNode] = distToLeaves[curNode] + 1;
      } else {
        treeHeight = Math.max(
          treeHeight,
          distToLeaves[curNode] + 1,
          distToLeaves[neighbourgNode]
        );
        distToLeaves[neighbourgNode] = Math.min(
          distToLeaves[neighbourgNode],
          distToLeaves[curNode] + 1
        );
      }
    });
  }

  // const treeHeight = Math.max(...Object.values(distToLeaves));
  // console.log("maxDist", maxDist);

  nodes.sort((n0, n1) => {
    if (distToLeaves[n0] < distToLeaves[n1]) {
      return -1;
    } else if (distToLeaves[n0] > distToLeaves[n1]) {
      return 1;
    } else {
      if (adjacentLists[n0].size < adjacentLists[n1].size) {
        return -1;
      } else if (adjacentLists[n0].size > adjacentLists[n1].size) {
        return 1;
      } else {
        return n1.localeCompare(n0);
      }
    }
  });

  const rootNode = parseInt(nodes[nodes.length - 1]);
  return [rootNode, treeHeight];
}

function graphFromNodeAux(seedNode, seedParentNode, adjacentLists) {
  // const subGraph = [seedNode];
  let subGraph = [];

  const nodeToVisite = [seedNode];
  const parentNodes = [seedParentNode];
  while (nodeToVisite.length !== 0) {
    const curNode = nodeToVisite.shift();
    const parentNode = parentNodes.shift();
    const nodeNeighbourghood = adjacentLists[curNode];

    const parentNodeSet = new Set([parentNode]);
    const correctNeighbourhood = setDifference(
      nodeNeighbourghood,
      parentNodeSet
    );
    // console.log("curNode", curNode);
    // console.log("correctNeighbourhood", correctNeighbourhood);

    if (correctNeighbourhood.size === 0) {
      subGraph.push(curNode);
      return subGraph;
    } else if (correctNeighbourhood.size === 1) {
      subGraph.push(curNode);
      for (let node of correctNeighbourhood) {
        // subGraph.push(node);
        nodeToVisite.push(node);
        parentNodes.push(curNode);
      }
    } else {
      const curGraph = [curNode];
      for (let node of correctNeighbourhood) {
        const subSubGraph = graphFromNodeAux(node, curNode, adjacentLists);
        // console.log("subSubGraph", subSubGraph);
        curGraph.push(subSubGraph);
      }

      // subGraph.push(curGraph);
      if (subGraph.length === 0) {
        subGraph = curGraph;
      } else {
        subGraph.push(curGraph);
      }
    }
    // console.log("subGraph", subGraph);
    // console.log("");
  }

  return subGraph;
}

function graphFromNode(rootNode, adjacentLists) {
  const graph = [rootNode];
  // console.log("graph", graph);

  const rootNeighbourghood = adjacentLists[rootNode];
  // console.log("rootNeighbourghood", rootNeighbourghood);
  // let parentNode = null;
  // for (let node in adjacentLists[rootNode]) {
  // }

  if (rootNeighbourghood.size === 1) {
    for (let node of rootNeighbourghood) {
      // console.log("unique node", node);
      graph.push(node);
    }
  } else {
    for (let node of rootNeighbourghood) {
      // console.log("NON UNIQUE node", node);
      const subGraph = graphFromNodeAux(node, rootNode, adjacentLists);
      graph.push(subGraph);
      // break;
    }
  }

  return graph;
}

function graphWeight(graph, key = [], res = {}) {
  let acc = 0;
  for (let i = 0; i < graph.length; ++i) {
    // console.log("graph[i]", graph[i]);
    // console.log("Array.isArray(graph[i])", Array.isArray(graph[i]));
    // // "number"
    if (Array.isArray(graph[i])) {
      const subKey = [...key, i];
      // acc += graphWeight(graph[i], subKey, res);
      graphWeight(graph[i], subKey, res);
      // subKey.push(0);
      // console.log("res", res);
      // console.log("subKey", subKey);
      // console.log("res[subKey]", res[subKey]);
      acc += res[subKey];
    } else {
      acc += 1;
    }
  }
  // const newKey = [...key, 0];
  const newKey = [...key];
  res[newKey] = acc;
  return res;
}

function cut1Bridge(weights, sortedKeys, treeHeight) {
  const heightThreshold = Math.ceil(treeHeight / 2) + 1;
  // console.log("heightThreshold", heightThreshold);
  const total = weights[""];
  let maxProd = 0;
  for (let i = 1; i < sortedKeys.length; ++i) {
    const key = sortedKeys[i];
    // console.log("key.split(", ").length ", key.split(",").length);
    if (key.split(",").length <= heightThreshold) {
      // console.log("IN");
      const comp0 = weights[key];
      const comp1 = total - comp0;
      // console.log("comp0 * comp1", comp0 * comp1);
      maxProd = Math.max(maxProd, comp0 * comp1);
    }
  }

  return maxProd;
}

function cut2Bridge(weights, sortedKeys, treeHeight) {
  const heightThreshold = Math.ceil(treeHeight / 2) + 1;
  // const heightThreshold = treeHeight + 1;
  // console.log("heightThreshold", heightThreshold);
  const total = weights[""];
  let maxProd = 0;
  for (let i = 1; i < sortedKeys.length - 1; ++i) {
    for (let j = i + 1; j < sortedKeys.length; ++j) {
      const key0 = sortedKeys[i];
      const key1 = sortedKeys[j];
      // console.log("key0", key0.split(",").length);
      // console.log("key1", key1);
      if (
        key0.split(",").length <= heightThreshold &&
        key1.split(",").length <= heightThreshold
      ) {
        // console.log("key0, key1", key0, key1);
        let comp0 = weights[key0];
        let comp1 = weights[key1];

        // console.log("'^' + key0", "^" + key0);
        const regex0 = new RegExp("^" + key0);
        const regex1 = new RegExp("^" + key1);
        // console.log("regex.test(key1)", regex.test(key1));
        // If the key1 is a subKey of key0
        if (regex0.test(key1)) {
          comp0 -= comp1;
        } else if (regex1.test(key0)) {
          comp1 -= comp0;
        }
        const comp2 = total - comp0 - comp1;
        // console.log("comp0 * comp1 * comp2", comp0 * comp1 * comp2);
        maxProd = Math.max(maxProd, comp0 * comp1 * comp2);
      }
    }
  }

  return maxProd;
}

function solution(A, B) {
  const N = A.length;

  if (N < 3) {
    const res = N + 1;
    return res.toString();
  }

  const adjacentLists = computeAdjacentLists(A, B);

  const [rootNode, treeHeight] = computeRootNode(adjacentLists);
  // console.log("[rootNode, treeHeight]", [rootNode, treeHeight]);

  const graph = graphFromNode(rootNode, adjacentLists);
  const weights = graphWeight(graph);
  const sortedKeys = Object.keys(weights).sort((k0, k1) => {
    if (k0.length < k1.length) {
      return -1;
    } else if (k0.length > k1.length) {
      return 1;
    } else {
      return k0.localeCompare(k1);
    }
  });
  // console.log("graph", graph);
  // console.log("sortedKeys", sortedKeys);
  // console.log("weights", weights);
  // console.log("treeHeight", treeHeight);

  const case0Prod = weights[""];
  const case1Prod = cut1Bridge(weights, sortedKeys, treeHeight);
  const case2Prod = cut2Bridge(weights, sortedKeys, treeHeight);
  let maxProd = Math.max(case0Prod, case1Prod, case2Prod);

  return maxProd.toString();
}

// console.log("solution(A, B)", solution(A, B));
