#include <iostream>
#include <iomanip>
#include <vector>
#include "mersenne-twister.h"

using namespace std;

// declare functions 
void drawBoard(int board[4][4]);
void fillBoardWithNums(int board[4][4], char levelChoice);
void moveRight(int board[4][4]);
void moveLeft(int board[4][4]);
void moveUp(int board[4][4]);
void moveDown(int board[4][4]);
void winningOrLosing(int board[4][4], char levelChoice, bool &exitGame);
int chooseRandomNumber(int min, int max);
	

int main() 
{
    // generate the random seed
    cout << "Enter random seed: " << endl;
    int randSeed;
    cin >> randSeed;
    seed(randSeed);
    
    // declare board and initialized it as an empty 2D-array
    int userBoard[4][4] = {0};
    char userChoice;
       
    // ask user to enter a valid game mode  
    do {
        cout << "Choose game mode: Easy (E), Medium (M), or Hard (H): " << endl;
        cin >> userChoice;
        userChoice = toupper(userChoice); // convert char to its upper form
        if (userChoice != 'E' && userChoice != 'M' && userChoice != 'H'){
            cout << "Error: Invalid mode." << endl;
        }
    } while (userChoice != 'E' && userChoice != 'M' && userChoice != 'H');

    // add two first numbers to the board at the begining of the game
    for (int i = 0; i < 2; i++){
        fillBoardWithNums(userBoard, userChoice);
    }

    // draw the board after adding numbers 
    drawBoard(userBoard);

    // using while loop to run the gameplay
    char userMoves;
    // variable to track the game progress. Once its value is true, the program will stop. 
    bool exitGame = false;

    while (!exitGame) {     // the loop will stop whenever exitGame is true
        // promt user for the move input
        cout << "Enter move: U, D, L, or R. Q to quit: " << endl;
        cin >> userMoves;

        // switch statements to handle the user's move input
        // user enter R to shift numbers to the right, L to shift to the left
        // U to move upward, D to move downward and Q to quit the program
        // default case is any other letters, the program will display 'error statement
        // and ask for a valid input.  
        switch(toupper(userMoves)) {
            case 'R':
                moveRight(userBoard);
                break;
            case 'L':
                moveLeft(userBoard);
                break;
            case 'U':
                moveUp(userBoard);
                break;
            case 'D':
                moveDown(userBoard);
                break;
            case 'Q':
                return -1;  // exit the program as soon as Q is entered
            default:
                cout << "Error: Invalid move." << endl;
                continue;
        }
        // update and draw a new board after each move
        fillBoardWithNums(userBoard, userChoice);
        cout << endl;
        drawBoard(userBoard);

        // check and print out an apporiate statement either player wins or loses
        winningOrLosing(userBoard, userChoice, exitGame);
        
    }       
    return 0;
}

//define functions 

// draw a 4x4 game board in a readable grid format
void drawBoard(int board[4][4]){
    cout << "---------------------" << endl;
    for(int i = 0; i < 4; i++){
        cout << "|";
        for(int j = 0; j < 4; j++){
            if(board[i][j]== 0){
                cout << "    "; 
            }
            else{
                cout << setw(4) << board[i][j];
            }
        cout << "|";   
        }
        cout << endl;
        cout << "---------------------" << endl;

    }
}

