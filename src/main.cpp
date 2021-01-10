# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{
    try
    {
        
        /*BasicEvaluator evaluator;
        BasicMovePicker naive(&evaluator, 4);
        BasicMovePickerHash naivehash(&evaluator, 4);

        ComputerPlayer computer1(&naive);
        ComputerPlayer computer2(&naivehash);
        //HumanPlayer human;

        Game game( (Player *) &computer1,  (Player *) &computer1);
        game.playGame();

        Game game2( (Player *) &computer2,  (Player *) &computer2);
        game2.playGame();*/



        BasicEvaluatorZ evaluatorZ;
        BasicMovePickerZ basic(&evaluatorZ, 5);
        BasicMovePickerHashZ basicHash(&evaluatorZ, 6);

        ComputerPlayerZ computerZ1(&basic);
        ComputerPlayerZ computerZ2(&basicHash);
        HumanPlayerZ human;

        //GameZ gameZ( (PlayerZ *) &computerZ1,  (PlayerZ *) &computerZ1);
        //gameZ.playGame();

        GameZ gameZ2( (PlayerZ *) &human,  (PlayerZ *) &computerZ2);
        gameZ2.playGame();



    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }

    return 0;
}
