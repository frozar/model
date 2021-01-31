
// un "Node", c'est un coup de la partie. Donc ça représente la position de deux billes qui sont tombées
//  pas besoin de scanner les groupes autres que ceux formés par ces deux billes puisqu'il n'y a rien de nouveau par ailleurs
//  Je ne pense pas, j'ai copié/coller ma class "Board" mais il te manque tout le reste pour l'utiliser
// frozar 00H35
// ok ok, déjà, c'est énorme de pas devoir scan toute la grille ^^
// frozar 00H38
// whaaaa, comme tu dis, c'est touffu le code...
// pb4GOD 00H38
// bon allez dodo
//  Bon courage :D
//  Allez un indice :
// frozar 00H38
// merci, bonne nuit, jete dirais si je survis à la lecture du code
// pb4GOD 00H39
// j'utile un tableau 1D pour stocker ma grille et celle de l'adversaire
//  PXY ça représente 8bits pour dire où je suis sur la grille
//  les bits 0 à 3 représentent la coordonnée Y
//  les bits 4 à 6 la coordonnée X
//  et le bit 7 le numéro de joueur
// frozar 00H40
// ok, c'est de ce niveau là.....
//  ça va piquer, merci
// pb4GOD 00H41
// donc pour aller une case à droite, tu fais pxy += 16, une case en haut je fais pxy -= 1 (par exemple, les valeurs sont à vérifier)
//  Par ailleurs
//  La grille est un peu plus grande que ce que disent les règles
//  ça m'évite de tester si je suis en dehors de la grille quand je regarde sur les côtés
//  bon allez dodo 
// frozar 00H42
// ok ok, merci en tout cas
//  :)

/*******************************************************************************
*
*     BOARD REPRESENTATION
*
*******************************************************************************/
typedef uint8_t tile_t;
typedef uint8_t bit_coord_t;
typedef uint8_t coord_t;
typedef double score_t;

constexpr int MAX_PLAYERS = 2;
constexpr coord_t WIDTH = 6;
constexpr coord_t HEIGHT = 12;

constexpr coord_t X0 = 1;
constexpr coord_t Y0 = 1;

//              Y=15    0000000000000000
//                      0000000000000000
//                      0000000000000000
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//                      0MMMMMM00MMMMMM0
//              Y=0     0000000000000000
//
//                     PX=0           PX=15
//                        P=0      P=1
//                      X=0  X=7 X=0   X=7




class Board {
  // Class used to manipulate the Board state
public:
  Tile board[0x100];

  bool explored[0x100];
  int colorPresent;

  bit_coord_t visitQueue[0x100];
  bit_coord_t *visitQueue_ptr, *visitQueue_ptr_end;

  bit_coord_t shapeQueue[0x100];
  bit_coord_t *shapeQueue_ptr_begin, *shapeQueue_ptr, *shapeQueue_ptr_end;

  bit_coord_t pxy_topTile[0x10];

  Tile boardSave[1][0x100];
  bit_coord_t heightSave[1][0x10];
  uint8_t indexSave = 0;
  int nodeCounter;

  int colorBonuses[32] = {0, 0, 0, 2, 0, 2, 2, 4, 0, 2, 2, 4, 2, 4, 4, 8,
                          0, 2, 2, 4, 2, 4, 4, 8, 2, 4, 4, 8, 4, 8, 8, 16};

  Tile nextTiles[N_TILES_KNOWN];

  int score[2 * MAX_PLAYERS] = {0, 0, 0, 0};
  int nuisancePoints[2 * MAX_PLAYERS] = {0, 0, 0, 0};

  int nColor[MAX_PLAYERS];
  int nSkull[MAX_PLAYERS];
  int maxHeight[MAX_PLAYERS];
  int minHeights[MAX_PLAYERS];