// random and add an appriate number (either 2 or 4) to the game board 
void fillBoardWithNums(int board[4][4], char levelChoice){
    // declare a array which uses to store the empty space's index
    vector <int> emptySpaceIndexes;
    int fillBoardWith;
    // find all empty spaces and convert these empty spaces' indexes from 2D-array format to 1D-array format,
    // then stores them in emptySpaceIndexes vector
    // for example, if board[2][3] is an empty space, its index to store in 1D-array will be 11
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (board[i][j] == 0){
                emptySpaceIndexes.push_back(i * 4 + j); // (i * 4 + j) is the index of empty spaces in 1D-array
            }
        }
    }

    // test 1D-array output
    /*
    for(int i = 0; i < emptySpaceIndexes.sizs(); i++){
        cout << "The value at index " << i << " is " << emptySpaceIndexes.at(i) << endl;
    }
    cout << "The size of 1D-array is " << emptySpaceIndexes.size() << endl;
    */

    // condition to check if there is no empty space, no more steps need
    if (emptySpaceIndexes.size() == 0){
        return;
    }

    // fill the board with a specific random number either 2 or 4
    while(emptySpaceIndexes.size() != 0){
        // choice a random index from the list of empty space
        int randIndex = chooseRandomNumber(0, emptySpaceIndexes.size() -1);
        int randIndexValue = emptySpaceIndexes.at(randIndex);
        
        // calculate the index of row and column from the index value
        // the row index is the quotient of devision randIndexValue by 4
        // the column index is the remainder of devision randIndexValue by 4
        int rowIndex = randIndexValue / 4;
        int colIndex = randIndexValue % 4;

        // determine which numbers (2 or 4) would be used to fill the board
        // the frequenency appearing of 2 is based on the mode that the user select
        // 50% the next added number to the board will 2 if the current mode is easy 
        // 70% if the current mode is medium and 90% if the current mode is hard
        int randNum = chooseRandomNumber(1, 10);
        if ((randNum <= 5 && toupper(levelChoice) == 'E') ||
            (randNum <= 7 && toupper(levelChoice) == 'M') ||
            (randNum <= 9 && toupper(levelChoice) == 'H')){
            
            fillBoardWith = 2;
        }
        else {
            fillBoardWith = 4;
        }
        board[rowIndex][colIndex] = fillBoardWith;
        
        //remove the chosen index from the empty space list 
        emptySpaceIndexes.erase(emptySpaceIndexes.begin() + randIndex);
        break;
    }   
}

// function to shift numbers on board to the right by one space
void moveRight(int board[4][4]){
    // declare a temporary 2D-array with same size as the main board
    // this array is used to temporarily store the sum of two numbers on the main board
    int tempBoard[4][4] = {0};
 
    for(int row = 0; row < 4; row++){
    // combine two equal numbers which are sitting next to each other on the same row.
    // store the sum of those two in a temporary 2D-array.
    // replace used numbers with 0.
        for(int col = 2; col >= 0; col--){
            if(board[row][col] == board[row][col + 1]){
                tempBoard[row][col + 1] = 2 * board[row][col];
                board[row][col] = 0;
                board[row][col + 1] = 0;
            }
        }
    
    // shift the entire numbers which are different from 0 on the board to the right
        for(int col = 2; col >=0; col--){
            if (board[row][col] != 0 && board[row][col + 1] == 0){
                board[row][col + 1] = board[row][col];
                board[row][col] = 0;
            }
        }
    
    // return the sum of two same numbers from temporary 2D-array to the main board       
        for(int col = 0; col < 4; col++){
            if (tempBoard[row][col] != 0 && board[row][col] == 0){
                board[row][col] = tempBoard[row][col];
            }
        }
    }
}

// function to shift numbers on board to the left by one space
void moveLeft(int board[4][4]){
    int tempBoard[4][4] = {0};
    // combine to numbers which are sitting next to each other to the left
    // store the sum of those two numbers in a temporary 2D-array
    // replace all used numbers with 0
    for (int row = 0; row < 4; row++){
        for(int col = 1; col < 4; col++ ){
            if( board[row][col] != 0 && board[row][col] == board[row][col - 1]){
                tempBoard[row][col - 1] = 2 * board[row][col];
                board[row][col] = 0;
                board[row][col - 1]= 0;
            }
        }
    
    // shift the entire numbers which are different from 0 on the board to the left    
        for(int col = 0; col < 3; col++){
            if(board[row][col] == 0 && board[row][col + 1] != 0){
                board[row][col] = board[row][col + 1];
                board[row][col + 1] = 0;
            }
        }

    // return the sum of two numbers from the temperary 2D-array to the board
        for(int col = 0; col < 4; col++){
            if(board[row][col] == 0 && tempBoard[row][col] != 0){
                board[row][col] = tempBoard[row][col];
            }
        }
        
    }

}

