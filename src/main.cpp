# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{
    try
    {
        
        BasicEvaluator evaluator;
        BasicMovePicker naive(&evaluator, 4);
        BasicMovePickerHash naivehash(&evaluator, 4);

        ComputerPlayer computer1(&naive);
        ComputerPlayer computer2(&naivehash);
        //HumanPlayer human;

        Game game( (Player *) &computer1,  (Player *) &computer1);
        game.playGame();

        Game game2( (Player *) &computer2,  (Player *) &computer2);
        game2.playGame();
    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }

    return 0;
}
