#include "evaluator.h"
#include "boardhelper.h"
#include "square.h"
#include "piece.h"

Evaluator::Evaluator(/* args */)
{
}

Evaluator::~Evaluator()
{
    
}

float Evaluator::evaluatePosition(const Position &position) const
{
    return 0;
}


Basicevaluator::Basicevaluator(std::map<PieceType,float> pieceValue): pieceValue(pieceValue)
{

}
    
Basicevaluator::~Basicevaluator()
{

}
    
float Basicevaluator::evaluatePosition(const Position &position) const
{
    float score = 0;
    Piece piece;
    bool ispiece;
    Color color;
    float value;
    for(uint index=0; index<64; ++index)
    {
        ispiece = position.getPiece(piece,Square(index));
        if(ispiece)
        {
            value = pieceValue.at(piece.getType());
            color = piece.getColor();
            if(color == Color::WHITE)
            {
                score +=value;
            }
            else
            {
                score -=value;
            }
        }
    }
    return score;
}