#include "boardhelper.h"
#include "board.h"

BoardHelper::BoardHelper(const Board * const board) : board(board)
{
    updateBitboards();
}

void BoardHelper::updateBitboards()
{
    PieceType type;
    Color color;
    for (uint i=0; i!=64; ++i)
    {
        type = board->pieces[i].getType();
        color = board->pieces[i].getColor();
        fullboard += (color==Color::BLACK) ? 1 << (i*8+7) : 0 ;
        switch (type)
        {
        case PieceType::EMPTY :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = false;
            blackPieces[i] = false;
            occupiedSquares[i] = false;
            break;
        }
        case PieceType::KING :
        {
            kings[i] = true;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::WHITE) ? true : false);
            blackPieces[i] = ((color==Color::BLACK) ? true : false);
            occupiedSquares[i] = true;
            fullboard += 1 << (i*8 + 5);
            break;
        }
        case PieceType::QUEEN :
        {
            kings[i] = false;
            queens[i] = true;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::WHITE) ? true : false);
            blackPieces[i] = ((color==Color::BLACK) ? true : false);
            occupiedSquares[i] = true;
            fullboard += 1 << (i*8 + 4);
            break;
        }
        case PieceType::ROOK :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = true;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::WHITE) ? true : false);
            blackPieces[i] = ((color==Color::BLACK) ? true : false);
            occupiedSquares[i] = true;
            fullboard += 1 << (i*8 + 3);
            break;
        }
        case PieceType::BISHOP :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = true;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::WHITE) ? true : false);
            blackPieces[i] = ((color==Color::BLACK) ? true : false);
            occupiedSquares[i] = true;
            fullboard += 1 << (i*8 + 2);
            break;
        }
        case PieceType::KNIGHT :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = true;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::WHITE) ? true : false);
            blackPieces[i] = ((color==Color::BLACK) ? true : false);
            occupiedSquares[i] = true;
            fullboard += 1 << (i*8 + 1);
            break;
        }
        case PieceType::PAWN :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = true;
            whitePieces[i] = ((color==Color::WHITE) ? true : false);
            blackPieces[i] = ((color==Color::BLACK) ? true : false);
            occupiedSquares[i] = true;
            fullboard += 1 << (i*8 + 0);
            break;
        }
        default: throw("Error in updateBitboards");
        }
    }
}
