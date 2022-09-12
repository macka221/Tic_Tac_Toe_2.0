#include <iostream>
#include <string>

#define MAX_ROW 11
#define MAX_COL 15
#define MIN_PLAYER 1
#define MAX_PLAYER 7
#define MAX_GAMES 1000

using namespace std;

struct player{
    string fName, lName;
    int win = 0, loss = 0, draw = 0;
    char piece;
};

bool checkNumInput(string);// arg:input
bool checkName(string); // arg: name
bool countSpaces(string);// arg: name
bool checkBounds(int, int, int);// arg: stringToInt, maxLimit, minLimit
bool checkFullSpot(char [][MAX_COL], int, int);// arg: board, row, col
bool checkMoveInput(string);// arg: inString
bool checkDraw(char [][MAX_COL], int, int);// args: board[11][15], row, col
bool checkAcross(char[][MAX_COL], char, int, int);// args: board[11][15], piece, row, col
bool checkDiagonal(char [][MAX_COL], char, int, int);// args: board[11][15], piece, row, col
bool checkDown(char [][MAX_COL], char, int, int);// args: board[11][15], piece, row, col
bool checkWin(char [][MAX_COL], char, int, int);// args: board[11][15], piece, row, col
bool checkInput(string, char, int, int);// arg: input, criteria, row, col
int stringToInt(string);// arg: stringInt
int pow(int, int); // arg: base, power
int checkLongestName(player[], int);// arg: users, totalPlayers
int stripRow(string);// arg: userInput
int stripCol(string);// arg: uSerInput
int getRow();
int getCol();
int getPlayers();
char playGame(player[], int, int);// args: users, startingPlayer, numPlayer
char playAgain();
string capitalizeFirstLetter(string);// arg: name
string createBoarder(int);// args: col
string createFormat(string, int);// arg: inString, largestName
void createHeader(int); // args: col
void createPlayer(player&, char);// arg: user, piece
void setName(string, player&); // arg: input, user
void setPiece(char, player&);// arg: piece, user
void calculateScore(player&, char);// arg: user, winningPiece
void displayStats(player[], int, int);// arg: users, totalGames, totalPlayer
void displayBoard(char [][MAX_COL], int, int);// args: board, row, col
void resetBoard(char [][MAX_COL]);//arg: board[11][15]
void placePiece(char [][MAX_COL],char, int, int);//arg: board[11][15], piece, row, col
void capitalizeWinConfig(char[][MAX_COL], int[], int[], int[]);// arg: board[11][15], pos1[2], pos2[2], pos3[2]

int main()
{
    int totalPlayers = 0, totalGames = 0, startPlayer = 0;
    char cont;
    string temp, userInput;

    cout << "Welcome to the extended version of Tic-Tac-Toe.\n"
         << "The player will first enter the amount of players,\n"
         << "then each player will enter the name starting with a\n"
         << "capital letter for the beginning of the first and last\n"
         << "name. Then the player will select the size of the\n"
         << "board at the start of each game. Enjoy!.\n\n";

    totalPlayers = getPlayers();
    player users[totalPlayers];

    cout << endl;

    for (int i = 0; i < totalPlayers; i++)
    {
        createPlayer(users[i], 'a' + i);
    }

    do
    {
        if (startPlayer >= totalPlayers)
        {
            startPlayer = 0;
        }

        char winner = playGame(users, startPlayer, totalPlayers);
        totalGames++;
        startPlayer++;

        for (int i = 0; i < totalPlayers; i++)
        {
            calculateScore(users[i], winner);
        }

        displayStats(users, totalGames, totalPlayers);
        totalGames == MAX_GAMES ? cont = 'n' : cont = playAgain();

        if (totalGames == MAX_GAMES)
        {
            cout << "Maximum number of games(" << MAX_GAMES << ") has been"
                 << " reached.";
        }

    } while (cont == 'y');


    return 0;
}

//=========================================================================
//    Args:
//        -input: name of user
//
// Checks to make sure that a name follow specific criteria.
//    -Consists of only letters
//    -Consists of one space between first and last name
//    -size is greater than 2 characters
//=========================================================================
bool checkName(string input)
{
    bool safe = false;

    for(int i = 0; i < input.size(); i++)
    {
        if (((input[i] >= 'A' && input[i] <= 'Z') ||
            (input[i] >= 'a' && input[i] <= 'z') ||
            (input[i] ==  ' ')) && input.size() > 2)
        {
            safe = true;

            if (input[i] == ' ')
            {
                safe = false;
            }
        }
        else
        {
            safe = false;
            break;
        }
    }

    return safe;
}

