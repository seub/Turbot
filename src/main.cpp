#include "position.h"
#include "legalmover.h"
#include "evaluator.h"
#include "piece.h"
#include "moveGenerator.h"

#include <functional>
#include <map>

# include "game.h"

std::map<PieceType,float> pieceValue = 
{
    {PieceType::BISHOP, 3},
    {PieceType::BISHOP, 3},
    {PieceType::EMPTY,0},
    {PieceType::KING, 1000},
    {PieceType::KNIGHT, 3},
    {PieceType::PAWN,1},
    {PieceType::QUEEN, 10},
    {PieceType::ROOK,5}
};




int main()
{

    try
    {

        Game game;

        while (!game.isFinished())
        {
            game.playRandomMove();
        }

        std::cout << game << std::endl;
        std::cout << std::endl;
        std::cout << game.getPosition().toLichessURL() << std::endl;

    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
