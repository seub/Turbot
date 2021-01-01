TODO

MISTAKES?
For the queen in boardhelper.cpp: fullboard += 1 << 5;

QUESTIONS
Why is there an unordered map and shit in class Evaluator?


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

