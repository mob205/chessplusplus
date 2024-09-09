# Chess	
This is a complete recreation of the classic game of Chess made in C++. This chess model faithfully recreates every feature of chess, including:

- Castling
- Checking and illegal King moves
- Checkmate and stalemate
- Pinning
- Pawn promotion
- En Passant

This version also includes a move history and game state saving/loading.

Note: Since this game is in the terminal, White and Black pieces are denoted by capital and lowercase letters, respectively.
Q - Queen | N - Knight | B - Bishop | R - Rook | P - Pawn | K - King

# How to Play
Each move is played by first inputting the tile of the piece you want to move. Then, input the tile that you want that piece to go to.

![moving a pawn](https://github.com/mob205/chessplusplus/tree/main/images/firstmove.png?raw=true)

You can also enter various other commands while in a game.
- UNDO - undo the last turn
- SAVE - save the current game to file
- QUIT - exit to main menu.

![undoing a move](https://github.com/mob205/chessplusplus/tree/main/images/undo.png?raw=true)

A game can be loaded from the main menu. Simply type the name of the save file when prompted. By default, save files are located in chessplusplus/Saves.

![loading a saved game](https://github.com/mob205/chessplusplus/tree/main/images/loading.png?raw=true)

There are various saved games already prepared to show off some chess moves that require setup. 

Scholar's Mate:
![scholar's mate](https://github.com/mob205/chessplusplus/tree/main/images/scholarsmate.png?raw=true)

Castling:
![castling](https://github.com/mob205/chessplusplus/tree/main/images/castling.png?raw=true)

En Passant:
![en passant](https://github.com/mob205/chessplusplus/tree/main/images/enpassant.png?raw=true)

Pawn Promotion:
![promotion](https://github.com/mob205/chessplusplus/tree/main/images/promotion.png?raw=true)