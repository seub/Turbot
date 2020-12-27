# include "game.h"

int main()
{

    try
    {
        Game game;

        while (!game.isFinished())
        {
            game.playRandomMove();
        }

        std::cout << game << std::endl;
        std::cout << std::endl;
        std::cout << game.getPosition().toLichessURL() << std::endl;

    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }


    return 0;
}