  void doNode(Node &node) {
    bit_coord_t pxy;
    bit_coord_t pxy_from;
    coord_t shapeLength;
    Tile tile;
    bit_coord_t pxy_top;
    int formula_B;
    int formula_GB;
    uint8_t stepNumber = 0;

    ++nodeCounter;

    node.scoreThisTurn = 0;

    // Appliquer les modifs, remplir la visitQueue
    visitQueue_ptr = visitQueue;
    visitQueue_ptr_end = visitQueue;

    if (node.rotation != 3) {
      // La tile centrale est "en bas"
      coord_t px = X0 + node.centerX + node.pId * 0x08;
      board[pxy_topTile[px]] = nextTiles[2 * node.turn];
      *visitQueue_ptr_end = pxy_topTile[px];
      ++pxy_topTile[px];
      if (get_Y(pxy_topTile[px]) > Y0 + HEIGHT) {
        node.overTop = true;
        return;
      }
      ++visitQueue_ptr_end;

      px += 1 - node.rotation;
      board[pxy_topTile[px]] = nextTiles[2 * node.turn + 1];
      *visitQueue_ptr_end = pxy_topTile[px];
      ++pxy_topTile[px];
      if (get_Y(pxy_topTile[px]) > Y0 + HEIGHT) {
        node.overTop = true;
        return;
      }
      ++visitQueue_ptr_end;
    } else {
      // La tile centrale est "en bas"
      coord_t px = X0 + node.centerX + node.pId * 0x08;
      board[pxy_topTile[px]] = nextTiles[2 * node.turn + 1];
      *visitQueue_ptr_end = pxy_topTile[px];
      ++pxy_topTile[px];
      if (get_Y(pxy_topTile[px]) > Y0 + HEIGHT) {
        node.overTop = true;
        return;
      }
      ++visitQueue_ptr_end;

      board[pxy_topTile[px]] = nextTiles[2 * node.turn];
      *visitQueue_ptr_end = pxy_topTile[px];
      ++pxy_topTile[px];
      if (get_Y(pxy_topTile[px]) > Y0 + HEIGHT) {
        node.overTop = true;
        return;
      }
      ++visitQueue_ptr_end;
    }

    while (visitQueue_ptr != visitQueue_ptr_end) {
      // La Queue n'est pas vide
      formula_B = 0;
      formula_GB = 0;
      colorPresent = 0;
      shapeQueue_ptr_begin = shapeQueue;
      std::memset(explored + 0x80 * node.pId, 0, sizeof(bool) * 0x80);

      // 1- Trouver les formes et les stocker dans shapeQueue entre
      // &shapeQueue[0] et shapeQueue_ptr_begin
      while (visitQueue_ptr != visitQueue_ptr_end) {
        // On fait un passage complet dans la queue
        if (!explored[*visitQueue_ptr]) {
          // Démarrer l'exploration de la forme
          shapeQueue_ptr = shapeQueue_ptr_begin;
          shapeQueue_ptr_end = shapeQueue_ptr_begin + 1;
          *shapeQueue_ptr = *visitQueue_ptr;
          tile = board[*shapeQueue_ptr];
          shapeLength = 1;
          explored[*visitQueue_ptr] = true;
          while (shapeQueue_ptr != shapeQueue_ptr_end) {
            // Boucle d'exploration de la forme
            pxy = *shapeQueue_ptr;
            if (board[pxy] != Tile::Skull) {
              // 0x01 : Y+1
              if (tile == board[pxy + 0x01] && !explored[pxy + 0x01]) {
                *shapeQueue_ptr_end = pxy + 0x01;
                ++shapeQueue_ptr_end;
                explored[pxy + 0x01] = true;
                ++shapeLength;
              } else if (Tile::Skull == board[pxy + 0x01]) {
                *shapeQueue_ptr_end = pxy + 0x01;
                ++shapeQueue_ptr_end;
              }
              // 0xFF : Y-1
              if (tile == board[pxy - 0x01] && !explored[pxy - 0x01]) {
                *shapeQueue_ptr_end = pxy - 0x01;
                ++shapeQueue_ptr_end;
                explored[pxy - 0x01] = true;
                ++shapeLength;
              } else if (Tile::Skull == board[pxy - 0x01]) {
                *shapeQueue_ptr_end = pxy - 0x01;
                ++shapeQueue_ptr_end;
              }
              // 0x10 : X+1
              if (tile == board[pxy + 0x10] && !explored[pxy + 0x10]) {
                *shapeQueue_ptr_end = pxy + 0x10;
                ++shapeQueue_ptr_end;
                explored[pxy + 0x10] = true;
                ++shapeLength;
              } else if (Tile::Skull == board[pxy + 0x10]) {
                *shapeQueue_ptr_end = pxy + 0x10;
                ++shapeQueue_ptr_end;
              }
              // 0xF0 : X-1
              if (tile == board[pxy - 0x10] && !explored[pxy - 0x10]) {
                *shapeQueue_ptr_end = pxy - 0x10;
                ++shapeQueue_ptr_end;
                explored[pxy - 0x10] = true;
                ++shapeLength;
              } else if (Tile::Skull == board[pxy - 0x10]) {
                *shapeQueue_ptr_end = pxy - 0x10;
                ++shapeQueue_ptr_end;
              }
            }
            ++shapeQueue_ptr;
          }

          // La forme a été explorée
          if (shapeLength >= MIN_SHAPE_LENGTH) {
            // La forme est acceptable, on l'enregistre.
            shapeQueue_ptr_begin = shapeQueue_ptr_end;
            formula_B += shapeLength;
            formula_GB += shapeLength - 4;
            colorPresent =
                colorPresent |
                (1 << (static_cast<int>(tile) - static_cast<int>(Tile::Red)));
          }
        }
        ++visitQueue_ptr;
      }

      if (shapeQueue_ptr_begin != shapeQueue) {
        // Au moins une shape a été détectée

        // Calcul du score
        int formula_CP = (stepNumber == 0) ? 0 : 1 << (2 + stepNumber);
        if (colorPresent > 32 || colorPresent < 0) {
          LOGERR("Erreur colorpresent");
        }
        int formula_CB = colorBonuses[colorPresent];

        node.scoreThisTurn +=
            (10 * formula_B) *
            std::min(999, std::max(1, (formula_CP + formula_CB + formula_GB)));

        // 2- Détruire les tiles qui font partie d'une shape
        // 3- Insérer dans la visitQueue toutes les tile qui ont été déplacées

        fast_sort(shapeQueue, shapeQueue_ptr_begin);

        shapeQueue_ptr = shapeQueue;

        visitQueue_ptr = visitQueue;
        visitQueue_ptr_end = visitQueue;

        while (shapeQueue_ptr != shapeQueue_ptr_begin) {
          // Traiter toute la colonne au dessus de *shapeQueue_ptr

          // D'abord on copie les éléments qui n'ont pas disparu
          // pxy est initialisé : c'est le point le plus bas de la colonne qui
          // doit être mis à jour.
          pxy = *shapeQueue_ptr;
          pxy_top = pxy_topTile[get_PX(pxy)];
          while (*shapeQueue_ptr == *(shapeQueue_ptr + 1) &&
                 (shapeQueue_ptr + 1) != shapeQueue_ptr_begin) {
            ++shapeQueue_ptr;
          }
          ++shapeQueue_ptr;

          // On cherche le premier pxy_from
          pxy_from = pxy + 1;
          while (pxy_from == *shapeQueue_ptr &&
                 shapeQueue_ptr != shapeQueue_ptr_begin) {
            while (*shapeQueue_ptr == *(shapeQueue_ptr + 1) &&
                   (shapeQueue_ptr + 1) != shapeQueue_ptr_begin) {
              ++shapeQueue_ptr;
            }
            ++shapeQueue_ptr;
            ++pxy_from;
          }
          while (pxy_from < pxy_top) {
            board[pxy] = board[pxy_from];
            if (board[pxy] != Tile::Skull) {
              *visitQueue_ptr_end = pxy;

              ++visitQueue_ptr_end;
            }
            // Tant que la "source" n'a pas atteint le haut de la colonne
            // On va en mettre à jour un de plus
            ++pxy;
            // Trouver la "source suivante"
            ++pxy_from;
            while (pxy_from == *shapeQueue_ptr &&
                   shapeQueue_ptr != shapeQueue_ptr_begin) {
              // Première condition : Skip toutes les sources à supprimer.
              // Deuxième condition : s'il n'y a plus de source, il n'y a rien à
              // skip.
              while (*shapeQueue_ptr == *(shapeQueue_ptr + 1) &&
                     (shapeQueue_ptr + 1) != shapeQueue_ptr_begin) {
                ++shapeQueue_ptr;
              }
              ++shapeQueue_ptr;
              ++pxy_from;
            }
          }
          // Il n'y a plus rien à copier, on remplit jusqu'à l'ancienne hauteur
          // avec des Tile::None et on met à jour la hauteur
          pxy_topTile[get_PX(pxy)] = pxy;
          while (pxy < pxy_top) {
            board[pxy] = Tile::None;
            ++pxy;
          }
        }

        ++stepNumber;
      }
    }

    // Update the scores :
    score[node.pId] += node.scoreThisTurn;
    nuisancePoints[node.pId] += node.scoreThisTurn;
    node.nSkullsDropped = nuisancePoints[node.pId] / NUISANCE_SIX_SKULLS;
    nuisancePoints[node.pId] -= node.nSkullsDropped * NUISANCE_SIX_SKULLS;
  }

