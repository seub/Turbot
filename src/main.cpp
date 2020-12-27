# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{

    try
    {
        Game game;
        BasicEvaluator evaluator;
        MovePicker picker(&evaluator);

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

        game.playWithHuman(&picker);
    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
