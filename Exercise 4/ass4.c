#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "ass4.h"

/***********************
 * Name: Yahav Zarfati
 * Group: 01 - CS
 * Assignment: ass04
 ***********************/

const int TRUE = 1;
const int FALSE = 0;
const int INVALID = -1;

// Move logical representation
typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, promotionPiece, tmpDest, WKR, WKC, BKR, BKC;
	int iscapture, ispromotion, isCheck, isMate;
} Move;

char boardCopy[SIZE][SIZE];
int isWayFree(Move* move, char board[][SIZE], int isWhiteTurn);
int validMove(Move* move, int isWhiteTurn);
int isDestFree(Move* move, char board[][SIZE]);

/******************
* Function Name:createBoard
* Input:char board[][SIZE] 2d array the board, char fen[] string
* Output:No output void
* Function Operation:The function use the given input,implement the letters of the string
in the right order in the board.
******************/
void createBoard(char board[][SIZE], char fen[]) {
	char SizeToChar = SIZE + '0';
	int numberOfSpaces = 0;
	int isNewLine = 0;
	int k = 0;
	//Creates the board, each row and each column
	for (int i = 0; i < SIZE; i++) {//Rows
		for (int j = 0; j < SIZE; j++) {//Columns
			board[i][j] = ' ';
			//Run on the given fen string
			while (fen[k] != '\0') {//fen[]
				//Match correct input to board from the fen - blank rows/spots/correct tools
				if (fen[k] == '/')
					k++;
				else if ((fen[k] > '0' && fen[k] <= SizeToChar)) {
					numberOfSpaces = fen[k] - '0';
					//Move forward k in fen and in board create blank places
					for (int r = 0; r < numberOfSpaces; r++) {
						board[i][j] = ' ';
						j++;
					}
					j--;
					k++;
					break;
				}
				else if ((fen[k] == 'k') || (fen[k] == 'K') || (fen[k] == 'q') || (fen[k] == 'Q') ||
					(fen[k] == 'r') || (fen[k] == 'R') || (fen[k] == 'b') || (fen[k] == 'B') || (fen[k] == 'n') ||
					(fen[k] == 'N') || (fen[k] == 'p') || (fen[k] == 'P')) {
					board[i][j] = fen[k];
					k++;
					break;
				}
			}
		}
	}
}

/******************
* Function Name:printBoard
* Input:char board[][SIZE] 2d array the board.
* Output:No output - void
* Function Operation:The function use the given input, prints it according a chess board format.
******************/
void printBoard(char board[][SIZE]) {
	char columnIndex = 'A';
	printf("* |");
	//Print first row
	for (int j = 0; j < SIZE; j++) {
		//Check if its last letter that should be printed
		if (j != SIZE - 1) {
			printf("%c ", columnIndex);
			columnIndex++;
		}
		else {
			printf("%c", columnIndex);
			columnIndex++;
		}
	}
	printf("| *\n");
	printf("* -");
	//Print second row
	for (int j = 0; j < SIZE; j++) {
		printf("--");
	}
	printf(" *\n");
	int counter = SIZE;
	//Print values with | and rows index
	for (int i = 0; i < SIZE; i++) {
		printf("%d |", counter);
		for (int j = 0; j < SIZE; j++) {
			printf("%c|", board[i][j]);
		}
		printf("  %d\n", counter);
		counter--;
	}
	printf("* -");
	//Print row before last
	for (int j = 0; j < SIZE; j++) {
		printf("--");
	}
	printf(" *\n");
	printf("* |");
	columnIndex = 'A';
	//Print last row
	for (int j = 0; j < SIZE; j++) {
		//Check if its last letter that should be printed
		if (j != SIZE - 1) {
			printf("%c ", columnIndex);
			columnIndex++;
		}
		else {
			printf("%c", columnIndex);
			columnIndex++;
		}

	}
	printf("| *\n");
}

/******************
* Function Name:charDigittoDigit
* Input:char c (valid values between '0'-'9')
* Output:int c(values between 0 - 9)
* Function Operation:The function use the given input,
converts the input to its integer value as its char show.
******************/
int charDigitttoDigit(char c) {
	assert('0' <= c && c <= '9');
	return c - '0';
}

/******************
 * Function Name:chartoDigit
 * Input:char c (valid values between 'a'-'z')
 * Output:int c(values between 97 - 122)
 * Function Operation:The function use the given input, converts the input to its integer value.
 ******************/
int chartoDigit(char c) {
	assert('a' <= c && c <= 'z');
	return c - ('a');
}

/******************
 * Function Name:upperCaseToLowerCase
 * Input:char c
 * Output:char c
 * Function Operation:The function use the given input, converts upper case to lower case,
 else, the input stay the same.
 ******************/