  bool isSame(const Board &otherBoard) const {
    for (bit_coord_t pxy = 0; pxy < 0xFF; ++pxy) {
      if (board[pxy] != otherBoard.board[pxy]) {
        return false;
      }
    }
    return true;
  }

  void dropSkulls(const int pId, const int nSkulls) {
    bit_coord_t pxymin;
    bit_coord_t temp2;
    bit_coord_t pxymax;
    coord_t pxmin = X0 + pId * 0x08;
    coord_t pxmax = X0 + WIDTH + pId * 0x08;
    for (coord_t px = pxmin; px < pxmax; ++px) {
      pxymin = pxy_topTile[px];
      pxymax = (pxy_topTile[px] + static_cast<bit_coord_t>(nSkulls));
      temp2 = get_PXY(px, Y0 + HEIGHT);
      if (pxymax > temp2) {
        pxymax = temp2;
      }
      for (bit_coord_t pxy = pxymin; pxy < pxymax; ++pxy) {
        board[pxy] = Tile::Skull;
      }
      pxy_topTile[px] = pxymax;
    }
  }

  void saveBoard() {
    memcpy(boardSave[0], board, 0x100 * sizeof(Tile));
    memcpy(heightSave[0], pxy_topTile, 0x10 * sizeof(bit_coord_t));
    score[2] = score[0];
    score[3] = score[1];
    nuisancePoints[2] = nuisancePoints[0];
    nuisancePoints[3] = nuisancePoints[1];
  }

