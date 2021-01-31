// To debug
function dbg() {
  console.error("DBG:", ...Object.values(arguments));
}

// ********** GLOBAL STUFF **********

// ********** GLOBAL CONSTANT **********
const LIMIT_BEGIN_GAME = 8;
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

// *** BIT MASK ***
const INV_MASK = ~(~0 << INV_SIZE);
const CHUNK_MASK = ~(~0 << CHUNK_SIZE);
const ACTION_ID_MASK = ~(~0 << ACTION_ID_SIZE);
const COUNT_MASK = ~(~0 << 5);

// ********** END GLOBAL CONSTANT **********

// ********** GLOBAL VARIABLE **********
let iter = 0;
let nbPotionDone = 0;
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
  return balance.reduce((acc, curVal, i) => {
    acc += curVal * 10 ** i;
    return acc;
  }, 0);
}

function score(p) {
  const x0 = -p.delta[0] * 1;
  const x1 = -p.delta[1] * 2;
  const x2 = -p.delta[2] * 4;
  const x3 = -p.delta[3] * 8;
  return x0 + x1 + x2 + x3 - p.price;
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
    const actionId = (castBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
    // If not repeatable
    if ((castBin & (1 << REPEATABLE_BIT)) === 0) {
      if (isAffordableBin(castBin, contextBin.me)) {
        possibleActions.push([CAST_ID, actionId]);
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
      possibleActions.push([CAST_ID, actionId, times]);
    }
  }

  if (restActionPossible) {
    possibleActions.push([REST_ID]);
  }

  return possibleActions;
}

function applyActionBin(contextBin, action) {
  let nextContext = {
    me: contextBin.me,
    toPrepare: [...contextBin.toPrepare],
    actionsDone: [...contextBin.actionsDone],
  };
  if (action[0] === REST_ID) {
    // Set castable to true
    for (let idx = 0; idx < nextContext.toPrepare.length; ++idx) {
      nextContext.toPrepare[idx] |= 1 << CASTABLE_BIT;
    }
  } else if (action[0] === CAST_ID) {
    const castId = action[1];
    const times = action[2];

    let castBin = null;
    let idxTargetCast = null;
    for (let idx = 0; idx < contextBin.toPrepare.length; ++idx) {
      const iterCastBin = contextBin.toPrepare[idx];
      const actionId = (iterCastBin >> ACTION_ID_FIRST_BIT) & ACTION_ID_MASK;
      if (actionId === castId) {
        idxTargetCast = idx;
        castBin = iterCastBin;
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
    break;
  }
  nextContext.actionsDone.push(action);
  return nextContext;
}

function hashContextBin(contextBin) {
  // Simple hash function based on XOR
  return contextBin.toPrepare.reduce(
    (acc, castBin) => acc ^ castBin,
    contextBin.me
  );
}

function printContextBin(contextBin) {
  dbg("contextBin");
  dbg("me", toObj(contextBin.me));
  // dbg("casts");
  // contextBin.toPrepare.map((castBin) => {
  //   printCast(toObj(castBin));
  // });
  dbg("actionsDone", contextBin.actionsDone);
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

function runSimuBin(contextBin, targetPotionBin, timeLimit) {
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
  let bestScore = -Infinity;

  const alreadyVisited = {};
  alreadyVisited[hashContextBin(contextBin)] = 1;

  if (isAffordableBin(targetPotionBin, contextBin.me)) {
    return [contextBin.actionsDone, 0.0];
  }
  const possibleActions = getPossibleActionsBin(contextBin);

  const contextsToVisit = [];
  for (action of possibleActions) {
    contextsToVisit.push(applyActionBin(contextBin, action));
  }

  let currentContextBin = contextsToVisit.shift();
  let count = 0;
  const solutions = [];
  let lengthMinSolution = Infinity;
  while (true) {
    if (isAffordableBin(targetPotionBin, currentContextBin.me)) {
      if (currentContextBin.actionsDone.length <= lengthMinSolution) {
        lengthMinSolution = currentContextBin.actionsDone.length;
        solutions.push(currentContextBin);
      }
      currentContextBin = contextsToVisit.shift();
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
        // for (const s of sortedSolution) {
        //   dbg("solution", s.actionsDone, " score", scoreInv(s));
        // }
        dbg("solution", sortedSolution[sortedSolution.length - 1].actionsDone);
        return [
          sortedSolution[sortedSolution.length - 1].actionsDone,
          timeInMillisecond,
        ];
      }

      const currentScore = scalarProduct(
        moveVector,
        contextBin,
        currentContextBin
      );
      if (bestScore < currentScore) {
        bestScore = currentScore;
        bestContextBin = { ...currentContextBin };
      }

      count += 1;
      // count % 32
      if ((count & COUNT_MASK) === 0) {
        const time = process.hrtime(start);
        // dbg("In SIMU Bin", time);
        // dbg("count", count);
        // dbg("depth", currentContext.actionsDone.length);
        // dbg("bestScore", bestScore);
        // dbg("bestInv", toObj(bestContextBin.me).inv);
        // dbg("bestActionDone", bestContextBin.actionsDone);

        const timeInMillisecond = time[1] / 1000000;
        if (timeLimit < timeInMillisecond) {
          dbg("Time limit exceeded", timeInMillisecond);
          if (solutions.length === 0) {
            return [bestContextBin.actionsDone, timeInMillisecond];
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
            // for (const s of sortedSolution) {
            //   dbg("solution", s.actionsDone, " score", scoreInv(s));
            // }
            dbg(
              "solution",
              sortedSolution[sortedSolution.length - 1].actionsDone
            );
            return [
              sortedSolution[sortedSolution.length - 1].actionsDone,
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
      currentContextBin = contextsToVisit.shift();
    }
  }
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
  const objId = (compact >> ID_FIRST_BIT) & ~(~0 << ID_SIZE);
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
  const sortedGrimoire = grimoire
    .slice(0, 2)
    .sort((c0, c1) => {
      if (dist(c0.delta) < dist(c1.delta)) {
        return -1;
      } else if (dist(c0.delta) === dist(c1.delta)) {
        return 0;
      } else {
        return 1;
      }
    })
    .reverse();
  //   dbg("sortedGrimoire", sortedGrimoire);
  let castToLearn = null;
  if (iter <= LIMIT_BEGIN_GAME) {
    // castToLearn = sortedGrimoire[0];
    castToLearn = grimoire[0];
    // dbg("castToLearn", castToLearn);
  }

  if (castToLearn && !isLearnable(castToLearn, me)) {
    castToLearn = null;
  }

  return castToLearn;
}

// ********** GAME LOOP **********

while (true) {
  const inputs = parseInput();
  iter += 1;
  dbg("iter", iter);

  const me = getMe(inputs);
  const casts = getCasts(inputs);
  const grimoire = getGrimoire(inputs);
  const potions = getPotions(inputs);

  const castToLearn = selectCastToLearn(me, grimoire, iter);

  // const sortedPotion = potions.sort((p0, p1) => {
  //   if (dist(getBalance(p0, me)) < dist(getBalance(p1, me))) {
  //     return -1;
  //   } else if (dist(getBalance(p0, me)) === dist(getBalance(p1, me))) {
  //     return 0;
  //   } else {
  //     return 1;
  //   }
  // });
  // const targetPotion = sortedPotion[sortedPotion.length - 1];

  // const targetPotion = potions[0];

  // const sortedPotion = potions.sort((p0, p1) => {
  //   if (p0.price < p1.price) {
  //     return -1;
  //   } else if (p0.price === p1.price) {
  //     // return 0;
  //     if (dist(getBalance(p0, me)) < dist(getBalance(p1, me))) {
  //       return -1;
  //     } else if (dist(getBalance(p0, me)) === dist(getBalance(p1, me))) {
  //       return 0;
  //     } else {
  //       return 1;
  //     }
  //   } else {
  //     return 1;
  //   }
  // });
  const sortedPotion = potions.sort((p0, p1) => {
    // sort increasingly according to score: low score first
    if (score(p0) < score(p1)) {
      return -1;
    } else if (score(p0) === score(p1)) {
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
  });
  // const sortedPotion = potions.sort((p0, p1) => {
  //   if (score(p0) === score(p1)) {
  //     // sort increasingly according to price: big price first
  //     if (p0.price < p1.price) {
  //       return -1;
  //     } else if (p0.price === p1.price) {
  //       return 0;
  //     } else {
  //       return 1;
  //     }
  //   }
  // });
  // dbg("sortedPotion[4]", sortedPotion[4]);
  // for (const p of sortedPotion) {
  //   dbg("p", p.delta, " price", p.price, " score", score(p));
  // }
  // throw "potion";
  // const targetPotion = sortedPotion[0];

  if (castToLearn) {
    console.log("LEARN", castToLearn.actionId);
  } else {
    let timeLimit = 41.0;
    let actionsDone0, simuTime0;
    let actionsDone1, simuTime1;

    const targetPotionBin0 = toBinary(sortedPotion[0]);
    [actionsDone0, simuTime0] = runSimuBin(
      {
        me: toBinary(me),
        toPrepare: casts.map((cast) => toBinary(cast)),
        actionsDone: [],
      },
      targetPotionBin0,
      timeLimit
    );

    // dbg("before timeLimit", timeLimit);
    // dbg("before simuTime0", simuTime0);
    timeLimit -= simuTime0;
    // dbg("substracted timeLimit", timeLimit);
    if (10.0 <= timeLimit) {
      const targetPotionBin1 = toBinary(sortedPotion[1]);
      [actionsDone1, simuTime1] = runSimuBin(
        {
          me: toBinary(me),
          toPrepare: casts.map((cast) => toBinary(cast)),
          actionsDone: [],
        },
        targetPotionBin1,
        timeLimit
      );
    }

    let targetPotion;
    let actionsDone;

    if (actionsDone1 && actionsDone1.length < actionsDone0.length) {
      dbg("solution 1");
      targetPotion = sortedPotion[1];
      actionsDone = actionsDone1;
    } else if (actionsDone1 && actionsDone1.length === actionsDone0.length) {
      if (sortedPotion[1].price < sortedPotion[0].price) {
        dbg("solution 0");
        targetPotion = sortedPotion[0];
        actionsDone = actionsDone0;
      }
      // Compare Inventory
      // else if (sortedPotion[1].price === sortedPotion[0].price) {
      //   if ()
      // }
      else {
        dbg("solution 1");
        targetPotion = sortedPotion[1];
        actionsDone = actionsDone1;
      }
    } else {
      dbg("solution 0");
      targetPotion = sortedPotion[0];
      actionsDone = actionsDone0;
    }

    if (actionsDone.length === 0) {
      console.log("BREW", targetPotion.actionId);
    } else {
      const actionToDo = actionsDone[0];
      // dbg("actionToDo", actionToDo);
      const actionId = actionToDo[0];
      if (actionId === REST_ID) {
        nbPotionDone += 1;
        console.log("REST");
      } else if (actionId === CAST_ID) {
        const castId = actionToDo[1];
        const times = actionToDo[2];

        if (times === undefined) {
          console.log("CAST", castId);
        } else {
          console.log("CAST", castId, times);
        }
      }
    }
  }
}
