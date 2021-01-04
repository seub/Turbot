# include "game.h"
# include "evaluator.h"
# include "movepicker.h"

int main()
{

    try
    {
        
        BasicEvaluator evaluator;
        NaiveMovePicker naive(&evaluator, 3);
        ForcefulMovePicker forceful(&evaluator, 3, 0, 1);

        ComputerPlayer computer1(&naive);
        ComputerPlayer computer2(&forceful);
        HumanPlayer human;
        Game game( (Player *) &human,  (Player *) &computer1);

        game.playGame();
    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }

    return 0;
}
