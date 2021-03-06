PROG Project 2
Bruno Mendes - up201906166 - participation estimative of 50%
José Costa - up201907216 - participation estimative of 50%
FEUP, 2020
------------------------------------------
SCRABBLE JUNIOR
-> All mandatory goals were accomplished, including:
   -> Import board from file
   -> Ask number of players from 2 to 4
   -> Robust in-game input/output: invalid moves of all types are prevented
      -> We considered continuing the same word is possible for all turns - the rules are ambiguous in that regard
      -> Every aglomeration of 2 letters or more vertically or horizontally is a word - 1 char words are not considered words
      -> Naturally, you must continue or start words with only the letters with you have on hand
   -> Distinguish played tiles with colors (different for each player)
   -> Exchange and pass behaviours:
      -> You can only pass when you cannot move and have tried to exchanged once
      -> We prohibited more than one exchange in the same turn
         -> You can only exchange when you cannot move
         -> If you exchanged once and still cannot move, you must pass
         -> You have two turns, so you can exchange a second time in the second turn if the exchanged letter in the first turn gave you a playable tile
      -> Automatic and manual passes:
         -> Most situations are manual so that the player learns the game mechanics
         -> If the same situation of the last turn happens again, you won't have to do it manually again
         -> If the pool is empty, you must try to exchange (which we'll be denied) and then we'll pass for you
         -> You cannot exchange when you have nothing on your hand, so we'll pass automatically for you
   -> The game ends when all tiles are highlighted or all players pass their moves (extreme situation)

-> Some nice and fun features were added to the game experience:
   -> "Card view" on the side of the board - shows adaptative game info
   -> Flexible command input
      -> The user may check in game help to learn all commands
      -> Player does not have to worry about extra spaces, upper and lowercase letters, misclicks on special chars - bloat is not interpreted
      -> We try to help you with some contextual command advice if you input a invalid command
   -> Spy pool - user can take a privileged look at the pool
      -> Note that he still can't control which random letter it's taken for him after each move as supposed
   -> Bot mode - all players can me made bots simply by including 'Computer' or 'Bot' in their names
      -> Bots have the ability to exchange from the pool when they cannot move
      -> Bots have no access to commands
      -> You can go from a 4 player bot game (spectator mode) to a 4 player human game as you desire
   -> Get hint - a human player may request a tip on what to do in current position
   -> "Territory dominance" - when completed, a word changes all its tiles colors to the player who completed it

-> Regarding the structure of the code:
   -> Six classes - Game, Move, Command, Player, Pool, Board - which interact as it would make sense in the real world:
      -> A Player gets 7 letters from the Pool when it's constructed
      -> To be valid, a Move may conform with Player's hand and with the Board state
      -> After each Move executes, a letter is taken from the pool if not empty
      -> A Command may be a Move, a exchange action, or any other available
      -> Game manipulates all other classes
   -> The Board class was shared between the two solutions to allow for greater consistency
      -> A common folder was used to avoid copy and pasting code: not mentioned in the project specs, but confirmed with teacher it's fine
      -> We included a .txt file that's used for the game introduction board: without it, a empty board is created with a error message
   -> ANSI escape codes were extensively used to allow for a enjoyable interface
      -> This is not supported on Windows versions prior to 2016 (current Windows 10 should be fine)
      -> Unix handles these instructions better than Windows (less screen flickering when clearing etc)
   -> The code compiles with no warnings in both CLang -Wextra (Clion) and MSVC -W4 (Visual Studio 2019)

----------------------------------------------
BOARD BUILDER
-> All mandatory goals were accomplished, including:
   -> Import board from file
   -> Create a new board with desired dimensions (max 20x20)
   -> Add words horizontally or vertically if in the dictionary
      -> We considered you cannot append to already existing words (eg. you cannot add MAN to POLICE)
      -> You cannot override already existing words (eg. you cannot add HURTING on top of HURT)
      -> You cannot create extra words as a side effect (thus the word must be isolated on non-intersected tiles)
      -> Naturally, you cannot crop words, so all characters of a word must fit inside the board
   -> Export created or edited board to later load on the Scrabble Game

-> Some extra features were added:
   -> Remove a word from the board
      -> A great attention was given to leave letters which belong to other words unchanged
      -> You cannot create other words as a side effect of the removal
   -> Import your own dictionary
      -> In the beggining, we ask you to import your own set of words to unveil your full creativity
   -> Delete current board
      -> You may import, edit and export multiple boards in a single programme execution
   -> Advice to help the user understand the programme functioning

-> Regarding the structure of the code:
   -> Two classes: CommandInterpreter, Board
      -> A CommandInterpreter performs all actions to manipulate a Board
   -> The code also compiles with no warnings in both CLang -Wextra (Clion) and MSVC -W4 (Visual Studio 2019)