#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"
#include "game.h"
#include "board.h"

class Move
{
    friend class Mover;
    friend std::ostream & operator<<(std::ostream &out, const Move &M);

public:
    Move(const Square &origin, const Square &target, bool promotion = false, PieceType promotedPiece= PieceType::Empty);
    bool operator==(const Move &other) const;

    static bool fromLAN(Move & res, std::string &LANstring); // LAN = Long Algebraic Notation // NB: static const function not possible in C++ :/
    std::string toLAN() const;

    Square getOrigin() const;
    Square getTarget() const;

protected:
    Move(uint originIndex, uint targetIndex, bool promotion = false, PieceType promotedPiece= PieceType::Empty);
    Square origin, target;
    bool promotion;
    PieceType promotedPiece;
};

class MovePGN : Move
{
    friend std::ostream & operator<<(std::ostream &out, const MovePGN &M);

    MovePGN(const Move &M);

    static bool fromPGN(MovePGN &res, const std::string &PGNstring, const Board &board); // Board is necessary to determine origin square
    static bool fromPGN(MovePGN &res, const std::string &PGNstring, const Board &board, Color side);

    std::string toPGN(bool includeMoveNumber = false, bool includeActualMove=false,
                      bool includedDrawOfferBefore = false, bool includeDrawOfferAfter = false, bool includeGameResult=false) const;



private:
    static bool extractMoveNumberAndSide(uint &move, Color &side, std::string &outputTruncatedString, const std::string &PGNstring);
    static bool fromPGNwithoutMoveNum(MovePGN &res, const std::string &PGNstring, const Board &board, Color side);

    PieceType piecetype;
    Color side;
    bool pieceOriginAmbiguous, ambiguityLiftedByFile, ambiguityLiftedByRank;
    bool capture;
    bool castleShort, castleLong;
    bool check, checkmate;
    bool drawOfferBeforeIncluded, drawOfferAfterIncluded;
    bool moveNumberIncluded;
    uint moveNumber;
    bool actualMoveIncluded;
    bool gameResultIncluded;
    GameResult result;
};



#endif // MOVE_H
