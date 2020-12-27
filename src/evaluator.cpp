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

float Evaluator::evaluatePosition(const Position &position)
{
    return 0;
}


Basicevaluator::Basicevaluator(std::map<PieceType,float> pieceValue): pieceValue(pieceValue)
{

}
    
Basicevaluator::~Basicevaluator()
{

}
    
float Basicevaluator::evaluatePosition(const Position &position)
{
    auto position_hash = position.getHash();
    auto value_pointer = evaluated_res.find(position_hash);
    if( value_pointer != evaluated_res.end()) return value_pointer->second;
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
    evaluated_res[position_hash] = score;
    return score;
}