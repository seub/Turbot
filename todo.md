TODO





BUGS
Hi there! What is your name? Brice
Okay Brice, let's play!

What is your first move? Enter it in PGN notation. 1. e4
Okay, Brice played 1. e4.

The computer "Turbot 2" played 2... e6.

What is your next move? 2. d4
Okay, Brice played 2. d4.

The computer "Turbot 2" played 3... Be7.

What is your next move? 3. Nc3
Okay, Brice played 3. Nc3.

The computer "Turbot 2" played 4... Nc6.

What is your next move? 4. d5
Okay, Brice played 4. d5.

The computer "Turbot 2" played 5... exd5.

What is your next move? 5. exd5
Okay, Brice played 5. exd5.

The computer "Turbot 2" played 6... Na5.

What is your next move? 6. Nf3
Okay, Brice played 6. Nf3.

The computer "Turbot 2" played 7... Bc5.

What is your next move? 7. h3
Okay, Brice played 7. h3.

The computer "Turbot 2" played 8... Be7.

What is your next move? 8. b4
Okay, Brice played 8. b4.

The computer "Turbot 2" played 9... Bxb4.

What is your next move? 9. axb4
Sorry, that's an illegal move. Try again: (Enter "exit" to exit)
What is your next move? 9. ab4
incorrect pawn (?) move
Sorry, that didn't work. Try again: (Enter "exit" to exit)
What is your next move? 9. axb4
Sorry, that's an illegal move. Try again: (Enter "exit" to exit)
What is your next move? 9. 




PROPOSITIONS DE CLASSES


BranchComparator:
    Tells the MinMax move generator which branch is more interesting
    operator < (Position*, Position*) -> bool
    *Can have a bunch of additional parameters:
        - Evaluation
        - depth of position in the three

    
IDÉES :

KC chess:
Quand on voudra optimiser, quand on calcule l'arbre des coups, je pense que ça vaut le coup de passer en "kc chess" (king capture allowed)

Special actions:
claim draw, resign, offer draw ne drevraient pas faire partie de Move, c'est une action séparée. Elle fera partie d'une classe "Game"!

Try to fork stockfish? At least try to understand some of it

