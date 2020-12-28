TODO

BUGS:
[Event "NA"]
[Site "NA"]
[Date "2020.12.28"]
[Round "NA"]
[White "Turbot 1"]
[Black "Turbot 2"]
[Result "1/2-1/2"]

1. Nc3 c6 2. f4 Qc7 3. Kf2 g5 4. f5 Bh6 5. g4 Nf6 6. Nb5 Qf4+ 7. Kg2 Nh5 8. e3 Rg8 9. Nd4 Bg7 10. d3 Qf3+ 11. Ndxf3 Bf6 12. a4 c5 13. Ne2 a5 14. c3 Nf4+ 15. Kg3 Bd4 16. Ra3 Nc6 17. b3 Nh3 18. Qc2 Ra7 19. Ra1 Kf8 20. e4 Rh8 21. Be3 b6 22. Rg1 Bb7 23. Rh1 e6 24. Ra2 Ba6 25. Nc1 Rg8 26. b4 Bc8 27. Qg2 Nd8 28. f6 Ba6 29. Bd2 Bc8 30. Nh4 Ng1 31. Bf4 Rb7 32. Bd6+ Ke8 33. Rc2 Nh3 34. Ra2 Rb8 35. Nf3 h6 36. Bf4 Ba6 37. Ne1 e5 38. Nf3 d5 39. Rd2 Rc8 40. Ne1 Ne6 41. Re2 Kd8 42. Rg1 Rg6 43. Qh1 Rg8 44. Qg2 Nf2 45. Re3 Nd1 46. h3 Rf8 47. Qh2 Kc7 48. Na2 Kd6 49. Qd2 Nd8 50. Ng2 Rh8 51. Re1 Re8 52. Be2 Rc6 53. Qe3 Rc7 54. Rgf1 Nf2 55. Kh2 Kd7 56. Qd2 Bb7 57. h4 Nc6 58. Rc1 Be3 59. Ra1 Nd8 60. Rg1 Kc8 61. Bd1 Rf8 62. d4 Rd7 63. b5 Bc6 64. Nc1 Kb7 65. Bf3 Kb8 66. Bd1 Rh8 67. Ne2 Kb7 68. Re1 Ne6 69. Kg3 Rhd8 70. Kh2 Rg8 71. Qa2 Rd6 72. Rf1 Nc7 73. Re1 Na6 74. Bg3 Rc8 75. Qb3 Nh1 76. Qc4 Nc7 77. h5 Nf2 78. Bc2 Ra8 79. Red1 Bd2 80. Bh4 Kc8 81. Ng1 Rd7 82. Ne1 Be3 83. Nh3 Kb8 84. Rdb1 Bd2 85. Kg1 Bb7 86. Rb4 Bc1 87. Bb1 Bb2 88. Qd3 Nd1 89. Qg3 Ne6 90. Bc2 Nc7 91. Kg2 c4 92. Kh2 Nf2 93. Qg2 Ne8 94. Qf1 Bc6 95. Qg1 Ng7 96. Bd3 Nh1 97. Ng2 Ne6 98. Rb1 Rc7 99. Qd1 Rc8 100. Kg1 Ba1 101. Bf2 Ra6 102. Nh4 Rd8 103. Rc1 Rc8 104. Bc2 Nc5 105. Qe2 Ng3 106. Qd1 Bd7 107. Ng6 Ka8 108. Ne7 Bb2 109. Qe1 Ba3 110. Nc6 Kb7 111. Nd8+ Ka7 112. Be3 Ne6 113. Ra1 Rb8 114. Nc6+ Ka8 115. Qc1 Nf1 116. Bf2 Rb7 117. Qe3 Rb8 118. Qf3 Nf4 119. Rb3 Be6 120. Bh4 Kb7 121. Qd1 Bc1 122. Rb2 Ra7 123. Nb4 Rd8 124. Kf2 Rda8 125. Rba2 Nh2 126. Ng1 Bd7 127. Qf1 Kb8 128. Nf3 Kc8 129. Qg1 Kc7 1/2-1/2

Why doesn't Turbot play 3.. Qxf4?


Question :
    Sur la class position, il y a une fonction reset. Est ce qu'on ne veut pas plutot une class immutable?
Réponse :
    Je crois que y a pas vraiment de classe "immutable", mais on peut faire en sorte que 1. toutes les méthodes publiques de la classe sont const, et 2. on peut utiliser
    le mot-clé const quand on crée un élément de la classe.


PROPOSITIONS DE CLASSES

Positionevaluator:
    (classe virtuelle)

BasicEvaluator(Positionevaluator):
    piecevalues

MoveGenerator:
    Returns a move. Has the job of deciding how long it should think.
        -Maybe the time decision is decoupled method or not (this move is the best lets not lose time)
    Probably has some cached memory not to explore already explored.
        -Further down the line -> cache manager, pruning
    methods 
        -virtual proposemove(position,timeRules) -> move

BranchComparator:
    Tells the MinMax move generator which branch is more interesting
    operator < (Position*, Position*) -> bool
    *Can have a bunch of additional parameters:
        - Evaluation
        - depth of position in the three

MoveGeneratorbyMinMax(MoveGenerator):
    - takes an evaluator as parameter:
    - takes a branchComparator as as parameter
    

    
IDÉES :

KC chess:
Quand on voudra optimiser, quand on calcule l'arbre des coups, je pense que ça vaut le coup de passer en "kc chess" (king capture allowed)

Special actions:
claim draw, resign, offer draw ne drevraient pas faire partie de Move, c'est une action séparée. Elle fera partie d'une classe "Game"!

Try to fork stockfish? At least try to understand some of it


NOTES POUR CODE "PROPRE"
- double au lieu de float
- ne pas oublier les const
- private before public?
- mot clé override
- no need to write destructor in general
- passer les arguments par référence
