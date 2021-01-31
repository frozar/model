// To debug
function dbg() {
  console.error("DBG:", ...Object.values(arguments));
}

// ********** GLOBAL STUFF **********

// ********** GLOBAL CONSTANT **********
const LIMIT_BEGIN_GAME = 8; // nb iteration
const LIMIT_END_GAME = 3; // nb potions
const LIMIT_NB_POTION = 1;

const INV_TOTAL_MAX = 10;

// *** BIT CONSTANT ***
const ME_ID = 0;
const CAST_ID = 1;
const BREW_ID = 2;
const LEARN_ID = 3;
const REST_ID = 4;

const INV_FIRST_BIT = 0;
const INV_LAST_BIT = 19;
const INV_SIZE = INV_LAST_BIT - INV_FIRST_BIT + 1;
const CHUNK_LAST_BIT = 4;
const CHUNK_SIZE = 5;
const SCORE_FIRST_BIT = 20;
const SCORE_LAST_BIT = 26;
const SCORE_SIZE = SCORE_LAST_BIT - SCORE_FIRST_BIT + 1;

const ACTION_ID_FIRST_BIT = 20;
const ACTION_ID_LAST_BIT = 26;
const ACTION_ID_SIZE = ACTION_ID_LAST_BIT - ACTION_ID_FIRST_BIT + 1;
const CASTABLE_BIT = 27;
const REPEATABLE_BIT = 28;

const TOME_INDEX_FIRST_BIT = 27;
const TOME_INDEX_LAST_BIT = 28;
const TOME_INDEX_SIZE = TOME_INDEX_LAST_BIT - TOME_INDEX_FIRST_BIT + 1;
const TAX_COUNT_BIT = 29;

const ID_FIRST_BIT = 30;
const ID_LAST_BIT = 31;
const ID_SIZE = ID_LAST_BIT - ID_FIRST_BIT + 1;

// *** ANSWER CONSTANT ***
const ANSWER_ID_FIRST_BIT = 29;
const ANSWER_ID_LAST_BIT = 31;
const ANSWER_ID_SIZE = ANSWER_ID_LAST_BIT - ANSWER_ID_FIRST_BIT + 1;

const TIME_LAST_BIT = 2;
const TIME_SIZE = 3;
// *** BIT MASK ***
const INV_MASK = ~(~0 << INV_SIZE);
const CHUNK_MASK = ~(~0 << CHUNK_SIZE);
const ACTION_ID_MASK = ~(~0 << ACTION_ID_SIZE);
const ID_MASK = ~(~0 << ID_SIZE);
const ANSWER_ID_MASK = ~(~0 << ANSWER_ID_SIZE);
const TIME_MASK = ~(~0 << TIME_SIZE);
const COUNT_MASK = ~(~0 << 3);

// ********** END GLOBAL CONSTANT **********

// ********** GLOBAL VARIABLE **********
let iter = 0;
let nbPotionDone = 0;
let nbOpponentPotionDone = 0;
let currentBestSolutionIdx = 0;
let currentBestSolutionActionId = null;
let previousPotionsActionId = [];
let hasBrew = false;
// ********** END GLOBAL VARIABLE **********

// ********** END GLOBAL STUFF **********

function parseInput() {
  let readInputs = [];
  const actionCount = parseInt(readline()); // the number of spells and recipes in play
  for (let i = 0; i < actionCount; i++) {
    var inputs = readline().split(" ");
    const actionId = parseInt(inputs[0]); // the unique ID of this spell or recipe
    const actionType = inputs[1]; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
    const delta0 = parseInt(inputs[2]); // tier-0 ingredient change
    const delta1 = parseInt(inputs[3]); // tier-1 ingredient change
    const delta2 = parseInt(inputs[4]); // tier-2 ingredient change
    const delta3 = parseInt(inputs[5]); // tier-3 ingredient change
    const price = parseInt(inputs[6]); // the price in rupees if this is a potion
    const tomeIndex = parseInt(inputs[7]); // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax; For brews, this is the value of the current urgency bonus
    const taxCount = parseInt(inputs[8]); // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell; For brews, this is how many times you can still gain an urgency bonus
    const castable = inputs[9] !== "0"; // in the first league: always 0; later: 1 if this is a castable player spell
    const repeatable = inputs[10] !== "0"; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
    const currentLine = {
      actionId,
      actionType,
      delta: [delta0, delta1, delta2, delta3],
      price,
      tomeIndex,
      taxCount,
      castable,
      repeatable,
    };
    readInputs.push(currentLine);
  }
  for (let i = 0; i < 2; i++) {
    var inputs = readline().split(" ");
    const inv0 = parseInt(inputs[0]); // tier-0 ingredients in inventory
    const inv1 = parseInt(inputs[1]);
    const inv2 = parseInt(inputs[2]);
    const inv3 = parseInt(inputs[3]);
    const score = parseInt(inputs[4]); // amount of rupees
    const playerId = i === 0 ? "ME" : "OPPONENT";
    const currentLine = {
      playerId,
      inv: [inv0, inv1, inv2, inv3],
      score,
    };
    readInputs.push(currentLine);
  }
  return readInputs;
}

