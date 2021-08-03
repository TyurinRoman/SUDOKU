#include <vector>
using namespace std;


//funkce solve, kterou jsem aplikoval, resi sudoku
// inspiroval jsem se na webove strance  https://www.tutorialcup.com/interview/matrix/sudoku-solver.htm


bool isSafe(vector<vector<int>> &sudoku, int i, int j, int n) {
    // Check in current row and column
    for (int x = 0; x < 9; x++) {
        // Row
        if (sudoku[i][x] == n) {
            return false;
        }
        // Column
        if (sudoku[x][j] == n) {
            return false;
        }
    }

    // Calculate the starting index of row and column of current 3x3 sub box
    int rs = i - (i % 3);
    int cs = j - (j % 3);
    // Check in the current sub box
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (sudoku[x + rs][y + cs] == n) {
                return false;
            }
        }
    }
    // Return true
    return true;
}
bool solve(vector<vector<int>> &sudoku) {
    int i = 0, j = 0;
    bool found = false;
    // Find an empty cell
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (sudoku[i][j] == 0) {
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // No empty cell found, return true
    if (i == 9 && j == 9) {
        return true;
    }

    // One by one try all the values in the current cell
    for (int n = 1; n <= 9; n++) {
        // check if it is valid to assign value n to current cell
        if (isSafe(sudoku, i, j, n)) {
            sudoku[i][j] = n;
            // Recursively solve the sudoku
            if (solve(sudoku) == true)
                return true;
            // back track if the recursion returns false
            sudoku[i][j] = 0;
        }
    }

    // Return false if no value fits
    return false;
}