//=========================================================================
//    Args:
//        - name: one part of each person's name
//
// Capitalizes the first letter of each name if it is not already done.
//=========================================================================
string capitalizeFirstLetter(string name)
{
    if (name[0] >= 'a')
    {
        name[0] = name[0] - 32;
    }

    return name;
}

//=========================================================================
//    Args:
//        -input: numerical input as a string type
//
// Checks to make sure the string is only integers, and the value is not
// null.
//=========================================================================
bool checkNumInput(string input)
{
    bool safe;
    int i = 0;

    while (i < input.size() && safe)
    {
        if (input[i] <= '0' && input[i] <= '9')
        {
            safe = true;
        }

        i++;
    }

    return (safe && input.size() > 0);
}

//=========================================================================
//    Args:
//        -name: input of the users name in format of "first name last name"
//        -user: the current player
//
// Uses the space character as a delimiter and sets the first and last
// names of each player.
//=========================================================================
void setName(string name, player& user)
{
    string n = "";

    for (int i = 0; i < name.size();i++)
    {
        char current = name[i];

        if (current == ' ')
        {
            user.fName = n;
            n = "";
        }
        else
        {
            n = n + current;
        }
    }

    user.lName = n;
}

//=========================================================================
//    Args:
//        -piece: piece each player will use
//        -user: current player
//
// Sets a specfic piece to the current player.
//=========================================================================
void setPiece(char piece, player& user)
{
    user.piece = piece;
}

//=========================================================================
//    Args:
//        -user: current player
//        -piece: piece to be set to  a player
//
// Creates and stores all of the user's data.
//=========================================================================
void createPlayer(player &user, char piece)
{
    string temp;
    bool safe = false;

    while (!safe)
    {
        cout <<"Please enter the name of the player -> ";
        getline(cin, temp);
        cin.ignore(0, '\n');

        safe = checkInput(temp, 'n', 0, 0);

        if (!safe)
        {
            cout << "Invalid input. Try again.\n";
        }
    }

        setName(temp, user);
        user.fName = capitalizeFirstLetter(user.fName);
        user.lName = capitalizeFirstLetter(user.lName);
        user.piece = piece;
}

//=========================================================================
//    Args:
//        -input: users name
//
// Counts and makes sure there is only one space in the input and that the
// input doesn't start with a space.
//=========================================================================
bool countSpaces(string input)
{
    int counter = 0;

    if (input[counter] == ' ')
        return false;

    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == ' ')
        {
            counter++;
        }
    }

    return (counter == 1);
}

//=========================================================================
//    Args:
//        -col: the total columns in the board
//
// Creates the top and bottom border of the board.
//=========================================================================
void createHeader(int col)
{
    cout << "   ";

    for (int i = 0; i < col; i++)
    {
        if (i < 9)
        {
            cout << " " << i + 1 << "  ";
        }
        else
        {
            cout << " " << i + 1 << " ";
        }
    }

    cout << endl;
}

//=========================================================================
//    Args:
//        -stringInt: a string containing integer values
//
// Converts a string of confirmed integers to the integer type.
//=========================================================================
int stringToInt(string stringInt)
{
    int total = 0;
    int valSize = stringInt.size();

    for (int i = valSize; i > 0; i--)
    {
        int multiple = pow(10, valSize - i);
        int currentPos = stringInt[i-1] - 48;

        total = total + (currentPos * multiple);
    }

    return total;
}

//=========================================================================
//    Args:
//        -base: base value
//        -power: exponent
//
// Represent the algorithm of exponents.
//=========================================================================
int pow(int base, int power)
{
    int total = 1;

    for (int i = 0; i < power; i++)
        total *= base;

    return total;
}

//=========================================================================
//    Args:
//        -col: columns of board
//
// Takes in an integer col and multiplys the string " ---" by col.
//=========================================================================
string createBoarder(int col)
{
    string boarder = "  ";

    for (int i = 0; i < col; i++)
    {
        boarder = boarder + " ---";
    }

    boarder = boarder + '\n';

    return boarder;
}

//=========================================================================
//    Args:
//        -users[]: list of players
//        -totalPlayers: total players in the list
//
// Checks and returns the size of the longest name in the list of players.
//=========================================================================
int checkLongestName(player users[], int totalPlayers)
{
    int largestName = 0;

    for (int i = 0; i < totalPlayers; i++)
    {
        player current = users[i];
        string fullName = current.fName + " " + current.lName;

        if (largestName < fullName.size())
            largestName = fullName.size();
    }

    return largestName;
}

