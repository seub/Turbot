# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{

    try
    {
        
        BasicEvaluator evaluator;
        MinMaxMovePicker picker(&evaluator,2);
        //ComputerPlayer whitePlayer(&picker, "Turbot 1");
        HumanPlayer firstPlayer(Color::WHITE);
        //ComputerPlayer blackPlayer(&picker,"Turbot 2");
        HumanPlayer secondPlayer(Color::BLACK);
        Game game( (Player *) &firstPlayer,  (Player *) &secondPlayer);

        game.playGame();
    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
