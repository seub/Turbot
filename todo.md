TODO

Test number of collisions

Remove from position:
uint moveNumber;
uint nbReversibleHalfMoves; // For 50 and 75 move rules for draw
bool applyMove(Position &res, const Move &m, bool checkLegal = false, bool checkKCLegal = false) const;
bool pickBestMove(Move &res, bool &bestMoveIsForceDraw, MovePicker *picker) const;
bool getLegalMoves(std::vector<Move> &res) const;
bool getKCLegalMoves(std::vector<Move> &res) const;


Decide whether PositionZ should contain a boardZ or a BoardZ


PROPOSITIONS DE CLASSES

    
IDÉES :

Try to fork stockfish? At least try to understand some of it

