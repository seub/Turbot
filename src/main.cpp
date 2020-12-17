# include "position.h"
# include "mover.h"

int main()
{
    Position P1(true);

    Mover LM(&P1, false);
    std::cout << LM.printKCMoves() << std::endl;

    return 0;
}
