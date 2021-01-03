#ifndef POSITION_H
#define POSITION_H

# include "board.h"
# include "move.h"
# include "legalmover.h"

class MovePicker;


//#include <boost/multiprecision/cpp_int.hpp>

class Position
{
    friend std::ostream & operator<<(std::ostream &out, const Position &p);
    friend class LegalMover;

public:
    Position(bool gamestart=false);

    bool operator==(Position const& other) const;
    bool isEqual(Position const &other) const;

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
    //std::size_t getHash() const;
    bool getLegalMoves(std::vector<Move> &res) const;
    bool getKCLegalMoves(std::vector<Move> &res) const;

    bool printPGN(std::string &res, const Move &move, bool printMoveNumber) const;
    bool printPGN(std::string &res, const std::vector<Move> &line) const;

    bool threeFoldRepetition() const; // Says whether a 3-fold repetition has occurred at any points in the past
    bool drawCanBeClaimed() const;

private:
    void clear();
    void reset();
    void commonReset();
    void resetPieces();

    Board board;
    Color turn;
    std::array<bool, 4> castlingRights;// White Kside, White Qside, Black Kside, Black Qside
    bool enPassantPossible;
    uint enPassantTargetSquare;
    uint moveNumber;
    uint nbReversibleHalfMoves; // For 50 and 75 move rules for draw

    // The information below is usually not included in a "position", but we need it for our implementation
    bool enPassantKingCapturePossibleK, enPassantKingCapturePossibleQ; // In "KC chess", we're allowing "en passant king capture" when the opponent king castled illegally.
    std::vector<Board> pastBoards; // Necessary for threefold repetition rule
    bool drawOffered; // Records whether a draw offer is on the table
};

/*namespace std
{
    template<> struct hash<Position>
    {
        std::size_t operator()(Position const& position) const noexcept
        {
            return position.getHash();
        }
    };
}*/

#endif // POSITION_H
