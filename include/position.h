#ifndef POSITION_H
#define POSITION_H

# include "board.h"
# include "move.h"
# include "legalmover.h"

class MovePicker;

class Position
{
    friend std::ostream & operator<<(std::ostream &out, const Position &p);
    friend class LegalMover;
    friend class PositionZobrist;

public:
    Position(bool initialize);

    bool operator==(Position const& other) const;

    std::string printString() const;
    std::string toFENstring() const;
    std::string toLichessURL() const;
    static bool fromFENstring(Position &res, const std::string &FENstr);
    uint getMoveNumber() const {return moveNumber;}
    bool getPiece(Piece &res, const Square &square) const {return board.getPiece(res, square);}
    Color getTurn() const {return turn;}
    uint getNbReversibleHalfMoves() const {return nbReversibleHalfMoves;}
    Board getBoard() const {return board;}

    bool applyMove(Position &res, const Move &m, bool checkLegal = false, bool checkKCLegal = false) const;
    bool pickBestMove(Move &res, bool &bestMoveIsForceDraw, MovePicker *picker) const;
    bool getLegalMoves(std::vector<Move> &res) const;

    bool printPGN(std::string &res, const Move &move, bool printMoveNumber) const;
    bool printPGN(std::string &res, const std::vector<Move> &line) const;

    void setDrawClaimable(bool b);
    bool getDrawClaimable() const;

    std::size_t getHash() const;

private:
    void clear();
    void reset();
    void commonReset();
    void resetPieces();

    Board board;
    Color turn;
    std::array<bool, 4> castlingRights; // White short, White long, Black short, Black long
    bool enPassantPossible;
    uint enPassantTargetSquare;
    uint moveNumber;
    uint nbReversibleHalfMoves; // For 50 and 75 move rules for draw

    // The information below is usually not included in a "position", but we need it for our implementation
    bool enPassantKingShort, enPassantKingLong; // In "KC chess", we're allowing "en passant king capture" when the opponent king castled illegally.
    bool drawClaimable; // Records whether a draw can be claimed
};


namespace std
{
template<> struct hash<Position>
{
    std::size_t operator()(Position const& position) const noexcept
    {
        return position.getHash();
    }
};
}


class PositionZobrist
{

public:
    PositionZobrist() {}
    PositionZobrist(const Position *position, const BoardHelper &helper);

    std::size_t recalculateHash() const;

private:
    bool pieces[64][6][2]; // [square][piece type][side to move] !!! 0=Black, 1=White !!!
    bool castlingRights[4]; // White short, White long, Black short, Black long
    bool enPassantFile[8]; // En passant file where capture is possible
    bool side; // !!! true=White, false=Black !!!
    bool drawClaimable; // Records whether a draw can be claimed
};



/*
class PositionZ
{
public:
    PositionZ();

    bool operator==(PositionZ const& other) const;
    bool isEqual(PositionZ const &other) const;


    std::size_t getHash() const;

private:
    void resetBoard();

    std::size_t recalculateHash() const;



    boardZ board; // [square][piece type][side to move] !!! 0=Black, 1=White !!!
    bool castlingRights[4]; // White short, White long, Black short, Black long
    bool enPassantFile[8]; // En passant file where capture is possible
    bool turn; // !!! true=White, false=Black !!!

    uint moveNumber;
    uint nbReversibleHalfMoves; // For 50 and 75 move rules for draw

    // The information below is usually not included in a "position", but we need it for our implementation
    bool enPassantKingShort, enPassantKingLong; // In "KC chess", we're allowing "en passant king capture" when the opponent king castled illegally.
    bool drawClaimable; // Records whether a draw can be claimed

    std::size_t hash;
};*/





#endif // POSITION_H