char upperCaseToLowerCase(char c) {
	//Check if input is upper case
	if ('A' <= c && c <= 'Z') {
		assert('A' <= c && c <= 'Z');
		return c + 32;
	}
	else
		return c;
}

/******************
 * Function Name:lowerCaseToUpperCase
 * Input:char c
 * Output:char c
 * Function Operation:The function use the given input, converts lower case to upper case,
 else, the input stay the same.
 ******************/
char lowerCaseToUpperCase(char c) {
	//Check if input is lower case
	if ('a' <= c && c <= 'z') {
		assert('a' <= c && c <= 'z');
		return c - 32;
	}
	else
		return c;
}

/******************
 * Function Name:copyBoard
 * Input:char board1 [][SIZE],char board2 [][SIZE] both 2d arrays
 * Output:No output - void
 * Function Operation:The function use the given input, copy the second given board
 into the first board
 ******************/
void copyBoard(char board1[][SIZE], char board2[][SIZE]) {
	//Runs over each place on both boards
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			board1[i][j] = board2[i][j];
		}
	}
}

/******************
 * Function Name:changePosition
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:No output - void
 * Function Operation:The function implement in board destination values both i and j,also,
 implement in source blank space.In case of promotion, implement the promotion and new values.
 ******************/
void changePosition(Move* move, char board[][SIZE], int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	//Check if there is promotion in current move
	if (move->ispromotion) {
		//Check if its black or white turn for right promotion piece
		if (isWhiteTurn == FALSE)
			move->promotionPiece = upperCaseToLowerCase(move->promotionPiece);
		board[iDest][jDest] = move->promotionPiece;
		move->srcPiece = move->promotionPiece;
		move->srcRow = iDest;
		move->srcCol = jDest;
	}
	else
		board[iDest][jDest] = board[iSrc][jSrc];
	board[iSrc][jSrc] = ' ';
}

/******************
 * Function Name:chartoDigitForSource
 * Input:char c (valid values between 'a'-'z')
 * Output:int c(values between 97 - 122)
 * Function Operation:The function use the given input, converts the input to its integer value.
 ******************/
int chartoDigitForSource(char c) {
	//Check if input is lower case
	if (c >= 'a' && c <= 'z')
		return c - ('a');
}

/******************
 * Function Name:charDigittoDigitForSource
 * Input:char c (valid values between '0'-'9')
 * Output:int c(values between 0 - 9)
 * Function Operation:The function use the given input,
 converts the input to its integer value as its char show.
 ******************/
int charDigitttoDigitForSource(char c) {
	//Check if input is char number
	if (c >= '0' && c <= '9')
		return c - '0';
}

/******************
 * Function Name:getToolSource
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:No output - void
 * Function Operation:The function runs over the board,
 finds out the correct tool for the move and extracts its source position.
 ******************/
void getToolSource(Move* move, char board[][SIZE], int isWhiteTurn) {
	int foundTool = FALSE;
	int validToolsCounter = 0;
	char srcPiece = 0;
	//Check if source is given from pgn
	if (move->srcRow != -1)
		move->srcRow = charDigitttoDigitForSource(move->srcRow);
	else if (move->srcCol != -1)
		move->srcCol = chartoDigitForSource(move->srcCol);
	//Check which loop to run according to source input from pgn
	if (move->srcRow == -1 && move->srcCol == -1) {
		//Runs over the whole board
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				//Make seperation between white/black tools
				if (isWhiteTurn == TRUE) {
					//Check if its the same tool as input
					if (board[i][j] == move->srcPiece) {
						move->srcRow = i;
						move->srcCol = j;
						//Check if its the correct tool for the move and counts it
						if (isWayFree(move, board, isWhiteTurn) == TRUE &&
							(validMove(move, isWhiteTurn) == TRUE))
							return;
					}
				}
				else {
					srcPiece = upperCaseToLowerCase(move->srcPiece);
					//Check if its the same tool as input
					if (board[i][j] == srcPiece) {
						move->srcRow = i;
						move->srcCol = j;
						//Check if its the correct tool for the move and counts it
						if (isWayFree(move, board, isWhiteTurn) == TRUE &&
							(validMove(move, isWhiteTurn) == TRUE))
							return;
					}
				}
			}
		}
	}
	else if (move->srcRow == -1) {
		//Run loop if not given row in source input
		for (int i = 0; i < SIZE; i++) {
			//Make seperation between white/black tools
			if (isWhiteTurn == TRUE) {
				//Check if its the same tool as input
				if (board[i][move->srcCol] == move->srcPiece) {
					move->srcRow = i;
					//Check if its the correct tool for the move and counts it
					if (isWayFree(move, board, isWhiteTurn) == TRUE &&
						(validMove(move, isWhiteTurn) == TRUE))
						return;
				}
			}
			else {
				srcPiece = upperCaseToLowerCase(move->srcPiece);
				//Check if its the same tool as input
				if (board[i][move->srcCol] == srcPiece) {
					move->srcRow = i;
					//Check if its the correct tool for the move and counts it
					if (isWayFree(move, board, isWhiteTurn) == TRUE &&
						(validMove(move, isWhiteTurn) == TRUE))
						return;
				}
			}
		}
	}
	else if (move->srcCol == -1) {
		//Run loop if not given column in source input
		for (int j = 0; j < SIZE; j++) {
			//Make seperation between white/black tools
			if (isWhiteTurn == TRUE) {
				//Check if its the same tool as input
				if (board[move->srcRow][j] == move->srcPiece) {
					move->srcCol = j;
					//Check if its the correct tool for the move and counts it
					if (isWayFree(move, board, isWhiteTurn) == TRUE &&
						(validMove(move, isWhiteTurn) == TRUE))
						return;
				}
			}
			else {
				srcPiece = upperCaseToLowerCase(move->srcPiece);
				//Check if its the same tool as input
				if (board[move->srcRow][j] == srcPiece) {
					move->srcCol = j;
					//Check if its the correct tool for the move and counts it
					if (isWayFree(move, board, isWhiteTurn) == TRUE &&
						(validMove(move, isWhiteTurn) == TRUE))
						return;
				}
			}
		}
	}
	//If found a tool return
	if (validToolsCounter == 1)
		return;
}

