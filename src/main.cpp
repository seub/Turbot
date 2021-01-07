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

        //std::cout << sizeof (uint_fast8_t) << std::endl;

        /*std::cout << sizeof (char) << std::endl;
        std::cout << sizeof (unsigned short int) << std::endl;
        std::cout << sizeof (int) << std::endl;
        std::cout << sizeof (long int) << std::endl;
        std::cout << sizeof (long long int) << std::endl;
        std::cout << sizeof (unsigned long long int) << std::endl;
        std::cout << sizeof (uint) << std::endl;*/

        //https://www.chessprogramming.org/Transposition_Table
        //https://www.chessprogramming.org/General_Setwise_Operations#UpdateByMove?
        //https://www.chessprogramming.org/Quiescence_Search

        std::cout << "White King   = " << 00111 << " = " << 0b00111 << std::endl;
        std::cout << "White Queen  = " << 01011 << " = " << 0b01011 << std::endl;
        std::cout << "White Rook   = " << 01111 << " = " << 0b01111 << std::endl;
        std::cout << "White Bishop = " << 10011 << " = " << 0b10011 << std::endl;
        std::cout << "White Knight = " << 10111 << " = " << 0b10111 << std::endl;
        std::cout << "White Pawn   = " << 11011 << " = " << 0b11011 << std::endl;
        std::cout << "White King   = " << 00101 << " = " << 0b00101 << std::endl;
        std::cout << "White Queen  = " << 01001 << " = " << 0b01001 << std::endl;
        std::cout << "White Rook   = " << 01101 << " = " << 0b01101 << std::endl;
        std::cout << "White Bishop = " << 10001 << " = " << 0b10001 << std::endl;
        std::cout << "White Knight = " << 10101 << " = " << 0b10101 << std::endl;
        std::cout << "White Pawn   = " << 11001 << " = " << 0b11001 << std::endl;


        // White King   = 00111 =
        // White Queen  = 01011 =
        // White Rook   = 01111 =
        // White Bishop = 10011 =
        // White Knight = 10111 =
        // White Pawn   = 11011 =
        // Black King   = 00101 =
        // Black Queen  = 01001 =
        // Black Rook   = 01101 =
        // Black Bishop = 10001 =
        // Black Knight = 10101 =
        // White Pawn   = 11001 =
    }
    catch (char const *errorMessage)
    {
        std::cout << "CAUGHT EXCEPTION: " << std::endl;
        std::cout << errorMessage << std::endl;
    }

    return 0;
}