//=========================================================================
//    Args:
//        -users[]: list of players
//        -totalGames: total games played
//        -totalPlayers: total number of players
//
// Displays the stats of each player in users in a specific format and
// displays the total number of games played.
//=========================================================================
void displayStats(player users[], int totalGames, int totalPlayers)
{
    cout << "\nTotal game(s) played - " << totalGames << endl << endl;

    int longestName = checkLongestName(users, totalPlayers);
    string divider = "  ------ ------ ------\n", newDivider;
    string header = " |  WIN | LOSS | DRAW |\n", newHeader;

    newDivider = createFormat(divider, longestName);
    newHeader = createFormat(header, longestName);

    cout << newDivider << newHeader << newDivider;

    for (int i = 0; i < totalPlayers; i++)
    {
        player current = users[i];
        string fullName = current.fName + " " + current.lName;
        string newName = createFormat(fullName, longestName);

        cout << newName << " |    " << current.win << " |    "
             << current.loss << " |    " << current.draw << " |\n"
             << newDivider;
    }

}

//=========================================================================
//    Arg:
//        -inString: string to be formated
//        -largestName: size of largest name
//
// Formats inString with spaces so that the inString formats correctly in
// the display of the statistics.
//=========================================================================
string createFormat(string inString, int largestName)
{
    string outString = "";
    int format = largestName - inString.size();

    if (inString[0] == ' ' || inString[0] == '|')
    {
        for (int i = 0; i < largestName; i++)
            outString = outString + " ";
    }

    else
    {
        for (int i = 0; i < format; i++)
            outString = outString + " ";
    }

    return outString + inString;
}

//=========================================================================
//    Arg:
//        -board: game board
//        -row: rows of board
//        -col: columns of board
//
// Displays a game board according to a specified format.
//=========================================================================
void displayBoard(char board[MAX_ROW][MAX_COL], int row, int col)
{
    string boarder = createBoarder(col);

    cout << endl;
    createHeader(col);

    char rowIndex = 'A';

    for (int i = 0; i < row; i++)
    {
        cout << boarder << rowIndex << " ";

        for (int j = 0; j < col; j++)
        {
            cout << "| " << board[i][j] << " ";
        }

        cout << "| " << rowIndex << "\n";

        rowIndex++;
    }

    cout << boarder;
    createHeader(col);
    cout << endl;
}

//=========================================================================
//    Arg:
//        -board: game board
//
// Necessary to reset the board to all space characters.
//=========================================================================
void resetBoard(char board[MAX_ROW][MAX_COL])
{
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            board[i][j] = ' ';
        }
    }
}

//=========================================================================
//    Args:
//        -row: character value of a row
//
// Checks and returns a verified index to match row with an integer value
// used to place the piece in the board.
//=========================================================================
int stripRow(char row)
{
    if (row >= 'a')
        row = row - 32;

    return row - 'A';
}

//=========================================================================
//    Args:
//        -board: game board
//        -piece: user's piece
//        -row: intended row
//        -col: intended column
//
// Places the user's piece in the specified position according to row and
// column.
//=========================================================================
void placePiece(char board[MAX_ROW][MAX_COL], char piece, int row, int col)
{
    board[row][col] = piece;
}

//=========================================================================
//    Args:
//        -userInput: string containg a letter and a an integer
//
// Strips the column index from the string userInput, and returns it - 1 to
// index the location of the board.
//=========================================================================
int stripCol(string userInput)
{
    string temp;

    for (int i = 1; i <userInput.size(); i++)
    {
        temp = temp + userInput[i];
    }

    return stringToInt(temp) - 1;
}

//=========================================================================
//    Args:
//        -striToInt: converted string to int
//        -maxLimit: maximum bounds of allowed input
//        -minLimit: minimum bounds of allowed input
//
// Checks to make sure the converted int is withing a set bounds given
// maxLimit and minLimit are provided.
//=========================================================================
bool checkBounds(int striToInt, int maxLimit, int minLimit)
{
    return minLimit <= striToInt && striToInt <= maxLimit;
}

//=========================================================================
//    Args:
//        -board: game board
//        -row: specified row
//        -col: specified column
//
// Checks to make sure that the requested move is availible in the board.
//=========================================================================
bool checkFullSpot(char board[MAX_ROW][MAX_COL], int row, int col)
{
    return board[row][col] == ' ';
}