/******************
 * Function Name:findBlackKingLocation
 * Input:struct pointer move,char board [][SIZE] 2d array
 * Output:No output - void
 * Function Operation:The function runs over the board,
 finds out the source location of the black king.
 ******************/
void findBlackKingLocation(Move* move, char board[][SIZE]) {
	//Run over the whole board
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			//Check if the tool is black king
			if (board[i][j] == 'k') {
				move->BKR = i;
				move->BKC = j;
			}
		}
	}
}

/******************
 * Function Name:findWhiteKingLocation
 * Input:struct pointer move,char board [][SIZE] 2d array
 * Output:No output - void
 * Function Operation:The function runs over the board,
 finds out the source location of the white king.
 ******************/
void findWhiteKingLocation(Move* move, char board[][SIZE]) {
	//Run over the whole board
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			//Check if the tool is white king
			if (board[i][j] == 'K') {
				move->WKR = i;
				move->WKC = j;
			}
		}
	}
}

/******************
 * Function Name:isBlackTeam
 * Input:char c (valid values between 'a'-'z')
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if input is from black team.
 ******************/
int isBlackTeam(char c) {
	//Check if input is lower case
	if ('a' <= c && c <= 'z')
		return TRUE;
	return FALSE;
}

/******************
 * Function Name:isWhiteTeam
 * Input:char c (valid values between 'A'-'Z')
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if input is from white team.
 ******************/
int isWhiteTeam(char c) {
	//Check if input is upper case
	if ('A' <= c && c <= 'Z')
		return TRUE;
	return FALSE;
}

/******************
 * Function Name:isTheSameTeam
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if input is from the same team (black with black/white with white).
 ******************/
int isTheSameTeam(char board[][SIZE], Move* move, int isWhiteTurn) {
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	//Check if its white or black turn
	if (isWhiteTurn == TRUE) {
		//Check if the tool in destination is from black team
		if (isBlackTeam(board[iDest][jDest]) == TRUE)
			return FALSE;
		return TRUE;
	}
	//Check if the tool in destination is from white team
	if (isWhiteTeam(board[iDest][jDest]) == TRUE)
		return FALSE;
	return TRUE;
}

/******************
 * Function Name:isHorizonLineClearForChess
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if horizonal line from tool source to black/white king is clear.
 ******************/
int isHorizonLineClearForChess(char board[][SIZE], Move* move, int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest, jDest;
	//Check if its white or black turn, therfore implement different destination
	if (isWhiteTurn == TRUE) {
		iDest = move->BKR;
		jDest = move->BKC;
	}
	else {
		iDest = move->WKR;
		jDest = move->WKC;
	}
	int iclimb = 0, jclimb = 0, lDest = 0, iFinal = 0, jFinal = 0;
	//Check if its horizonal line
	if (jDest != jSrc)
		return FALSE;
	lDest = abs(iDest - iSrc);
	//Check if its going upwards or downwards
	if (iDest > iSrc)
		iclimb = 1;
	else
		iclimb = -1;
	//Run over each step for tool way
	for (int i = 1; i < lDest; i++) {
		iFinal = iSrc + iclimb * i;
		//Check if the step is empty
		if (board[iFinal][jDest] != ' ')
			return FALSE;
	}
	return TRUE;
}

