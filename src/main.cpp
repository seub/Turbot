# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{

    try
    {
        
        BasicEvaluator evaluator;
        MinMaxMovePicker minMaxPicker(&evaluator, 3);
        NaiveMovePicker naivePicker(&evaluator, 5);
        //ComputerPlayer firstPlayer(Color::WHITE, &minMaxPicker, "Turbot MinMax");
        HumanPlayer firstPlayer(Color::WHITE);
        ComputerPlayer secondPlayer(Color::BLACK, &naivePicker,"Turbot Naive");
        //HumanPlayer secondPlayer(Color::BLACK);
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