//=========================================================================
//    Args:
//        -inString: users move input
//
// Checks to make sure that inString follows a particular criteria of a row
// denoted by a letter and the column denoted by an integer. Example) A2
//=========================================================================
bool checkMoveInput(string inString)
{
    bool safe = true;

    int i = 1;

    if (('A' > inString[0] && 'Z' < inString[0]) ||
        ('a' > inString[0] && 'z' < inString[0]))
        {
            safe = false;
        }

    while (i < inString.size() && safe)
    {
        if ((i == 1 && inString[i] == '0') ||
            ('0' > inString[i] || inString[i] > '9'))
        {
            safe = false;
        }

        i++;
    }

    return (safe && inString.size() >= 2);
}

//=========================================================================
//    Args:
//        -board: game board
//        -pos1, pos2, pos3: index[0] = row, index[1] = column
//
// Takes in the value of every winning position and capitalizes them.
//=========================================================================
void capitalizeWinConfig(char board[MAX_ROW][MAX_COL], int pos1[2], int pos2[2], int pos3[2])
{
    // In case of already capitalized character this will ignore it.
    int pos1a, pos1b;
    int pos2a, pos2b;
    int pos3a, pos3b;

    pos1a = pos1[0], pos1b = pos1[1];
    pos2a = pos2[0], pos2b = pos2[1];
    pos3a = pos3[0], pos3b = pos3[1];

    if ('a' <= board[pos1a][pos1b])
    {
        board[pos1a][pos1b] = board[pos1a][pos1b] - 32;
    }

    if ('a' <= board[pos2a][pos2b])
    {
        board[pos2a][pos2b] = board[pos2a][pos2b] - 32;
    }

    if ('a' <= board[pos3a][pos3b])
    {
        board[pos3a][pos3b] = board[pos3a][pos3b] - 32;
    }
}

//=========================================================================
//    Args:
//        -board: game board
//        -row: amount of row
//        -col: amount of column
//
// Checks every space in the board to make sure each space is full. This
// function uses the product of the row and column as a terminal point to
// determine if the board is full.
//=========================================================================
bool checkDraw(char board[MAX_ROW][MAX_COL], int row, int col)
{
    int maxCount = row * col;
    int counter = 0;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (board[i][j] != ' ')
            {
                counter++;
            }
        }
    }

    return counter == maxCount;
}

//========================================================================
//    Args:
//        -board: game board
//        -piece: user's piece
//        -row: current row
//        -col: current column
//
// Uses the current position to check and see if there are winning
// configurations relative to the given position lengthwise.
//=========================================================================
bool checkAcross(char board[MAX_ROW][MAX_COL], char piece, int row, int col)
{
    bool cond1, cond2, cond3;

    cond1 = (board[row][col-1] == piece && board[row][col-2] == piece);
    cond2 = (board[row][col+1] == piece && board[row][col+2] == piece);
    cond3 = (board[row][col-1] == piece && board[row][col+1] == piece);

    return (cond1 || cond2 || cond3);
}

//========================================================================
//    Args:
//        -board: game board
//        -piece: user's piece
//        -row: current row
//        -col: current column
//
// Uses the current position to check and see if there are winning
// configurations relative to the given position vertically.
//=========================================================================
bool checkDown(char board[MAX_ROW][MAX_COL], char piece, int row, int col)
{
    bool cond1, cond2, cond3;

    cond1 = (board[row-1][col] == piece && board[row-2][col] == piece);
    cond2 = (board[row+1][col] == piece && board[row+2][col] == piece);
    cond3 = (board[row-1][col] == piece && board[row+1][col] == piece);

    return (cond1 || cond2 || cond3);
}

//========================================================================
//    Args:
//        -board: game board
//        -piece: user's piece
//        -row: current row
//        -col: current column
//
// Uses the current position to check and see if there are winning
// configurations relative to the given position diagonally.
//=========================================================================
bool checkDiagonal(char board[MAX_ROW][MAX_COL], char piece, int row, int col)
{
    bool win = false;

    if (board[row-1][col-1] == piece)
    {
        if (board[row - 2][col - 2] == piece || board[row+1][col+1] == piece)
            {
                win = true;
            }
    }
    else if (board[row-1][col+1] == piece)
    {
        if (board[row+1][col-1] == piece || board[row-2][col-2] == piece)
            {
                win = true;
            }
    }
    else if ((board[row+1][col+1] == piece && board[row+2][col+2] == piece) ||
            (board[row+1][col-1] == piece && board[row+2][col-2] == piece))
    {
        win = true;
    }

    return win;
}