/******************
 * Function Name:isStrightLineClearForChess
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if straight line from tool source to black/white king is clear.
 ******************/
int isStrightLineClearForChess(char board[][SIZE], Move* move, int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest, jDest;
	//Check if its white or black turn, therfore implement different destination
	if (isWhiteTurn == TRUE) {
		iDest = move->BKR;
		jDest = move->BKC;
	}
	else {
		iDest = move->WKR;
		jDest = move->WKC;
	}
	int iclimb = 0, jclimb = 0, lDest = 0, iFinal = 0, jFinal = 0;
	//Check if its a stright line
	if (iDest != iSrc)
		return FALSE;
	lDest = abs(jDest - jSrc);
	//Check if its going left or right
	if (jDest > jSrc)
		jclimb = 1;
	else
		jclimb = -1;
	//Run over each step for tool way
	for (int i = 1; i < lDest; i++) {
		jFinal = jSrc + jclimb * i;
		//Check if the step is empty
		if (board[iDest][jFinal] != ' ')
			return FALSE;
	}
	return TRUE;
}

/******************
 * Function Name:isDiagonalLineClearForChess
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if diagonal line from tool source to black/white king is clear.
 ******************/
int isDiagonalClearForChess(char board[][SIZE], Move* move, int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest, jDest;
	//Check if its white or black turn, therfore implement different destination
	if (isWhiteTurn == TRUE) {
		iDest = move->BKR;
		jDest = move->BKC;
	}
	else {
		iDest = move->WKR;
		jDest = move->WKC;
	}
	int iclimb = 0, jclimb = 0, lDest = 0, iFinal = 0, jFinal = 0;
	//Check if its diagonal line
	if (abs(iSrc - iDest) != abs(jSrc - jDest))
		return FALSE;
	//Check if its going upwards or downwards in diagonal line
	if (iDest > iSrc)
		iclimb = 1;
	else
		iclimb = -1;
	//Check if its going left or right in diagonal line
	if (jDest > jSrc)
		jclimb = 1;
	else
		jclimb = -1;
	lDest = abs(iDest - iSrc);
	//Run over each step for tool way
	for (int i = 1; i < lDest; i++) {
		iFinal = iSrc + iclimb * i;
		jFinal = jSrc + jclimb * i;
		//Check if the step is empty
		if (board[iFinal][jFinal] != ' ')
			return FALSE;
	}
	return TRUE;
}

/******************
 * Function Name:isWayFreeForChess
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check according to the input tool if its way clear to the white/black king.
 ******************/
int isWayFreeForChess(Move* move, char board[][SIZE], int isWhiteTurn) {
	switch (lowerCaseToUpperCase(move->srcPiece)) {
	case 'K':
		return TRUE;
	case 'Q':
		//Check if it has a clear way to black/white king according to the tool right steps rules
		if (isStrightLineClearForChess(board, move, isWhiteTurn) == TRUE ||
			isHorizonLineClearForChess(board, move, isWhiteTurn) == TRUE ||
			isDiagonalClearForChess(board, move, isWhiteTurn) == TRUE)
			return TRUE;
	case 'R':
		//Check if it has a clear way to black/white king according to the tool right steps rules
		if (isStrightLineClearForChess(board, move, isWhiteTurn) == TRUE ||
			isHorizonLineClearForChess(board, move, isWhiteTurn) == TRUE)
			return TRUE;
		return FALSE;
	case 'B':
		//Check if it has a clear way to black/white king according to the tool right steps rules
		if (isDiagonalClearForChess(board, move, isWhiteTurn) == TRUE)
			return TRUE;
		return FALSE;
	case 'N':
		return TRUE;
	case 'P':
		//Check if it has a clear way to black/white king according to the tool right steps rules
		if ((isHorizonLineClearForChess(board, move, isWhiteTurn) == TRUE)
			|| (move->iscapture == TRUE))
			return FALSE;
	case 'p':
		//Check if it has a clear way to black/white king according to the tool right steps rules
		if ((isHorizonLineClearForChess(board, move, isWhiteTurn) == TRUE)
			|| (move->iscapture == TRUE))
			return TRUE;
		return FALSE;
	default:
		return FALSE;
	}
}

/******************
 * Function Name:validMoveForChess
 * Input:struct pointer move, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check according to the input tool,
 if the tool can make a legal step to the white/black king.
 ******************/