  void loadBoard() {
    memcpy(board, boardSave[0], 0x100 * sizeof(Tile));
    memcpy(pxy_topTile, heightSave[0], 0x10 * sizeof(bit_coord_t));
    score[0] = score[2];
    score[1] = score[3];
    nuisancePoints[0] = nuisancePoints[2];
    nuisancePoints[1] = nuisancePoints[3];
  }

  void dump(std::ostream &oStream) {
    for (int i = 0; i < N_TILES_KNOWN; ++i) {
      oStream << nextTiles[i] << " ";
    }
    oStream << std::endl;
    for (coord_t y = 0x0D; y <= 0x0F; --y) {
      for (coord_t px = 0; px < 0x10; ++px) {
        oStream << board[get_PXY(px, y)];
      }
      oStream << std::endl;
    }
  }

  void read(std::istream &iStream) {
    nColor[0] = 0;
    nColor[1] = 0;
    nSkull[0] = 0;
    nSkull[1] = 0;
    for (int i = 0; i < N_TILES_KNOWN; ++i) {
      iStream >> nextTiles[i];
    }
    for (coord_t y = HEIGHT - 1; y < HEIGHT; --y) {
      for (coord_t x = 0; x < WIDTH; ++x) {
        iStream >> board[get_PXY(0, X0 + x, Y0 + y)];
        if (isColorTile(board[get_PXY(0, X0 + x, Y0 + y)])) {
          ++nColor[0];
        } else if (board[get_PXY(0, X0 + x, Y0 + y)] == Tile::Skull) {
          ++nSkull[0];
        }
      }
    }
    for (coord_t y = HEIGHT - 1; y < HEIGHT; --y) {
      for (coord_t x = 0; x < WIDTH; ++x) {
        iStream >> board[get_PXY(1, X0 + x, Y0 + y)];
        if (isColorTile(board[get_PXY(1, X0 + x, Y0 + y)])) {
          ++nColor[1];
        } else if (board[get_PXY(1, X0 + x, Y0 + y)] == Tile::Skull) {
          ++nSkull[1];
        }
      }
    }
    scanHeight();
    nodeCounter = 0;
  }