//========================================================================
//    Args:
//        -board: game board
//        -piece: user's piece
//        -row: current row
//        -col: current column
//
// Uses the current position to check and for all possible winning
// configurations relative to the given position.
//=========================================================================
bool checkWin(char board[MAX_ROW][MAX_COL], char piece, int row, int col)
{
    bool winAcross, winDown, winDiagonal;
    int winPos1[2], winPos2[2], winPos3[2];

    winAcross = checkAcross(board, piece, row, col);
    winDown = checkDown(board, piece, row, col);
    winDiagonal = checkDiagonal(board, piece, row, col);

    if (winAcross)
    {
        winPos1[0] = row, winPos2[0] = row, winPos3[0] = row;

        if (board[row][col-2] == piece && board[row][col-1] == piece)
        {
            winPos1[1] = col-2, winPos2[1] = col-1, winPos3[1] = col;
        }
        else if (board[row][col+1] == piece && board[row][col+2] == piece)
        {
            winPos1[1] = col, winPos2[1] = col+1, winPos3[1] = col+2;
        }
        else
        {
            winPos1[1] = col-1, winPos2[1] = col; winPos3[1] = col+1;
        }

        capitalizeWinConfig(board, winPos1, winPos2, winPos3);
    }

    if (winDiagonal)
    {
        winPos2[0] = row, winPos2[1] = col;

        if (board[row-1][col-1] == piece && board[row+1][col+1] == piece)
        {
            winPos1[0] = row-1, winPos1[1] = col-1;
            winPos3[0] = row+1, winPos3[1] = col+1;
            capitalizeWinConfig(board, winPos1, winPos2, winPos3);
        }

        if (board[row-1][col+1] == piece && board[row+1][col-1] == piece)
        {
            winPos1[0] = row-1, winPos1[1] = col+1;
            winPos3[0] = row+1, winPos3[1] = col-1;
            capitalizeWinConfig(board, winPos1, winPos2, winPos3);
        }
        else if (board[row-1][col-1] == piece && board[row-2][col-2] == piece)
        {
            winPos1[0] = row-1, winPos1[1] = col-1;
            winPos3[0] = row-2, winPos3[1] = col-2;
            capitalizeWinConfig(board, winPos1, winPos2, winPos3);
        }
        else if (board[row+1][col+1] == piece && board[row+2][col+2] == piece)
        {
            winPos1[0] = row+1, winPos1[1] = col+1;
            winPos3[0] = row+2, winPos3[1] = col+2;
            capitalizeWinConfig(board, winPos1, winPos2, winPos3);
        }
    }

    if (winDown)
    {
        winPos1[1] = col, winPos2[1] = col, winPos3[1] = col;

        if (board[row-1][col] == piece && board[row-2][col] == piece)
            winPos1[0] = row-1, winPos2[0] = row-2, winPos3[0] = row;
        else if (board[row+1][col] == piece && board[row+2][col] == piece)
            winPos1[0] = row+1, winPos2[0] = row+2, winPos3[0] = row;
        else if (board[row-1][col] == piece && board[row+1][col] == piece)
            winPos1[0] = row-1, winPos2[0] = row+1, winPos3[0] = row;

        capitalizeWinConfig(board, winPos1, winPos2, winPos3);
    }

    return (winAcross || winDiagonal || winDown);
}

//=========================================================================
//    Args:
//        -user: current player
//        -winPiece: winning players piece otherwise 'x' if a draw
//
// Calculates the score of the current player.
//=========================================================================
void calculateScore(player& user, char winPiece)
{
    if (winPiece == 'x')
    {
        user.draw++;
    }
    else if (user.piece == winPiece)
    {
        user.win++;
    }
    else
    {
        user.loss++;
    }
}

