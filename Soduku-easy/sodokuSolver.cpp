using namespace std;

#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

class SodokuGame {

    int grid[9][9];

    public: SodokuGame() {

        cliMenu();

    }

    private: void cliMenu() {

        // Inital text display when program starts.

        cout << "Type 1 for command input. Type 2 for file input.";

        int option = cliGetOption();

        if (option == 1) {
            loadCliPuzzle();
        }
        if (option == 2) {
            loadFilePuzzle();
        }
    }

    private: int cliGetOption() {

        int option;
        bool valid_input = false;
        while (valid_input == false) {
            
            if (cin.fail()) {
                valid_input = false;
                cin.clear();
                cin.ignore(100, '\n');
            }

            cin >> option;

            if (option >= 1 && option <= 2) {

                return option;

            }
            else {
                // input provided is not valid
                cout << option << " is an incorrect option. Type 1 for command input. Type 2 for file input.";
            }
        }
        return 0;
    }

    private: void loadCliPuzzle() {

        // Get user to type sodoku puzzle 

        int rowIterator, colIterator;

        for (int rowIter = 1; rowIter <= 9; rowIter++) {
            for (int colIter = 1; colIter <= 9; colIter++) {
                
                int position;
                cout << "Enter position: [" << rowIter << "][" << colIter << "] of the puzzle. Type 0 for blanks.";
                
                cin >> position;
                
                if (position >= 0 && position < 9) {
                    grid[rowIter][colIter] = position;
                }
                else {
                    cout << "Incorrect value. Please enter 1 to 9 for a number, or 0 for a blank.";
                    colIter--;
                }
            }
        }
        
        showGrid();
    }

    private: void loadFilePuzzle() {

        cout << "The file which will be read will be called: InputPuzzle.txt\n";
        ifstream input_file;
        input_file.open("InputPuzzle.txt", ios::in);

        for (int rowIter = 0; rowIter < 9; rowIter++) {

            string readLine;
            input_file >> readLine;

            for (int colIter = 0; colIter < 9; colIter++) {
                
                int readValue = std::stoi(readLine.substr(colIter, 1));
                

                if (!(readValue >= 0 || readValue <= 9)) {
                    cout << "Value for cell [" << (rowIter * 9) + colIter + 1 << "] is incorrect. Please fix.";
                    exit(EXIT_FAILURE);
                }

                grid[rowIter][colIter] = readValue;

                
            }

            if (input_file.eof()) {
                    break;

            }
        }

        input_file.close();
        cout << endl;

        showGrid();

    }

    public: void setCellValue(int i, int j, int cellValue) {
        grid[i][j] = cellValue;
    }

    public: int getCellValue(int i, int j) {
        return grid[i][j];
    }

    public: void showGrid() {

        cout << "\n";
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                cout << grid[i][j];
            }
            cout << "\n";
        }

    }

    public: bool puzzleSolved() {

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {

                if (grid[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    }
};

class SodokuSolver {

    SodokuGame puzzle;

    public: SodokuSolver() {
        cout << "Lets try solve this fucking thing...\n";
        solve();
    }

    private: void solve() {
        int valueToCheck = 1;
        while (!puzzle.puzzleSolved()) {
            int cellChanges = 0;
            for (int i = 0; i < 9; i++) {
            std::vector <int> possibleValues;
            int insertionCol;

                for (int j = 0; j < 9; j++) {
                        
                    if (puzzle.getCellValue(i, j) != 0) {
                        continue;
                    }
                    else {

                        if (rowValidifier(i, valueToCheck)) {
                            if (colValidifier(j, valueToCheck) && subgridValidifier(i, j, valueToCheck)) {
                                
                                cout << valueToCheck << " is a valid value for cell [" << i + 1 << "][" << j + 1 << "].\n";
                                possibleValues.push_back(valueToCheck);
                                insertionCol = j;

                            }
                        }
                        else {
                            i++;
                            continue;
                        }
                    }
                }

                if (possibleValues.size() == 1) {
                    cout << "Looks like " << valueToCheck << " has to go in [" << i + 1 << "][" << insertionCol + 1 << "].\n";
                    cout << "DEBUG - i: " << i << ", insertionCol: " << insertionCol << ", valueToCheck: " << valueToCheck;
                    puzzle.setCellValue(i, insertionCol, valueToCheck);
                    puzzle.showGrid();
                    cellChanges++;
                    i++;
                }
                else {
                    cout << "Seems like there are " << possibleValues.size() << " cells which can store a " << valueToCheck << " in row " << i + 1 << ".\n";
                }
            }
            cout << "After checking each cell for whether a " << valueToCheck << " was absolute, " << cellChanges << " cell changes were made.";
            puzzle.showGrid();

            if (valueToCheck < 9) {
                valueToCheck++;
            }
            else {
                valueToCheck = 1;
            }
        }

        //Go to the first cell, check if there is a value already entered. If so, skip to next
        //This needs to call 3 functions:
        //rowValidifier
        //colValidifier
        //subgridValidifier


    }

    private: bool rowValidifier(int rowValue, int valueToCheck) {

        //Scan along a row until 0 is found. 
        //Scan along row to eliminate numbers already wntered on that row.
        //make a list of possibilities until there is only one possible value for that cell

        for (int j = 0; j < 9; j++) {
            if (puzzle.getCellValue(rowValue, j) == valueToCheck) {
                cout << "Looks like " << valueToCheck << " is already in row [" << rowValue << "].\n";
                return false;
            }
        }
        return true;

    }

    private: bool colValidifier(int colValue, int valueToCheck) {
        
        for (int i = 0; i < 9; i++) {
            if (puzzle.getCellValue(i, colValue) == valueToCheck) {
                cout << "Looks like " << valueToCheck << " is already in column [" << colValue << "].\n";
                return false;
            }
        }
        return true;
    }

    private: bool subgridValidifier(int cellRow, int cellCol, int valueToCheck) {
        
        int subStartPoint[3] = {0, 3, 6};
        int subEndPoint[3] = {2, 5, 8};
        int startRow = cellRow / 3;
        int startCol = cellCol / 3;

        for (int i = subStartPoint[startRow]; i <= subEndPoint[startRow]; i++) {
            for (int j = subStartPoint[startCol]; j <= subEndPoint[startCol]; j++) {

                if (puzzle.getCellValue(i, j) == valueToCheck) {
                    cout << "Looks like " << valueToCheck << " is already in subgrid [" << startRow + 1 << "][" << startCol + 1 << "].\n";
                    return false;
                }
            }
        }
        return true; 
    }

};

int main() {

    // SodokuGame sg;
    SodokuSolver ss;
    return 0;

}