  void scanHeight() {
    for (coord_t player = 0; player < MAX_PLAYERS; ++player) {
      for (coord_t x = 0; x < WIDTH; ++x) {
        pxy_topTile[get_PX(player, X0 + x)] = get_PXY(player, X0 + x, Y0);
        while (board[pxy_topTile[get_PX(player, X0 + x)]] != Tile::None &&
               board[pxy_topTile[get_PX(player, X0 + x)]] != Tile::Border) {
          ++pxy_topTile[get_PX(player, X0 + x)];
        }
      }
    }
  }

  void initBoard() {
    for (coord_t y = 0x0F; y <= 0x0F; --y) {
      for (coord_t px = 0; px < 0x10; ++px) {
        board[get_PXY(px, y)] = Tile::Border;
      }
    }
    for (coord_t y = HEIGHT - 1; y < HEIGHT; --y) {
      for (coord_t player = 0; player < MAX_PLAYERS; ++player) {
        for (coord_t x = 0; x < WIDTH; ++x) {
          board[get_PXY(player, X0 + x, Y0 + y)] = Tile::None;
        }
      }
    }
  }

  void fullEvaluateBoard(Node &node) const {
    bit_coord_t pxy;
    bit_coord_t pxy_top;
    coord_t max_px = X0 + node.pId * 0x08 + WIDTH;
    int evaluation = 0;
    //    int nTotal = 0;
    //    int nSkull = 0;
    for (coord_t px = X0 + node.pId * 0x08; px < max_px; ++px) {
      pxy_top = pxy_topTile[px];
      for (pxy = get_PXY(px, Y0); pxy <= pxy_top; ++pxy) {
        evaluation += _precalculatedScore[static_cast<int>(board[pxy])]
                                         [static_cast<int>(board[pxy + 0x01])]
                                         [static_cast<int>(board[pxy - 0x10])]
                                         [static_cast<int>(board[pxy + 0x10])]
                                         [static_cast<int>(board[pxy - 0x01])];
        //        nSkull += (board[pxy] == Tile::Skull);
        //        ++nTotal;
      }
    }
    node.positionalEstimation = evaluation;
  }

private:
  inline bit_coord_t get_PXY(const coord_t P, const coord_t X,
                             const coord_t Y) const
      __attribute((always_inline)) {
    return (P << 7) | (X << 4) | Y;
  }
  inline bit_coord_t get_PXY(const coord_t PX, const coord_t Y) const
      __attribute((always_inline)) {
    return (PX << 4) | Y;
  }
  inline coord_t get_P(const bit_coord_t pxy) const
      __attribute((always_inline)) {
    return (pxy & 0x80) >> 7;
  }
  inline coord_t get_X(const bit_coord_t pxy) const
      __attribute((always_inline)) {
    return (pxy & 0x70) >> 4;
  }
  inline coord_t get_PX(const bit_coord_t pxy) const
      __attribute((always_inline)) {
    return (pxy & 0xF0) >> 4;
  }
  inline coord_t get_PX(const coord_t P, const coord_t X) const
      __attribute((always_inline)) {
    return (P << 3 | X);
  }
  inline coord_t get_Y(const bit_coord_t pxy) const
      __attribute((always_inline)) {
    return (pxy & 0xF);
  }
};