//=========================================================================
//    Args:
//        -users[]: list of all players
//        -startingPlayer: player who starts this iteration of the game
//        -totalPlayers: used to iterate through the array of players
//
// Plays each round of tic tac toe.
//=========================================================================
char playGame(player users[], int startingPlayer, int totalPlayers)
{
    char board[MAX_ROW][MAX_COL];
    bool draw, win;
    int i = startingPlayer, rowSize, colSize;

    rowSize = getRow();
    colSize = getCol();
    resetBoard(board);



    while (!win && !draw)
    {
        int row, col;
        string temp;
        bool safe;



        if (i >= totalPlayers)
            i = 0;

        player current = users[i];
        displayBoard(board, rowSize, colSize);

        do
        {

            cout << current.fName << " (" << current.piece << "),"
                 << " please select a move(ex A2)-> ";
            getline(cin, temp);
            cin.clear();

            safe = checkInput(temp, 'm', rowSize - 1, colSize - 1);

            if (!safe)
            {
                cout << "Invalid input! Please try again.\n";
            }
            else
            {
                char r = temp[0];
                row = stripRow(r), col = stripCol(temp);
                safe = checkFullSpot(board, row, col);
                if (!safe)
                {
                    cout << "Invalid input! Please try again.\n";
                }
            }
        } while (!safe);

        placePiece(board, current.piece, row, col);
        win = checkWin(board, current.piece, row, col);
        draw = checkDraw(board, rowSize, colSize);

        if (win)
        {
            displayBoard(board, rowSize, colSize);

            return current.piece;
        }

        i++;
    }

    displayBoard(board, rowSize, colSize);

    return 'x';
}

//=========================================================================
//    Args:
//        -input: string to be evaluated
//        -criteria: determines which criteria to evaluate any given input
//        -bRow, bCol: bounds of the row and columns respectfully
//
// Checks each string based on the criteria of each input.
//=========================================================================
bool checkInput(string input, char criteria, int bRow, int bCol)
{
    bool safe;

    if (criteria == 'r' || criteria == 'c')
    {
        safe = checkNumInput(input);

        if (safe)
        {
            int checkNum = stringToInt(input);

            criteria == 'r' ? safe = checkBounds(checkNum, MAX_ROW, 3) :
                              safe = checkBounds(checkNum, MAX_COL, 3);
        }
    }
    else if (criteria == 'p')
    {
        safe = checkNumInput(input);

        if (safe)
        {
            int checkNum = stringToInt(input);

            safe = checkBounds(checkNum, MAX_PLAYER, 2);
        }
    }
    else if (criteria == 'm')
    {
        safe = checkMoveInput(input);

        if (safe)
        {
            int row, col;

            row = stripRow(input[0]);
            col = stripCol(input);

            safe = checkBounds(row, bRow, 0) && checkBounds(col, bCol, 0);
        }
    }
    else if (criteria == 'a')
    {
        safe = (input == "y" || input == "n" || input == "Y" || input == "N");
    }
    else
    {
        safe = checkName(input) && countSpaces(input);
    }

    return safe;
}

//=========================================================================
// Prompts the user to enter the total number of rows.
//=========================================================================
int getRow()
{
    string temp;
    bool safe;

    do
    {
        cout << "Enter the number of rows -> ";
        getline(cin, temp);
        cin.clear();

        safe = checkInput(temp, 'r', MAX_ROW, MAX_COL);

        if (!safe)
        {
            cout << "Invalid input! Please try again.\n\n";
        }
    } while (!safe);

    return stringToInt(temp);
}

//=========================================================================
// Prompts the user to enter the total number of columns.
//=========================================================================
int getCol()
{
    string temp;
    bool safe;

    do
    {
        cout << "Enter the number of columns -> ";
        getline(cin, temp);
        cin.clear();

        safe = checkInput(temp, 'c', MAX_ROW, MAX_COL);

        if (!safe)
        {
            cout << "Invalid input! Please try again.\n\n";
        }
    } while (!safe);

    return stringToInt(temp);
}

//=========================================================================
// Prompts the user to enter the total number of players in the game.
//=========================================================================
int getPlayers()
{
    string temp;
    bool safe;

    do
    {
        cout << "Enter the total amount of players -> ";
        getline(cin, temp);
        cin.clear();

        safe = checkInput(temp, 'p', 0, 0);

        if (!safe)
        {
            cout << "Invalid input! Please try again.\n\n";
        }
    } while (!safe);

    return stringToInt(temp);
}

//=========================================================================
// Prompts the user to enter y or n if they want to play again.
//=========================================================================
char playAgain()
{
    string temp;
    bool safe;
    char result;

    do
    {
        cout << "Would you like to play again? (y/n) -> ";
        getline(cin, temp);

        safe = checkInput(temp, 'a', 0, 0);

        if (!safe)
        {
            cout << "Invalid input! Try again.\n";
        }

    } while (!safe);

    result = temp[0];

    return result;
}
