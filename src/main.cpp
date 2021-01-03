# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{

    try
    {
        
        BasicEvaluator evaluator;
        MinMaxMovePicker minMaxPicker3(&evaluator, 3);
        NaiveMovePicker naivePicker3(&evaluator, 3), naivePicker4(&evaluator, 4), naivePicker5(&evaluator, 5);

        //ComputerPlayer firstPlayer(Color::WHITE, &minMaxPicker3, "Turbot MinMax 3");
        ComputerPlayer firstPlayer(Color::WHITE, &naivePicker4, "Turbot Naive 4");
        //HumanPlayer firstPlayer(Color::WHITE);
        ComputerPlayer secondPlayer(Color::BLACK, &naivePicker5,"Turbot Naive 5");
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
