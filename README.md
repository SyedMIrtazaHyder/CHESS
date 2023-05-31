# Data-Structure-Project: CHESS ENGINE
##Story</br>
This project was made for our course EC-200 Data Structure. When we first started this project out we had thought to make it in our own terms and make the project using mainly OOP and DS concepts. Hence this implementation does not use bitboards or any other chess programming concepts generally used by people who persue to a working chess Engine. Generally theo outline for how to start working on any chess engine are listed here: https://www.chessprogramming.org/Main_Page </br>
The Project involved many parts, and me and my teammate learned a lot along the way. This was the first project I had used Git Bash for, as well as the first project where I broke down the whole code into smaller segments using header files.</br>
This project means a lot to me and the time and effort poured into this was absurd (a month and a half almost). However, I am proud of this engine. It may not be good at evaluating moves and may not be properly programmed using bitboard, but this is one of the most challenging projects I have undertaken as me writing this README.</br>
##Resources</br>
Must Read: https://alexanderameye.github.io/notes/chess-engine/</br>
Bot Ideas: https://www.freecodecamp.org/news/simple-chess-ai-step-by-step-1d55a9266977/</br>
How to Properly make a chess engine: https://www.chessprogramming.org/Main_Page</br>
Generating Psuedo Legal and Legal Moves: https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/#:~:text=Pseudo-legal%20move%20generation,enemy%20pieces%20on%20(captures). </br>
##Feautures:
The main features that are implemented include:</br>
<ul>
  <li>ASCII Gameboard</li>
  <li>Player vs Player</li>
  <li>Hardcoded decision Bot</li>
</ul>
###Gameboard
###Player
###Bot
A lot of time and effort was put into making the game cater for the Bot. I had to refractor the whole code to generate legal moves so it was easier for the Bot to pick a move. After that I had designed a custom class called moves which took the move and some evaluation metrics, which was sent to a priority queue to always get the Best Computed move based on our evaluation functions. This operation was applied to all the pieces on a single side and then the played moved was choosen randomly between the general top 3 moves among all the existing pieces.