int validMoveForChess(Move* move, int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest, jDest;
	//Check if its white or black turn, therfore implement different destination
	if (isWhiteTurn == TRUE) {
		iDest = move->BKR;
		jDest = move->BKC;
	}
	else {
		iDest = move->WKR;
		jDest = move->WKC;
	}
	switch (move->srcPiece) {
	case 'K':
		//Check if its a legal move to black/white king according to the tool right steps rules
		if (((abs(iDest - iSrc == 0) && (abs(jDest - jSrc) == 1))) || (abs(iDest - iSrc == 1) && (abs(jDest - jSrc) == 0)) ||
			(abs(iDest - iSrc == 1) && (abs(jDest - jSrc) == 1)))
			return TRUE;
		return FALSE;
	case 'Q':
		//Check if its a legal move to black/white king according to the tool right steps rules
		if (((abs(iDest - iSrc) == 0) || (abs(jDest - jSrc) == 0)) || ((abs(iDest - iSrc)) == (abs(jDest - jSrc))))
			return TRUE;
		return FALSE;
	case 'R':
		//Check if its a legal move to black/white king according to the tool right steps rules
		if ((abs(iDest - iSrc) == 0) || (abs(jDest - jSrc) == 0))
			return TRUE;
		return FALSE;
	case 'B':
		//Check if its a legal move to black/white king according to the tool right steps rules
		if ((abs(iDest - iSrc)) == (abs(jDest - jSrc)))
			return TRUE;
		return FALSE;
	case 'N':
		//Check if its a legal move to black/white king according to the tool right steps rules
		if (((abs(iDest - iSrc) == 2) && (abs(jDest - jSrc) == 1)) || (((abs(iDest - iSrc) == 1) && (abs(jDest - jSrc) == 2))))
			return TRUE;
		return FALSE;
	default:
		//Check if there is capture in the current move
		if (move->iscapture == TRUE) {
			//Check if its black or white pawn
			if (isWhiteTurn == TRUE) {
				//Check if its legal white pawn capture step
				if ((iDest - iSrc == -1) && (abs(jDest - jSrc) == 1))
					return TRUE;
			}
			//Check if its legal black pawn capture step
			if ((iDest - iSrc == 1) && (abs(jDest - jSrc) == 1))
				return TRUE;
		}
		else {
			//Check if its legal pawn movement (not capture)
			if (jDest == jSrc) {
				//Check if its black or white pawn
				if (isWhiteTurn == TRUE) {
					//Check if its normal movement or 2 steps from a specific line
					if (iSrc == SIZE - 2 && iDest == SIZE - 4)
						return TRUE;
					else if (iDest - iSrc == -1)
						return TRUE;
				}
				//Check if its normal movement or 2 steps from a specific line
				if (iSrc == 1 && iDest == 3)
					return TRUE;
				else if (iDest - iSrc == 1)
					return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
}

/******************
 * Function Name:promotion
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if there is legal promotion in current move.
 ******************/
int promotion(Move* move, char board[][SIZE], int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	//Check if its the right tool for promotion
	if (move->srcPiece == 'P' || move->srcPiece == 'p') {
		//Check if destination is clear
		if (isDestFree(move, board) == TRUE) {
			//Check if its white or black turn
			if (isWhiteTurn == TRUE) {
				//Check if its legal move for promotion
				if (iSrc == 1 && iDest == 0)
					return TRUE;
			}
			//Check if its legal move for promotion
			if (iSrc == SIZE - 2 && iDest == SIZE - 1)
				return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

/******************
 * Function Name:capture
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if there is legal capture in current move.
 ******************/
int capture(Move* move, char board[][SIZE], int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	//Check if its white or black tool
	if (isWhiteTurn == TRUE) {
		//Check if its from the same team
		if (isTheSameTeam(board, move, isWhiteTurn) == TRUE)
			return FALSE;
		//Check if it has clear way for capture and its a legal move for tool
		if ((isWayFree(move, board, isWhiteTurn) == TRUE) && (validMove(move, isWhiteTurn))) {
			return TRUE;
		}
	}
	//Check if its from the same team
	if (isTheSameTeam(board, move, isWhiteTurn) == TRUE)
		return FALSE;
	//Check if it has clear way for capture and its a legal move for tool
	if ((isWayFree(move, board, isWhiteTurn) == TRUE) && (validMove(move, isWhiteTurn))) {
		return TRUE;
	}
	return FALSE;
}

/******************
 * Function Name:Check
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if there is legal check in current move(passive and active).
 ******************/
int Check(Move* move, char board[][SIZE], int isWhiteTurn) {
	int backupSrcRow = 0;
	int backupSrcCol = 0;
	//Check if its white or black turn
	if (isWhiteTurn == TRUE) {
		findBlackKingLocation(move, boardCopy);
		//Runs over the whole board
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				//Check if each tool has a clear way and its doing a valid move in order to threat the king
				if (isWayFreeForChess(move, boardCopy, isWhiteTurn) == TRUE &&
					validMoveForChess(move, isWhiteTurn) == TRUE)
					return TRUE;
				move->srcRow = i;
				move->srcCol = j;
			}
		}
		move->srcRow = backupSrcRow;
		move->srcCol = backupSrcCol;
		return FALSE;
	}
	//Runs over the whole board
	findWhiteKingLocation(move, boardCopy);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			//Check if each tool has a clear way and its doing a valid move in order to threat the king
			if (isWayFreeForChess(move, boardCopy, isWhiteTurn) == TRUE &&
				validMoveForChess(move, isWhiteTurn) == TRUE)
				return TRUE;
			move->srcRow = i;
			move->srcCol = j;
		}
	}
	move->srcRow = backupSrcRow;
	move->srcCol = backupSrcCol;
	return FALSE;
}

/******************
 * Function Name:getInfoPGN
 * Input:struct pointer move,char board [][SIZE] 2d array, char pgn[] string, int isWhiteTurn
 * Output:No output - void
 * Function Operation:The function proccess the input PGN into the right fields of struct move.
 ******************/
void getInfoPGN(char pgn[], Move* move, char board[][SIZE], int isWhiteTurn) {
	int length = 0;
	length = strlen(pgn);
	int isDestRow = TRUE;
	int isDestCol = TRUE;
	int isValidCol = 0;
	int counter = 0;
	//Runs over the whole PGN from the end to the start
	for (int i = length - 1; i >= 0; i--) {
		//Check which letter in PGN in order to put it in the correct field
		if (pgn[i] == '#')
			move->isMate = pgn[i];
		else if (pgn[i] == '+')
			move->isCheck = TRUE;
		else if (pgn[i] == '=') {
			move->ispromotion = TRUE;
			move->promotionPiece = pgn[i + 1];
			if (isWhiteTurn == FALSE)
				upperCaseToLowerCase(move->promotionPiece);
		}
		else if (pgn[i] >= '1' && pgn[i] <= '9') {
			//Flag after getting destination row, put source row
			if (isDestRow == TRUE) {
				move->destRow = pgn[i];
				isDestRow = FALSE;
			}
			else
				move->srcRow = pgn[i];
		}
		else if (pgn[i] >= 'a' && pgn[i] <= 'i') {
			//Flag after getting destination column, put source column
			if (isDestCol == TRUE) {
				isValidCol = chartoDigit(pgn[i]);
				isDestCol = FALSE;
				//Check if its legal column
				if (isValidCol < SIZE)
					move->destCol = pgn[i];
			}
			else {
				isValidCol = chartoDigit(pgn[i]);
				//Check if its legal row
				if (isValidCol <= SIZE)
					move->srcCol = pgn[i];
			}

		}
		else if (pgn[i] == 'x')
			move->iscapture = TRUE;
		else if ((i == 0) && (pgn[i] == 'K' || pgn[i] == 'Q' || pgn[i] == 'B' || pgn[i] == 'N' || pgn[i] == 'R')) {
			move->srcPiece = pgn[i];
		}
	}
	findBlackKingLocation(move, board);
	findWhiteKingLocation(move, board);
}

/******************
 * Function Name:initMove
 * Input:struct pointer move,int isWhiteTurn
 * Output:No output - void
 * Function Operation:The function initialize the move struct
 ******************/
void initMove(Move* move, int isWhiteTurn) {
	//Check if its black or white turn for correct default
	if (isWhiteTurn == TRUE)
		move->srcPiece = 'P';
	else
		move->srcPiece = 'p';
	move->srcRow = INVALID;
	move->srcCol = INVALID;
	move->destPiece = INVALID;
	move->destRow = INVALID;
	move->destCol = INVALID;
	move->promotionPiece = INVALID;
	move->tmpDest = INVALID;
	move->WKR = INVALID;
	move->WKC = INVALID;
	move->BKR = INVALID;
	move->BKC = INVALID;
	move->iscapture = FALSE;
	move->ispromotion = FALSE;
	move->isCheck = FALSE;
	move->isMate = FALSE;
}

/******************
 * Function Name:isDestFree
 * Input:struct pointer move,char board [][SIZE] 2d array
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if input destination is clear.
 ******************/
int isDestFree(Move* move, char board[][SIZE]) {
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	//Check if destination is empty
	if (board[iDest][jDest] == ' ')
		return TRUE;
	return FALSE;
}

/******************
 * Function Name:isHorizonLineClear
 * Input:struct pointer move,char board [][SIZE] 2d array
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if horizonal line from tool source to destination is clear.
 ******************/
int isHorizonLineClear(char board[][SIZE], Move* move) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	int iclimb = 0, jclimb = 0, lDest = 0, iFinal = 0, jFinal = 0;
	//Check if its white or black turn, therfore implement different destination
	if (jDest != jSrc)
		return FALSE;
	lDest = abs(iDest - iSrc);
	//Check if its going upwards or downwards
	if (iDest > iSrc)
		iclimb = 1;
	else
		iclimb = -1;
	//Run over each step for tool way
	for (int i = 1; i < lDest; i++) {
		iFinal = iSrc + iclimb * i;
		//Check if the step is empty
		if (board[iFinal][jDest] != ' ')
			return FALSE;
	}
	return TRUE;
}

/******************
 * Function Name:isStrightLineClear
 * Input:struct pointer move,char board [][SIZE] 2d array
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if straight line from tool source to destination is clear.
 ******************/
int isStrightLineClear(char board[][SIZE], Move* move) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	int iclimb = 0, jclimb = 0, lDest = 0, iFinal = 0, jFinal = 0;
	//Check if its a stright line
	if (iDest != iSrc)
		return FALSE;
	lDest = abs(jDest - jSrc);
	//Check if its going left or right
	if (jDest > jSrc)
		jclimb = 1;
	else
		jclimb = -1;
	//Run over each step for tool way
	for (int i = 1; i < lDest; i++) {
		jFinal = jSrc + jclimb * i;
		//Check if the step is empty
		if (board[iDest][jFinal] != ' ')
			return FALSE;
	}
	return TRUE;
}

