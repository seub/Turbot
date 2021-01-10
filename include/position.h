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
    bool enPassantKingShort, enPassantKingLong; // For capturing the king after a pseudo-legal castle
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
    bool enPassantKingShort, enPassantKingLong; // For capturing the king after a pseudo-legal castle
    bool drawClaimable; // Records whether a draw can be claimed
};




class PositionZ
{
    friend class LegalMoverZ;

public:
    PositionZ();
    PositionZ(bool gamestart);

    bool operator==(PositionZ const& other) const;

    std::size_t getHash() const;
    BoardZ getBoard() const {return board;}
    bool getDrawClaimable() const {return drawClaimable;}
    uint getNbReversiblePlies() const {return nbReversiblePlies;}
    uint getMoveNumber() const {return moveNumber;}
    bool getTurn() const {return turn;}
    uint8f getPiece(uint8f square) const {return board.pieces[square];}
    std::size_t recalculateHash(bool recalculateBoardHash) const;

    bool printPGN(std::string &res, const MoveZ &move, bool printMoveNumber) const;
    bool printPGN(std::string &res, const std::vector<MoveZ> &line) const;

private:
    void updateDrawClaimable();

    void applyMove(PositionZ &res, const MoveZ &move) const;

    BoardZ board;

    bool turn;
    // true=White, false=Black
    uint8f castlingRights;
    // 4-bit integer: bit #0 is White short, bit #1 is White Long, bit #2 is Black Short, bit #3 is Black Long. //
    // Example: castlingRights = 6 = 0110 means White Long + Black Short
    uint8f enPassant;
    // 4-bit integer: bit #0 is whether or not en Passant is possible, bits #1--#3 contain the index of the file of the target square
    // Example: enPassant = 13 = 1101 means en Passant is possible, and target file is 110 = 6 = g

    uint8f enPassantKing;
    // Records whether the opponent king castled illegally (but pseudo-legally) on the previous move.
    // 2-bit integer: bit #0 is enPassantKingShort, bit #1 is enPassantKingLong
    // Example: enPassantKing = 2 = 10 means en Passant Long king capture is possible

    bool drawClaimable;
    // Records whether a draw can be claimed

    std::size_t hash;
    // This can be calculated from the other member variables but is it easy to update after playing a move so we store it for speed

    uint moveNumber, nbReversiblePlies;
    bool fiftyDraw, threeFoldDraw;
    std::unordered_map<BoardZ, uint> repetitions;
    // These are inessential and not taken into account in the hash
};

namespace std
{
template<> struct hash<PositionZ>
{
    std::size_t operator()(PositionZ const& position) const noexcept
    {
        return position.getHash();
    }
};
}



#endif // POSITION_H
