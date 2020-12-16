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
            whiteKings[i] = false;
            whiteQueens[i] = false;
            whiteRooks[i] = false;
            whiteBishops[i] = false;
            whiteKnights[i] = false;
            whitePawns[i] = false;
            blackKings[i] = false;
            blackQueens[i] = false;
            blackRooks[i] = false;
            blackBishops[i] = false;
            blackKnights[i] = false;
            blackPawns[i] = false;
            whitePieces[i] = false;
            blackPieces[i] = false;
            occupiedSquares[i] = false;
            emptySquares[i] = true;
            break;
        }
        case PieceType::King :
        {
            whiteKings[i] = ((color==Color::White) ? true : false);
            whiteQueens[i] = false;
            whiteRooks[i] = false;
            whiteBishops[i] = false;
            whiteKnights[i] = false;
            whitePawns[i] = false;
            blackKings[i] = ((color==Color::Black) ? true : false);
            blackQueens[i] = false;
            blackRooks[i] = false;
            blackBishops[i] = false;
            blackKnights[i] = false;
            blackPawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            emptySquares[i] = false;
            break;
        }
        case PieceType::Queen :
        {
            whiteKings[i] = false;
            whiteQueens[i] = ((color==Color::White) ? true : false);
            whiteRooks[i] = false;
            whiteBishops[i] = false;
            whiteKnights[i] = false;
            whitePawns[i] = false;
            blackKings[i] = false;
            blackQueens[i] = ((color==Color::Black) ? true : false);
            blackRooks[i] = false;
            blackBishops[i] = false;
            blackKnights[i] = false;
            blackPawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            emptySquares[i] = false;
            break;
        }
        case PieceType::Rook :
        {
            whiteKings[i] = false;
            whiteQueens[i] = false;
            whiteRooks[i] = ((color==Color::White) ? true : false);
            whiteBishops[i] = false;
            whiteKnights[i] = false;
            whitePawns[i] = false;
            blackKings[i] = false;
            blackQueens[i] = false;
            blackRooks[i] = ((color==Color::Black) ? true : false);
            blackBishops[i] = false;
            blackKnights[i] = false;
            blackPawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            emptySquares[i] = false;
            break;
        }
        case PieceType::Bishop :
        {
            whiteKings[i] = false;
            whiteQueens[i] = false;
            whiteRooks[i] = false;
            whiteBishops[i] = ((color==Color::White) ? true : false);
            whiteKnights[i] = false;
            whitePawns[i] = false;
            blackKings[i] = false;
            blackQueens[i] = false;
            blackRooks[i] = false;
            blackBishops[i] = ((color==Color::Black) ? true : false);
            blackKnights[i] = false;
            blackPawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            emptySquares[i] = false;
            break;
        }
        case PieceType::Knight :
        {
            whiteKings[i] = false;
            whiteQueens[i] = false;
            whiteRooks[i] = false;
            whiteBishops[i] = false;
            whiteKnights[i] = ((color==Color::White) ? true : false);
            whitePawns[i] = false;
            blackKings[i] = false;
            blackQueens[i] = false;
            blackRooks[i] = false;
            blackBishops[i] = false;
            blackKnights[i] = ((color==Color::Black) ? true : false);
            blackPawns[i] = false;
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            emptySquares[i] = false;
            break;
        }
        case PieceType::Pawn : break;
        {
            whiteKings[i] = false;
            whiteQueens[i] = false;
            whiteRooks[i] = false;
            whiteBishops[i] = false;
            whiteKnights[i] = false;
            whitePawns[i] = ((color==Color::White) ? true : false);
            blackKings[i] = false;
            blackQueens[i] = false;
            blackRooks[i] = false;
            blackBishops[i] = false;
            blackKnights[i] = false;
            blackPawns[i] = ((color==Color::Black) ? true : false);
            whitePieces[i] = ((color==Color::White) ? true : false);
            blackPieces[i] = ((color==Color::Black) ? true : false);
            occupiedSquares[i] = true;
            emptySquares[i] = false;
            break;
        }
        default: throw("Error in updateBitboards");
        }
    }
}
