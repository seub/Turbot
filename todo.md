TODO

Test number of collisions

Remove from position:
uint moveNumber;
uint nbReversibleHalfMoves; // For 50 and 75 move rules for draw

bool applyMove(Position &res, const Move &m, bool checkLegal = false, bool checkKCLegal = false) const;
bool pickBestMove(Move &res, bool &bestMoveIsForceDraw, MovePicker *picker) const;
bool getLegalMoves(std::vector<Move> &res) const;
bool getKCLegalMoves(std::vector<Move> &res) const;

Question : c'est con quand même que le hash ne soit pas unique non ? Du coup à chaque fois qu'il trouve une position déjà calculée
il doit vérifier qu'elle est vraiment la même, mais c'est couteux toutes ces comparaisons !?


PROPOSITIONS DE CLASSES

    
IDÉES :

KC chess:
Quand on voudra optimiser, quand on calcule l'arbre des coups, je pense que ça vaut le coup de passer en "kc chess" (king capture allowed)

Special actions:
claim draw, resign, offer draw ne drevraient pas faire partie de Move, c'est une action séparée. Elle fera partie d'une classe "Game"!

Try to fork stockfish? At least try to understand some of it