// function to move numbers on board upward by one space
void moveUp(int board[4][4]){
    int tempBoard[4][4] = {0};
    for (int col = 0; col < 4; col++){ 
        // combine two numbers which are sitting next to each other on the same column in upward direction.
        // store the sum of those two numbers in a temporary 2D-array.
        // replace used numbers with 0.   
        for (int row = 1; row < 4; row ++){
            if (board[row][col] != 0 && board[row][col] == board[row - 1][col]){
                tempBoard[row - 1][col] = 2 * board[row][col];
                board[row - 1][col] = 0;
                board[row][col] = 0;
            }
        }

        // shift the entire nonzero numbers on the board upward
        for(int row = 1; row < 4; row++){
            if(board[row - 1][col] == 0 && board[row][col] != 0){
                board[row - 1][col] = board[row][col];
                board[row][col] = 0;
            }
        }

        //return the sum of two same numbers from the temporary board to the main board
        for(int row = 0; row < 4; row++){
            if(board[row][col] == 0 && tempBoard[row][col] != 0){
                board[row][col] = tempBoard[row][col];
            }
        }

    }

}

// function to move numbers on board downward by one space
void moveDown(int board[4][4]){
    int tempBoard[4][4] = {0};
    for (int col = 0; col < 4; col++){ 
        // combine two numbers which are sitting next to each other on the same column in downward direction.
        // store the sum of those two numbers in a temporary 2D-array.
        // replace used numbers with 0.   
        for (int row = 2; row >= 0; row--){
            if (board[row][col] != 0 && board[row][col] == board[row + 1][col]){
                tempBoard[row + 1][col] = 2 * board[row][col];
                board[row + 1][col] = 0;
                board[row][col] = 0;
            }
        }

        // shift the entire nonzero numbers on the board downward
        for(int row = 2; row >= 0; row--){
            if(board[row + 1][col] == 0 && board[row][col] != 0){
                board[row + 1][col] = board[row][col];
                board[row][col] = 0;
            }
        }

        //return the sum of two same numbers from the temporary board to the main board
        for(int row = 0; row < 4; row++){
            if(board[row][col] == 0 && tempBoard[row][col] != 0){
                board[row][col] = tempBoard[row][col];
            }
        }
    }

}

// fuction to check winning and losing
// the user wins if they reach the target numbers which depend on the mode.
// the target number is 256 for easy mode, 512 for medium and 1024 for hard.
// if the spaces on the table are all occupied and user hasn't got the target number yet, they lose.
// the function takes 3 parameter:
// - board[4][4]
// - levelchoice: the character representing the current mode
// - exitGame: a reference to the boolean variable that determine if the game is over.
//   the game ends once exitGame is set to 'true' which means either user reaches target number or the board is full
void winningOrLosing(int board[4][4], char levelChoice, bool &exitGame){
    // variable that determine if player win as the winning number occurs, 
    // initialized with false value as player hasn't reached target yet
    bool playerWin = false;

    // varible to check if there exists empty spaces on the board
    // initialized with true value as I assume all spaces are filled
    bool noEmptySpace = true;

    // variable stores the winning number 
    int targetNumber = 0;

    // assign the winning number corresponding to the user level choice
    switch(toupper(levelChoice)){
        case 'E':
            targetNumber = 256;
            break;
        
        case 'M':
            targetNumber = 512;
            break;

        case 'H':
            targetNumber = 1024;
        
        default:
            break;
    }
    // look up the board to check the current empty spaces status, 
    // also check if the winning number occurs 
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(board[i][j] == targetNumber){
                playerWin = true;
            }
            if(board[i][j] == 0){
                noEmptySpace = false;
            }
        }
    }
    // player wins as reaching the winning number
    if (playerWin){
        cout << "You win!" << endl;
        exitGame = true;
        //break;
    }
    // player loses as the board is full and the winning number didn't exist
    if(noEmptySpace){
        cout << "You lose." << endl;
        exitGame = true;
    }
}

int chooseRandomNumber(int min, int max) {
	return rand_u32() % (max + 1 - min) + min;
}
