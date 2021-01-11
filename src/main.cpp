# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{
    try
    {

        BasicEvaluator evaluator;
        BasicMovePicker basic(&evaluator, 4);
        BasicMovePickerHash basicHash(&evaluator, 4);

        ComputerPlayer computer1(&basic);
        ComputerPlayer computer2(&basicHash);
        //HumanPlayerZ human;

        Game gameZ( (Player *) &computer1,  (Player *) &computer1);
        gameZ.playGame();

        Game gameZ2( (Player *) &computer2,  (Player *) &computer2);
        gameZ2.playGame();



    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }

    return 0;
}