/******************
 * Function Name:isDiagonalLineClear
 * Input:struct pointer move,char board [][SIZE] 2d array
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if diagonal line from tool source to destination is clear.
 ******************/
int isDiagonalClear(char board[][SIZE], Move* move) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	int iclimb = 0, jclimb = 0, lDest = 0, iFinal = 0, jFinal = 0;
	//Check if its diagonal line
	if (abs(iSrc - iDest) != abs(jSrc - jDest))
		return FALSE;
	//Check if its going upwards or downwards in diagonal line
	if (iDest > iSrc)
		iclimb = 1;
	else
		iclimb = -1;
	//Check if its going left or right in diagonal line
	if (jDest > jSrc)
		jclimb = 1;
	else
		jclimb = -1;
	lDest = abs(iDest - iSrc);
	//Run over each step for tool way
	for (int i = 1; i < lDest; i++) {
		iFinal = iSrc + iclimb * i;
		jFinal = jSrc + jclimb * i;
		//Check if the step is empty
		if (board[iFinal][jFinal] != ' ')
			return FALSE;
	}
	return TRUE;
}

/******************
 * Function Name:isWayFree
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check according to the input tool if its way clear to destination.
 ******************/
int isWayFree(Move* move, char board[][SIZE], int isWhiteTurn) {
	switch (move->srcPiece) {
	case 'K':
		return TRUE;
	case 'Q':
		//Check if it has a clear way to destination according to the tool right steps rules
		if (isStrightLineClear(board, move) == TRUE || isHorizonLineClear(board, move) == TRUE ||
			isDiagonalClear(board, move) == TRUE)
			return TRUE;
	case 'R':
		//Check if it has a clear way to destination according to the tool right steps rules
		if (isStrightLineClear(board, move) == TRUE || isHorizonLineClear(board, move) == TRUE)
			return TRUE;
		return FALSE;
	case 'B':
		//Check if it has a clear way to destination according to the tool right steps rules
		if (isDiagonalClear(board, move) == TRUE)
			return TRUE;
		return FALSE;
	case 'N':
		return TRUE;
	case 'P':
		//Check if it has a clear way to destination according to the tool right steps rules or if its capture
		if ((isHorizonLineClear(board, move) == TRUE) || (move->iscapture == TRUE))
			return TRUE;
		return FALSE;
	case 'p':
		//Check if it has a clear way to destination according to the tool right steps rules or if its capture
		if ((isHorizonLineClear(board, move) == TRUE) || (move->iscapture == TRUE))
			return TRUE;
		return FALSE;
	default:
		return FALSE;
	}
}

