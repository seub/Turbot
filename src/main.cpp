# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{

    try
    {
        
        BasicEvaluator evaluator;
        MinMaxMovePicker picker(&evaluator,2);
        ComputerPlayer whitePlayer(&picker, "Turbot 1");
        ComputerPlayer blackPlayer(&picker,"Turbot 2");
        Game game( (Player *) &whitePlayer,  (Player *) &blackPlayer);
        /*while (!game.isFinished())
        {
            game.playBestMove(&picker);
        }

        std::cout << game << std::endl;
        std::cout << std::endl;
        std::cout << game.getPosition().toLichessURL() << std::endl;

        std::string test;
        std::cin >> test;
        std::cout << test << std::endl;*/

        game.playGame();
    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