function getMe(inputs) {
  for (obj of inputs) {
    if (obj.playerId === "ME") {
      return obj;
    }
  }
}

function getOpponent(inputs) {
  for (obj of inputs) {
    if (obj.playerId === "OPPONENT") {
      return obj;
    }
  }
}

function getGrimoire(inputs) {
  return inputs.filter((obj) => obj.actionType === "LEARN");
}

function getCasts(inputs) {
  return inputs.filter((obj) => obj.actionType === "CAST");
}

function getPotions(inputs) {
  return inputs.filter((obj) => obj.actionType === "BREW");
}

// toPrepare: a potion or a cast
function getBalance(toPrepare, me) {
  const delta = toPrepare.delta;
  const inv = me.inv;
  let balance = [0, 0, 0, 0];
  for (let idx = 0; idx < 4; ++idx) {
    balance[idx] = delta[idx] + inv[idx];
  }
  return balance;
}

function isAffordable(toPrepare, me) {
  const balance = getBalance(toPrepare, me);
  let allPositive = true;
  let totalIngredient = 0;
  for (const val of balance) {
    allPositive &= 0 <= val;
    totalIngredient += val;
  }
  return allPositive && totalIngredient <= INV_TOTAL_MAX;
}

function isAffordableBin(toPrepareBin, meBin) {
  const deltaBin = toPrepareBin & INV_MASK;
  const invBin = meBin & INV_MASK;
  let balance = 0;
  let total = 0;
  for (let idx = 0; idx < 4; ++idx) {
    const n0 = (deltaBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
    const n1 = (invBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
    const add = (n0 + n1) & CHUNK_MASK;
    balance |= add << (idx * CHUNK_SIZE);
    total += add;
  }
  const sign0 = balance & (1 << (CHUNK_LAST_BIT + 0)) ? 1 : 0;
  const sign1 = balance & (1 << (CHUNK_LAST_BIT + 5)) ? 1 : 0;
  const sign2 = balance & (1 << (CHUNK_LAST_BIT + 10)) ? 1 : 0;
  const sign3 = balance & (1 << (CHUNK_LAST_BIT + 15)) ? 1 : 0;
  return sign0 + sign1 + sign2 + sign3 === 0 && total <= INV_TOTAL_MAX;
}

function isLearnable(castToLearn, me) {
  const cost = castToLearn.tomeIndex;
  const inv = me.inv;
  return cost <= inv[0];
}

function dist(balance) {
  // return balance.reduce((acc, curVal, i) => {
  //   acc += curVal * 10 ** i;
  //   return acc;
  // }, 0);
  const x0 = balance[0] * 1;
  const x1 = balance[1] * 2;
  const x2 = balance[2] * 4;
  const x3 = balance[3] * 8;
  return x0 + x1 + x2 + x3;
}

function score(p, me) {
  const x0 = -p.delta[0] * 1;
  const x1 = -p.delta[1] * 2;
  const x2 = -p.delta[2] * 4;
  const x3 = -p.delta[3] * 8;
  // const inv = me.inv;
  // const x0 = Math.max(0, -(inv + p.delta[0])) * 1;
  // const x1 = Math.max(0, -(inv + p.delta[1])) * 2;
  // const x2 = Math.max(0, -(inv + p.delta[2])) * 4;
  // const x3 = Math.max(0, -(inv + p.delta[3])) * 8;
  return x0 + x1 + x2 + x3 - p.price / 2.0;
}

function printCast(cast) {
  const { actionId, delta, castable, repeatable } = cast;
  dbg(
    "actionId",
    actionId,
    " delta",
    delta,
    " castable",
    castable ? 1 : 0,
    " repeatable",
    repeatable ? 1 : 0
  );
}

function getPossibleActionsBin(contextBin) {
  let possibleActions = [];
  let restActionPossible = false;
  for (const castBin of contextBin.toPrepare) {
    // If not castable
    if ((castBin & (1 << CASTABLE_BIT)) === 0) {
      restActionPossible = true;
      continue;
    }
    // If not repeatable
    if ((castBin & (1 << REPEATABLE_BIT)) === 0) {
      if (isAffordableBin(castBin, contextBin.me)) {
        const idInt =
          ((castBin >> ID_FIRST_BIT) & ID_MASK) << ANSWER_ID_FIRST_BIT;
        const actionIdInt =
          ((castBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK) <<
          ACTION_ID_FIRST_BIT;
        const actionBin = idInt | actionIdInt;

        possibleActions.push(actionBin);
      }
      continue;
    }
    // If castable and repeatable
    let probeCastBin = castBin;

    let factor = 0;
    while (isAffordableBin(probeCastBin, contextBin.me)) {
      factor += 1;
      const probeCastDeltaBin = probeCastBin & INV_MASK;
      const castDeltaBin = castBin & INV_MASK;
      let nextDeltaBin = 0;
      for (let idx = 0; idx < 4; ++idx) {
        const n0 = (probeCastDeltaBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
        const n1 = (castDeltaBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
        const add = (n0 + n1) & CHUNK_MASK;
        nextDeltaBin |= add << (idx * CHUNK_SIZE);
      }
      probeCastBin = (probeCastBin & (~0 << INV_SIZE)) | nextDeltaBin;
    }

    for (let times = 1; times <= factor; times++) {
      const idInt =
        ((castBin >> ID_FIRST_BIT) & ID_MASK) << ANSWER_ID_FIRST_BIT;
      const actionIdInt =
        ((castBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK) <<
        ACTION_ID_FIRST_BIT;
      const timesInt = times & TIME_MASK;
      const actionBin = idInt | actionIdInt | timesInt;

      possibleActions.push(actionBin);
    }
  }

  if (restActionPossible) {
    const actionBin = REST_ID << ANSWER_ID_FIRST_BIT;

    const id = (actionBin >> ANSWER_ID_FIRST_BIT) & ANSWER_ID_MASK;
    if (id === 0) {
      throw "THROW REST";
    }

    possibleActions.push(actionBin);
  }

  return possibleActions;
}

function applyActionBin(contextBin, actionBin) {
  let nextContext = {
    me: contextBin.me,
    toPrepare: [...contextBin.toPrepare],
    actionsDone: [...contextBin.actionsDone],
  };
  const id = (actionBin >> ANSWER_ID_FIRST_BIT) & ANSWER_ID_MASK;
  if (id === REST_ID) {
    // Set castable to true
    for (let idx = 0; idx < nextContext.toPrepare.length; ++idx) {
      nextContext.toPrepare[idx] |= 1 << CASTABLE_BIT;
    }
  } else if (id === CAST_ID) {
    const castId = (actionBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
    const times = actionBin & TIME_MASK;

    let castBin = null;
    let idxTargetCast = null;
    for (let idx = 0; idx < contextBin.toPrepare.length; ++idx) {
      const availableCastBin = contextBin.toPrepare[idx];
      const actionId =
        (availableCastBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
      if (actionId === castId) {
        idxTargetCast = idx;
        castBin = availableCastBin;
        break;
      }
    }

    // Init deltaBin
    let deltaBin = castBin & INV_MASK;
    if (times !== undefined) {
      // Realise the multiplication by multiple addition
      // More addition are done only if (2 <= times)
      const incDeltaBin = castBin & INV_MASK;
      for (let idx = 2; idx <= times; ++idx) {
        let nextDeltaBin = 0;
        for (let idx = 0; idx < 4; ++idx) {
          const n0 = (deltaBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
          const n1 = (incDeltaBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
          const add = (n0 + n1) & CHUNK_MASK;
          nextDeltaBin |= add << (idx * CHUNK_SIZE);
        }
        deltaBin = nextDeltaBin;
      }
    }

    // Get balance: the new inv
    const invBin = contextBin.me & INV_MASK;
    let nextInvBin = 0;
    for (let idx = 0; idx < 4; ++idx) {
      const n0 = (deltaBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
      const n1 = (invBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
      const add = (n0 + n1) & CHUNK_MASK;
      nextInvBin |= add << (idx * CHUNK_SIZE);
    }
    // Assign the new inv
    nextContext.me = (nextContext.me & (~0 << INV_SIZE)) | nextInvBin;
    // Set castable to false
    nextContext.toPrepare[idxTargetCast] &= ~(1 << CASTABLE_BIT);
  }
  nextContext.actionsDone.push(actionBin);
  return nextContext;
}

function hashContextBin(contextBin) {
  // Simple hash function based on XOR
  // return contextBin.toPrepare.reduce(
  //   (acc, castBin) => acc ^ castBin,
  //   contextBin.me
  // );
  let res = contextBin.me;
  for (const castBin of contextBin.toPrepare) {
    res = res ^ castBin;
  }
  for (const actionBin of contextBin.actionsDone) {
    res = res ^ actionBin;
  }
  return res;
}

const actionToObj = (actionBin) => {
  const id = (actionBin >> ANSWER_ID_FIRST_BIT) & ANSWER_ID_MASK;
  if (id === REST_ID) {
    return "REST";
  } else if (id === CAST_ID) {
    const castId = (actionBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
    const times = actionBin & TIME_MASK;
    if (times === 0) {
      return "CAST " + castId;
    } else {
      return "CAST " + castId + " " + times;
    }
  }
};

function printContextBin(contextBin) {
  dbg("contextBin");
  dbg("me", toObj(contextBin.me));
  // dbg("casts");
  // contextBin.toPrepare.map((castBin) => {
  //   printCast(toObj(castBin));
  // });
  dbg("actionsDone", contextBin.actionsDone.map(actionToObj));
}

function distToPotion(contextBin, delta) {
  const inv = contextBin.me & INV_MASK;
  let x0 = (inv & ~(~0 << 5)) >> 0;
  let x1 = (inv & ~(~0 << 10)) >> 5;
  let x2 = (inv & ~(~0 << 15)) >> 10;
  let x3 = (inv & ~(~0 << 20)) >> 15;
  x0 = (x0 & (1 << CHUNK_LAST_BIT)) === 0 ? x0 : x0 | (~0 << CHUNK_SIZE);
  x1 = (x1 & (1 << CHUNK_LAST_BIT)) === 0 ? x1 : x1 | (~0 << CHUNK_SIZE);
  x2 = (x2 & (1 << CHUNK_LAST_BIT)) === 0 ? x2 : x2 | (~0 << CHUNK_SIZE);
  x3 = (x3 & (1 << CHUNK_LAST_BIT)) === 0 ? x3 : x3 | (~0 << CHUNK_SIZE);
  const d0 = delta[0];
  const d1 = delta[1];
  const d2 = delta[2];
  const d3 = delta[3];
  let res = 0;
  if (x0 < d0) {
    res += d0 - x0;
  }
  if (x1 < d1) {
    res += d1 - x1;
  }
  if (x2 < d2) {
    res += d2 - x2;
  }
  if (x3 < d3) {
    res += d3 - x3;
  }
  return res;
}

function scalarProduct(moveVector, origContextBin, curContextBin) {
  const origInv = origContextBin.me & INV_MASK;
  let origX0 = (origInv & ~(~0 << 5)) >> 0;
  let origX1 = (origInv & ~(~0 << 10)) >> 5;
  let origX2 = (origInv & ~(~0 << 15)) >> 10;
  let origX3 = (origInv & ~(~0 << 20)) >> 15;
  origX0 =
    (origX0 & (1 << CHUNK_LAST_BIT)) === 0
      ? origX0
      : origX0 | (~0 << CHUNK_SIZE);
  origX1 =
    (origX1 & (1 << CHUNK_LAST_BIT)) === 0
      ? origX1
      : origX1 | (~0 << CHUNK_SIZE);
  origX2 =
    (origX2 & (1 << CHUNK_LAST_BIT)) === 0
      ? origX2
      : origX2 | (~0 << CHUNK_SIZE);
  origX3 =
    (origX3 & (1 << CHUNK_LAST_BIT)) === 0
      ? origX3
      : origX3 | (~0 << CHUNK_SIZE);
  const curInv = curContextBin.me & INV_MASK;
  let curX0 = (curInv & ~(~0 << 5)) >> 0;
  let curX1 = (curInv & ~(~0 << 10)) >> 5;
  let curX2 = (curInv & ~(~0 << 15)) >> 10;
  let curX3 = (curInv & ~(~0 << 20)) >> 15;
  curX0 =
    (curX0 & (1 << CHUNK_LAST_BIT)) === 0 ? curX0 : curX0 | (~0 << CHUNK_SIZE);
  curX1 =
    (curX1 & (1 << CHUNK_LAST_BIT)) === 0 ? curX1 : curX1 | (~0 << CHUNK_SIZE);
  curX2 =
    (curX2 & (1 << CHUNK_LAST_BIT)) === 0 ? curX2 : curX2 | (~0 << CHUNK_SIZE);
  curX3 =
    (curX3 & (1 << CHUNK_LAST_BIT)) === 0 ? curX3 : curX3 | (~0 << CHUNK_SIZE);
  const x0 = curX0 - origX0;
  const x1 = curX1 - origX1;
  const x2 = curX2 - origX2;
  const x3 = curX3 - origX3;
  return (
    moveVector[0] * x0 +
    moveVector[1] * x1 +
    moveVector[2] * x2 +
    moveVector[3] * x3
  );
}

function scoreInv(contextBin) {
  const invBin = contextBin.me & INV_MASK;
  let score = 0;
  for (let idx = 0; idx < 4; ++idx) {
    const n = (invBin >> (idx * CHUNK_SIZE)) & CHUNK_MASK;
    score += n * 2 ** idx;
  }
  return score;
}

function runSimuBin(contextBin, targetPotionBin, targetPotion, timeLimit) {
  // ***** Compute move vector *****
  const delta = toObj(targetPotionBin).delta;
  const inv = toObj(contextBin.me).inv;
  const moveVector = [0, 0, 0, 0];

  let norm = 0;
  for (let idx = 0; idx < 4; ++idx) {
    delta[idx] *= -1;
    moveVector[idx] = delta[idx] - inv[idx];
    norm += moveVector[idx] * moveVector[idx];
  }
  norm = Math.sqrt(norm);
  // Normalised move vector
  for (let idx = 0; idx < 4; ++idx) {
    moveVector[idx] /= norm;
  }

  // ***** Start simulation *****
  const start = process.hrtime();

  let bestContextBin;
  // let bestScore = -Infinity;
  let bestDist = Infinity;

  const alreadyVisited = {};
  alreadyVisited[hashContextBin(contextBin)] = 1;

  if (isAffordableBin(targetPotionBin, contextBin.me)) {
    return [true, contextBin, 0.0];
  }

  const contextsToVisit = [];
  const possibleActions = getPossibleActionsBin(contextBin);
  for (action of possibleActions) {
    contextsToVisit.push(applyActionBin(contextBin, action));
  }

  let count = 0;
  const solutions = [];
  let lengthMinSolution = Infinity;
  let currentContextBin;
  while (true && 0 < contextsToVisit.length) {
    currentContextBin = contextsToVisit.shift();
    if (isAffordableBin(targetPotionBin, currentContextBin.me)) {
      // dbg("SOLUTION FOUND");
      // dbg(
      //   "currentContextBin.actionsDone.length",
      //   currentContextBin.actionsDone.length
      // );
      // dbg("lengthMinSolution", lengthMinSolution);
      if (currentContextBin.actionsDone.length <= lengthMinSolution) {
        lengthMinSolution = currentContextBin.actionsDone.length;
        solutions.push(currentContextBin);
      } else {
        const time = process.hrtime(start);
        const timeInMillisecond = time[1] / 1000000;
        dbg("Simulation time", timeInMillisecond);
        const sortedSolution = solutions.sort((s0, s1) => {
          if (scoreInv(s0) < scoreInv(s1)) {
            return -1;
          } else if (scoreInv(s0) === scoreInv(s1)) {
            return 0;
          } else {
            return 1;
          }
        });
        dbg(
          "solution",
          sortedSolution[sortedSolution.length - 1].actionsDone.map(
            actionToObj
          ),
          targetPotion.price
        );
        return [
          true,
          sortedSolution[sortedSolution.length - 1],
          timeInMillisecond,
        ];
      }
      // currentContextBin = contextsToVisit.shift();
    } else {
      // If current exploration slower than the current solutions
      if (lengthMinSolution < currentContextBin.actionsDone.length) {
        const time = process.hrtime(start);
        const timeInMillisecond = time[1] / 1000000;
        dbg("Simulation time", timeInMillisecond);
        const sortedSolution = solutions.sort((s0, s1) => {
          if (scoreInv(s0) < scoreInv(s1)) {
            return -1;
          } else if (scoreInv(s0) === scoreInv(s1)) {
            return 0;
          } else {
            return 1;
          }
        });
        dbg(
          "solution",
          sortedSolution[sortedSolution.length - 1].actionsDone.map(
            actionToObj
          ),
          targetPotion.price
        );
        return [
          true,
          sortedSolution[sortedSolution.length - 1],
          timeInMillisecond,
        ];
      }

      // const currentScore = scalarProduct(
      //   moveVector,
      //   contextBin,
      //   currentContextBin
      // );
      // if (bestScore < currentScore) {
      //   bestScore = currentScore;
      //   bestContextBin = { ...currentContextBin };
      // }
      const currentDist = distToPotion(currentContextBin, delta);
      if (currentDist < bestDist) {
        bestDist = currentDist;
        bestContextBin = { ...currentContextBin };
      } else if (currentDist === bestDist) {
        if (scoreInv(bestContextBin) < scoreInv(currentContextBin)) {
          bestDist = currentDist;
          bestContextBin = { ...currentContextBin };
        }
      }

      count += 1;
      // count % 32
      if ((count & COUNT_MASK) === 0) {
        const time = process.hrtime(start);

        const timeInMillisecond = time[1] / 1000000;
        if (timeLimit < timeInMillisecond) {
          dbg("Time limit exceeded", timeInMillisecond);
          if (solutions.length === 0) {
            return [false, bestContextBin, timeInMillisecond];
          } else {
            const sortedSolution = solutions.sort((s0, s1) => {
              if (scoreInv(s0) < scoreInv(s1)) {
                return -1;
              } else if (scoreInv(s0) === scoreInv(s1)) {
                return 0;
              } else {
                return 1;
              }
            });
            // dbg(
            //   "solution",
            //   sortedSolution[sortedSolution.length - 1].actionsDone.map(
            //     actionToObj
            //   ),
            //   targetPotion.price
            // );
            return [
              false,
              sortedSolution[sortedSolution.length - 1],
              timeInMillisecond,
            ];
          }
        }
        // dbg("");
      }

      if (!alreadyVisited[hashContextBin(currentContextBin)]) {
        alreadyVisited[hashContextBin(currentContextBin)] = 1;
      }

      const possibleActions = getPossibleActionsBin(currentContextBin);
      for (const action of possibleActions) {
        const nextCandidatContext = applyActionBin(currentContextBin, action);

        if (alreadyVisited[hashContextBin(nextCandidatContext)]) {
          continue;
        }

        contextsToVisit.push(nextCandidatContext);
      }
    }
  }

  throw "REACH END RUNSIMU";
}

function arrayToBinary(a) {
  let res = 0;
  for (let i = 0; i < a.length; ++i) {
    const val = a[i];
    const mantissa = val & ~(~0 << CHUNK_LAST_BIT);
    const sign = (val & (1 << 31)) !== 0 ? 1 : 0;
    const compactNb = mantissa | (sign << CHUNK_LAST_BIT);
    res |= compactNb << (i * CHUNK_SIZE);
  }
  return res;
}

function toBinary(obj) {
  if (obj.playerId === "ME") {
    const invInt = arrayToBinary(obj.inv);
    const scoreInt = obj.score << SCORE_FIRST_BIT;
    const meInt = ME_ID << ID_FIRST_BIT;
    const res = invInt | scoreInt | meInt;
    return res;
  }
  if (obj.actionType === "CAST") {
    const deltaInt = arrayToBinary(obj.delta);
    const actionIdInt = obj.actionId << ACTION_ID_FIRST_BIT;
    const castableInt = (obj.castable ? 1 : 0) << CASTABLE_BIT;
    const repeatableInt = (obj.repeatable ? 1 : 0) << REPEATABLE_BIT;
    const castInt = CAST_ID << ID_FIRST_BIT;
    const res = deltaInt | actionIdInt | castableInt | repeatableInt | castInt;
    return res;
  }
  if (obj.actionType === "BREW") {
    const deltaInt = arrayToBinary(obj.delta);
    const actionIdInt = obj.actionId << ACTION_ID_FIRST_BIT;
    const brewInt = BREW_ID << ID_FIRST_BIT;
    const res = deltaInt | actionIdInt | brewInt;
    return res;
  }
  if (obj.actionType === "LEARN") {
    const deltaInt = arrayToBinary(obj.delta);
    const actionIdInt = obj.actionId << ACTION_ID_FIRST_BIT;
    const tomeIndexInt = obj.tomeIndex << TOME_INDEX_FIRST_BIT;
    const taxCountInt = obj.taxCount << TOME_INDEX_FIRST_BIT;
    const learnInt = LEARN_ID << ID_FIRST_BIT;
    const res = deltaInt | actionIdInt | tomeIndexInt | taxCountInt | learnInt;
    return res;
  }
}

function arrayToDecimal(compact) {
  let res = [0, 0, 0, 0];
  for (let i = 0; i < 4; ++i) {
    const compactNb = (compact >> (i * CHUNK_SIZE)) & CHUNK_MASK;
    const mantissa = compactNb & ~(~0 << CHUNK_LAST_BIT);
    const sign = (compactNb & (1 << CHUNK_LAST_BIT)) !== 0 ? 1 : 0;
    const val = sign ? mantissa | (~0 << CHUNK_LAST_BIT) : mantissa;
    res[i] = val;
  }
  return res;
}

function toObj(compact) {
  const objId = (compact >> ID_FIRST_BIT) & ID_MASK;
  //   dbg("objId", objId);
  if (objId === ME_ID) {
    const playerId = "ME";
    const invInt = (compact >> 0) & INV_MASK;
    const inv = arrayToDecimal(invInt);
    const score = (compact >> SCORE_FIRST_BIT) & ~(~0 << SCORE_SIZE);
    return { playerId, inv, score };
  }
  if (objId === CAST_ID) {
    const actionType = "CAST";
    const deltaInt = (compact >> 0) & INV_MASK;
    const delta = arrayToDecimal(deltaInt);
    const actionId = (compact >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
    const castable = (compact >> CASTABLE_BIT) & 1 ? true : false;
    const repeatable = (compact >> REPEATABLE_BIT) & 1 ? true : false;
    return { actionId, actionType, delta, castable, repeatable };
  }
  if (objId === BREW_ID) {
    const actionType = "BREW";
    const deltaInt = (compact >> 0) & INV_MASK;
    const delta = arrayToDecimal(deltaInt);
    const actionId = (compact >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
    return { actionId, actionType, delta };
  }
  if (objId === LEARN_ID) {
    const actionType = "LEARN";
    const deltaInt = (compact >> 0) & INV_MASK;
    const delta = arrayToDecimal(deltaInt);
    const actionId = (compact >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
    const tomeIndex =
      (compact >> TOME_INDEX_FIRST_BIT) & ~(~0 << TOME_INDEX_SIZE);
    const taxCount = (compact & (1 << TAX_COUNT_BIT)) >> TAX_COUNT_BIT;
    return { actionId, actionType, delta, tomeIndex, taxCount };
  }
}

function selectCastToLearn(me, grimoire, iter) {
  // const sortedGrimoire = grimoire
  //   .slice(0, 2)
  //   .sort((c0, c1) => {
  //     if (dist(c0.delta) < dist(c1.delta)) {
  //       return -1;
  //     } else if (dist(c0.delta) === dist(c1.delta)) {
  //       return 0;
  //     } else {
  //       return 1;
  //     }
  //   })
  //   .reverse();
  //   dbg("sortedGrimoire", sortedGrimoire);
  let castToLearn = null;
  if (iter <= LIMIT_BEGIN_GAME) {
    // dbg("dist(grimoire[1].delta)", dist(grimoire[1].delta));
    // castToLearn = sortedGrimoire[0];
    // castToLearn = grimoire[0];
    // const allPositive = grimoire[1].delta.reduce((acc, val) => {
    //   return acc & (0 <= val);
    // }, true);
    // if (allPositive) {
    //   castToLearn = grimoire[1];
    // }
    // dbg("castToLearn", castToLearn);

    castToLearn = grimoire[0];
    for (let idx = 0; idx < 3; ++idx) {
      const allPositive = grimoire[idx].delta.reduce((acc, val) => {
        return acc & (0 <= val);
      }, true);
      if (allPositive && isLearnable(grimoire[idx], me)) {
        castToLearn = grimoire[idx];
      }
    }
  }

  // if (castToLearn && !isLearnable(castToLearn, me)) {
  //   // castToLearn = null;
  //   castToLearn = grimoire[0];
  // }

  return castToLearn;
}

// ********** GAME LOOP **********

while (true) {
  const inputs = parseInput();
  ++iter;
  dbg("iter", iter);
  dbg("nbPotionDone", nbPotionDone);

  const me = getMe(inputs);
  const opponent = getOpponent(inputs);
  const casts = getCasts(inputs);
  const grimoire = getGrimoire(inputs);
  const potions = getPotions(inputs);

  const castToLearn = selectCastToLearn(me, grimoire, iter);

  if (previousPotionsActionId.length === 5) {
    const actionIds = potions.map((p) => p.actionId);
    let inCommon = 0;
    for (const previousActionId of previousPotionsActionId) {
      if (actionIds.indexOf(previousActionId) !== -1) {
        ++inCommon;
      }
    }

    const nbDiff = 5 - inCommon;
    if (nbDiff === 1) {
      // The opponent has brew
      if (!hasBrew) {
        ++nbOpponentPotionDone;
        //dbg("OPPONENT HAS BREW");
      }
    } else if (nbDiff === 2) {
      ++nbOpponentPotionDone;
      //dbg("BOTH OF US HAS BREW");
    }
  }
  dbg("nbOpponentPotionDone", nbOpponentPotionDone);
  hasBrew = false;

  let effectivePotions = potions;

  if (
    LIMIT_END_GAME <= nbOpponentPotionDone ||
    LIMIT_END_GAME <= nbPotionDone
  ) {
    const opponentEndGamePoint =
      opponent.inv[1] + opponent.inv[2] + opponent.inv[3];
    const diffScore = opponent.score - me.score + opponentEndGamePoint + 13;
    dbg("diffScore", diffScore);
    const filtedPotions = potions.filter((p) => {
      return diffScore < p.price;
    });
    // dbg("filtedPotions", filtedPotions);
    // dbg("potions", potions);

    if (filtedPotions.length === 0) {
      const maxPricePotion = potions.reduce(
        (p, curP) => {
          if (p.price < curP.price) {
            return { ...curP };
          } else {
            return { ...p };
          }
        },
        { ...potions[0] }
      );
      effectivePotions = [maxPricePotion];
      currentBestSolutionActionId = null;
      currentBestSolutionIdx = 0;
      // dbg("effectivePotions", effectivePotions);
    } else {
      effectivePotions = filtedPotions;
      currentBestSolutionActionId = null;
      currentBestSolutionIdx = 0;
    }
  }

  const sortedPotion = effectivePotions.sort((p0, p1) => {
    // sort increasingly according to score: low score first
    if (score(p0, me) < score(p1, me)) {
      return -1;
    } else if (score(p0, me) === score(p1, me)) {
      // sort decreasingly according to price: big price first
      if (p0.price < p1.price) {
        return 1;
      } else if (p0.price === p1.price) {
        return 0;
      } else {
        return -1;
      }
    } else {
      return 1;
    }
    // sort decreasingly according to price: small price first
    // if (p0.price < p1.price) {
    //   return 1;
    // } else if (p0.price === p1.price) {
    //   return 0;
    // } else {
    //   return -1;
    // }
  });
  // .slice(Math.max(0, effectivePotions.length - 3), effectivePotions.length)
  // .slice(0, Math.min(effectivePotions.length, 2))
  // if (LIMIT_END_GAME <= nbOpponentPotionDone || LIMIT_END_GAME <= nbPotionDone) {
  //   dbg("sortedPotion", sortedPotion);
  // }

  if (castToLearn) {
    console.log("LEARN", castToLearn.actionId);
  } else {
    let timeLimit = 38.0;

    let bestPotion = null;
    let bestActionsDone = null;
    let bestContext = null;

    // let idx = currentBestSolutionIdx;
    if (!currentBestSolutionActionId) {
      currentBestSolutionActionId =
        sortedPotion[currentBestSolutionIdx].actionId;
    }

    // The opponent has realised a potion so the list of potion has changed
    if (
      currentBestSolutionActionId !==
      sortedPotion[currentBestSolutionIdx].actionId
    ) {
      dbg("currentBestSolutionActionId", currentBestSolutionActionId);
      let found = false;
      for (let i = 0; i < sortedPotion.length; ++i) {
        p = sortedPotion[i];
        if (p.actionId === currentBestSolutionActionId) {
          currentBestSolutionIdx = i;
          found = true;
          break;
        }
      }
      // The target potion has been done by the opponent
      if (!found) {
        dbg("The target potion has been done by the opponent");
        currentBestSolutionIdx = 0;
        currentBestSolutionActionId = sortedPotion[0].action;
      }
    }
    let idx = currentBestSolutionIdx;
    let nbSimu = 0;
    while (20.0 <= timeLimit && nbSimu < sortedPotion.length) {
      nbSimu += 1;
      const iterTargetPotion = sortedPotion[idx];
      const iterTargetPotionBin = toBinary(sortedPotion[idx]);
      const [iterSolutionFound, iterContextBin, iterSimuTime] = runSimuBin(
        {
          me: toBinary(me),
          toPrepare: casts.map((cast) => toBinary(cast)),
          actionsDone: [],
        },
        iterTargetPotionBin,
        iterTargetPotion,
        timeLimit
      );
      const iterActionsDone = iterContextBin.actionsDone;
      timeLimit -= iterSimuTime;

      if (!bestPotion) {
        dbg("BETTER solution", idx);
        dbg("");
        bestPotion = iterTargetPotion;
        bestActionsDone = iterActionsDone;
        bestContext = iterContextBin;
        currentBestSolutionIdx = idx;
        currentBestSolutionActionId = bestPotion.actionId;
        idx = (idx + 1) % sortedPotion.length;
        if (iterActionsDone.length === 0) {
          break;
        } else {
          continue;
        }
      }

      if (!iterSolutionFound) {
        break;
      }

      if (iterActionsDone && iterActionsDone.length < bestActionsDone.length) {
        dbg("BETTER solution", idx);
        dbg("");
        bestPotion = iterTargetPotion;
        bestActionsDone = iterActionsDone;
        bestContext = iterContextBin;
        currentBestSolutionIdx = idx;
        currentBestSolutionActionId = bestPotion.actionId;
      } else if (
        iterActionsDone &&
        iterActionsDone.length === bestActionsDone.length
      ) {
        // if (iterTargetPotionBin.price < bestPotion.price) {
        //   dbg("solution 0");
        //   targetPotion = bestPotion;
        //   actionsDone = bestActionsDone;
        // }
        // Compare Inventory
        // else if (sortedPotion[1].price === sortedPotion[0].price) {
        //   if ()
        // }
        // dbg("LENGTH EQUALITY");
        // dbg("bestPotion.price", bestPotion.price);
        // dbg("iterTargetPotionBin.price", iterTargetPotion.price);
        // dbg("comparison", bestPotion.price < iterTargetPotion.price);
        if (bestPotion.price < iterTargetPotion.price) {
          dbg("BETTER solution", idx);
          dbg("");
          bestPotion = iterTargetPotion;
          bestActionsDone = iterActionsDone;
          bestContext = iterContextBin;
          currentBestSolutionIdx = idx;
          currentBestSolutionActionId = bestPotion.actionId;
        } else if (bestPotion.price === iterTargetPotion.price) {
          if (scoreInv(bestContext) < scoreInv(iterTargetPotion)) {
            dbg("NEW CASE UPDATE BETTER solution", idx);
            dbg("");
            bestPotion = iterTargetPotion;
            bestActionsDone = iterActionsDone;
            bestContext = iterContextBin;
            currentBestSolutionIdx = idx;
            currentBestSolutionActionId = bestPotion.actionId;
          }
        }
      }
      // else {
      //   dbg("solution 0");
      //   targetPotion = bestPotion;
      //   actionsDone = actionsDone0;
      // }
      idx = (idx + 1) % sortedPotion.length;
    } // while (10.0 <= timeLimit && idx < 4) {
    // } // if (solutionFound) {

    let targetPotion = bestPotion;
    let actionsDone = bestActionsDone;
    // dbg("actionsDone", actionsDone);

    if (actionsDone.length === 0) {
      currentBestSolutionIdx = 0;
      currentBestSolutionActionId = null;
      hasBrew = true;
      ++nbPotionDone;
      console.log("BREW", targetPotion.actionId);
    } else {
      const actionBin = actionsDone[0];
      const id = (actionBin >> ANSWER_ID_FIRST_BIT) & ANSWER_ID_MASK;
      if (id === 0) {
        throw "THROW while loop";
      }
      if (id === REST_ID) {
        console.log("REST");
      } else if (id === CAST_ID) {
        const castId = (actionBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
        const times = actionBin & TIME_MASK;

        if (times === 0) {
          console.log("CAST", castId);
        } else {
          console.log("CAST", castId, times);
        }
      }
    }
  }
  previousPotionsActionId = [...potions.map((p) => p.actionId)];
}
