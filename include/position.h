#ifndef POSITION_H
#define POSITION_H

# include "board.h"
# include "move.h"
# include "legalmover.h"



class Position
{
    friend class LegalMover;

public:
    Position();
    Position(bool gamestart);

    bool operator==(Position const& other) const;

    std::size_t getHash() const;
    Board getBoard() const {return board;}
    bool getDrawClaimable() const {return drawClaimable;}
    uint getNbReversiblePlies() const {return nbReversiblePlies;}
    uint getMoveNumber() const {return moveNumber;}
    bool getTurn() const {return turn;}
    uint8f getPiece(uint8f square) const {return board.pieces[square];}
    std::size_t recalculateHash(bool recalculateBoardHash) const;

    bool printMovePGN(std::string &res, const Move &move, bool printMoveNumber) const;
    bool printLinePGN(std::string &res, const std::vector<Move> &line) const;
    std::string toFENstring() const;
    std::string toLichessURL() const;
    static bool fromFENstring(Position &res, const std::string &FENstr);

private:
    void updateDrawClaimable();

    void applyMove(Position &res, const Move &move) const;

    Board board;

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
    std::unordered_map<Board, uint> repetitions;
    // These are inessential and not taken into account in the hash
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



#endif // POSITION_H
