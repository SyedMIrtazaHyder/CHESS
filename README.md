# Data-Structure-Project: CHESS ENGINE
## This branch contains two programs
The files ending with cpp is the chess made using C++ and has an implemented bot. The other Folders and sln file is for GUI 2 player chess in C#.
## Story</br>
This project was made for our course EC-200 Data Structure. When we first started this project out we had thought to make it in our own terms and make the project using mainly OOP and DS concepts. Hence this implementation does not use bitboards or any other chess programming concepts generally used by people who persue to a working chess Engine. Generally theo outline for how to start working on any chess engine are listed here: https://www.chessprogramming.org/Main_Page </br>
The Project involved many parts, and me and my teammate learned a lot along the way. This was the first project I had used Git Bash for, as well as the first project where I broke down the whole code into smaller segments using header files.</br>
This project means a lot to me and the time and effort poured into this was absurd (a month and a half almost). However, I am proud of this engine. It may not be good at evaluating moves and may not be properly programmed using bitboard, but this is one of the most challenging projects I have undertaken as me writing this README.</br>

## Resources</br>
Must Read: https://alexanderameye.github.io/notes/chess-engine/</br>
Bot Ideas: https://www.freecodecamp.org/news/simple-chess-ai-step-by-step-1d55a9266977/</br>
How to Properly make a chess engine: https://www.chessprogramming.org/Main_Page</br>
Generating Psuedo Legal and Legal Moves: https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/#:~:text=Pseudo-legal%20move%20generation,enemy%20pieces%20on%20(captures). </br>

## Feautures</br>
The main features that are implemented include:</br>
<ul>
  <li>ASCII Gameboard</li>
  <li>Player vs Player</li>
  <li>Hardcoded decision Bot</li>
</ul>

### Interface </br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/578d2ef0-8681-41fc-87d6-35f603108c59)</br>
</br>
<ol>
  <li>Bot vs Player: The main aim of our project, make the game such that players can play against the chess bot.</li>
  <li>Player vs Player: Matchmaking between 2 players. Player 1 always plays as white and Player 2 as black.</li>
  <li>Bot vs Bot: A mode to measure the Bots performance, preferance of moves to be played and general bug searching</li>
</ol>

### Gameboard</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/eff8badb-1471-4e6b-aa82-c1955a2f71ba)</br>
</br>
We developed a simple Ascii chessboard. The lower case first letter tells the color of the piece ("w" = white and "b" = black) and the latter capital case letter denotes the Piece's type ("P" = Pawn, "R" = Rook, "B" = Bishop, "Q" = Queen, "N" = Knight, "K" = King).The board is implemented as a 2D vector containing pointers to the Pieces in the backend (A more optimal approach would be to declare a single 64 element long vector as it would simplify programming piece movement).

### Player</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/ebb93289-c5ab-439f-9d82-f29a944c7d91)</br>
</br>
The player is has 3 options as shown above.</br>

#### Make a move:
The player is shown the prompt where they enter valid current position and future position of one of their existing pieces on the board.</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/d70ae5d9-e5e3-47a3-98dd-69d24169b03c)</br>
</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/b0ea33e0-bb92-46c9-a671-8c594479943c)</br>
</br>
#### Undo a move
Undo's both the players move back to an earlier game state. However, this option is not allowed after castling and promotions. Each piece has its own list where it stores its previous x and y co-ordinates whenever it is moved. The undo was intially implemented as a stack, but was changed to list to cater for the 3 fold <a href = "https://www.chess.com/terms/threefold-repetition-chess">repition rule</a>.</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/9458b097-7416-40b2-b12e-bc4fc9ffe066)</br>
</br>

#### Resignation
The current player forfeits, giving the opponent the win. Takes the player back to the main menu screen.
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/29cdd0ac-ced3-4aef-bca1-c3d4e369cdb0)</br>
</br>

### Bot</br>
A lot of time and effort was put into making the game cater for the Bot. I had to refractor the whole code to generate legal moves so it was easier for the Bot to pick a move. After that I had designed a custom class called moves which took the move and some evaluation metrics, which was sent to a priority queue to always get the Best Computed move based on our evaluation functions. This operation was applied to all the pieces on a single side and then the played moved was choosen randomly between the from the top 3 moves among all the existing pieces (if the evaluation differnce between the best anf the other moves  15 or less).

## End of Game</br>
#### Checkmate</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/613dfcc5-c11e-4dd6-adfa-1aae8bebc6ed)</br>
#### Stalemate</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/b56cc5b5-21d2-4f70-a4ac-a2dd6990093c)</br>

#### Draw by Repition</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/d775ad13-0020-4d0e-ad7c-a006652a2fb6)</br>

#### Resign</br>
![image](https://github.com/SyedMIrtazaHyder/Data-Structure-Project/assets/111231209/54cf75da-82c4-491e-8a95-ce1483ef9f51)</br>