/******************
 * Function Name:validMove
 * Input:struct pointer move, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check according to the input tool,
 if the tool can make a legal step to destination.
 ******************/
int validMove(Move* move, int isWhiteTurn) {
	int iSrc = move->srcRow;
	int jSrc = move->srcCol;
	int iDest = SIZE - (charDigitttoDigit(move->destRow));
	int jDest = chartoDigit(move->destCol);
	switch (move->srcPiece) {
	case 'K':
		//Check if its a legal move to destination according to the tool right steps rules
		if ((abs(iDest - iSrc) == 0 && abs(jDest - jSrc) == 1) || (abs(iDest - iSrc) == 1 && (abs(jDest - jSrc) == 0)) ||
			(abs(iDest - iSrc) == 1 && abs(jDest - jSrc) == 1))
			return TRUE;
		return FALSE;
	case 'Q':
		//Check if its a legal move to destination according to the tool right steps rules
		if (((abs(iDest - iSrc) == 0) || (abs(jDest - jSrc) == 0)) || ((abs(iDest - iSrc)) == (abs(jDest - jSrc))))
			return TRUE;
		return FALSE;
	case 'R':
		//Check if its a legal move to destination according to the tool right steps rules
		if ((abs(iDest - iSrc) == 0) || (abs(jDest - jSrc) == 0))
			return TRUE;
		return FALSE;
	case 'B':
		//Check if its a legal move to destination according to the tool right steps rules
		if ((abs(iDest - iSrc)) == (abs(jDest - jSrc)))
			return TRUE;
		return FALSE;
	case 'N':
		//Check if its a legal move to destination according to the tool right steps rules
		if (((abs(iDest - iSrc) == 2) && (abs(jDest - jSrc) == 1)) || (((abs(iDest - iSrc) == 1) && (abs(jDest - jSrc) == 2))))
			return TRUE;
		return FALSE;
	default:
		//Check if there is capture in the current move
		if (move->iscapture == TRUE) {
			//Check if its black or white pawn
			if (isWhiteTurn == TRUE) {
				//Check if its legal white pawn capture step
				if ((iDest - iSrc == -1) && (abs(jDest - jSrc) == 1))
					return TRUE;
				return FALSE;
			}
			//Check if its legal black pawn capture step
			if ((iDest - iSrc == 1) && (abs(jDest - jSrc) == 1))
				return TRUE;
			return FALSE;
		}
		else {
			//Check if its legal pawn movement (not capture)
			if (jDest == jSrc) {
				//Check if its black or white pawn
				if (isWhiteTurn == TRUE) {
					//Check if its normal movement or 2 steps from a specific line
					if (iSrc == SIZE - 2 && iDest == SIZE - 4)
						return TRUE;
					else if (iDest - iSrc == -1)
						return TRUE;
					return FALSE;
				}
				//Check if its normal movement or 2 steps from a specific line
				if (iSrc == 1 && iDest == 3)
					return TRUE;
				else if (iDest - iSrc == 1)
					return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
}

/******************
 * Function Name:legalMove
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if the whole movement is legal for tool, from source to destination.
 ******************/
int legalMove(Move* move, int isWhiteTurn, char board[][SIZE]) {
	//Check if its a legal step
	if (validMove(move, isWhiteTurn) == TRUE) {
		//Check if the destination is clear
		if (isDestFree(move, board) == TRUE) {
			//Check if the way is clear
			if (isWayFree(move, board, isWhiteTurn) == TRUE) {
				return TRUE;
				return FALSE;
			}
			return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

/******************
 * Function Name:isValidMove
 * Input:struct pointer move,char board [][SIZE] 2d array, int isWhiteTurn, char pgn[] string
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function check if the whole move is legal,
 including special cases,except check,proccess PGN and get input tool source.
 ******************/
int isValidMove(char pgn[], Move* move, int isWhiteTurn, char board[][SIZE]) {
	getInfoPGN(pgn, move, board, isWhiteTurn);
	getToolSource(move, board, isWhiteTurn);
	int specialCaseCounter = 0;
	int counter = 0;
	//Check if special case promotion is true in move
	if (move->ispromotion == TRUE) {
		counter++;
		//Check if actually there is legal promotion in current move
		if (promotion(move, board, isWhiteTurn) == FALSE)
			return FALSE;
		specialCaseCounter++;
	}
	//Check if special case capture is true in move
	if (move->iscapture == TRUE) {
		counter++;
		//Check if actually there is legal capture in current move
		if (capture(move, board, isWhiteTurn) == FALSE)
			return FALSE;
		specialCaseCounter++;
	}
	//Check if current move is legal and special cases
	if ((legalMove(move, isWhiteTurn, board) == TRUE) || ((counter == specialCaseCounter) && (counter > 0)))
		return TRUE;
	return FALSE;
}

/******************
 * Function Name:makeMove
 * Input:char board [][SIZE] 2d array, int isWhiteTurn, char pgn[] string
 * Output:int 1 or 0 (True or False)
 * Function Operation:The function initializes struct move,check whole move is legal,
 also passive and active check by using copy board and change the board.
 ******************/
int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
	Move move;
	initMove(&move, isWhiteTurn);
	//Check if move is legal
	if (isValidMove(pgn, &move, isWhiteTurn, board) == TRUE) {
		//Check if check is turned on in move struct
		if (move.isCheck == TRUE) {
			copyBoard(boardCopy, board);
			changePosition(&move, boardCopy, isWhiteTurn);
			getToolSource(&move, boardCopy, isWhiteTurn);
			//Check if there is legal check
			if (Check(&move, boardCopy, isWhiteTurn) == TRUE) {
				copyBoard(board, boardCopy);
				return TRUE;
			}
			return FALSE;
		}
		changePosition(&move, board, isWhiteTurn);
		return TRUE;
	}
	return FALSE;
}
