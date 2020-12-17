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
        switch (type)
        {
        case PieceType::Empty :
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
        case PieceType::King :
        {
            kings[i] = true;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            break;
        }
        case PieceType::Queen :
        {
            kings[i] = false;
            queens[i] = true;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            break;
        }
        case PieceType::Rook :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = true;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            break;
        }
        case PieceType::Bishop :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = true;
            knights[i] = false;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            break;
        }
        case PieceType::Knight :
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = true;
            pawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            break;
        }
        case PieceType::Pawn : break;
        {
            kings[i] = false;
            queens[i] = false;
            rooks[i] = false;
            bishops[i] = false;
            knights[i] = false;
            pawns[i] = true;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            break;
        }
        default: throw("Error in updateBitboards");
        }
    }
}
