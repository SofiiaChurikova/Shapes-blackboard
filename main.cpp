#include <iostream>
#include <vector>
#include <sstream>

using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

struct Board {
    vector<vector<char> > grid;

    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {
    }

    void print() {
        for (auto &row: grid) {
            for (char c: row) {
                cout << c;
            }
            cout << "\n";
        }
    }
};
class Triangle {
private:
    int x, y, height;
public:
    Triangle(int x, int y, int height) : x(x), y(y), height(height) {}
    void drawTriangle(vector<vector<char>>& grid) {
        if (height <= 0) return;;
        for (int i = 0; i < height; ++i) {
            int leftMost = x - i;
            int rightMost = x + i;
            int posY = y + i;
            if (posY < BOARD_HEIGHT) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH) {
                    grid[posY][leftMost] = '*';
                    if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost) {
                        grid[posY][rightMost] = '*';
                    }
                }
            }

        }
        for (int j = 0; j < 2 * height - 1; ++j) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) {
                grid[baseY][baseX] = '*';
            }
        }

    }
};

class CommandsExecution {
private:
    Board board;

public:
    void inputReader() {
        string input;

        while (true) {
            cout << "Enter a command: ";
            getline(cin, input);
            string command;
            istringstream iss(input);
            iss >> command;

            if (command == "draw") {
                board.print();
            } else if (command == "list") {
                continue;
            } else if (command == "shapes") {
                continue;
            } else if (command == "add") {
                Triangle triangle(10, 1, 5);
                triangle.drawTriangle(board.grid);
            } else if (command == "undo") {
                continue;
            } else if (command == "clear") {
                continue;
            } else if (command == "save") {
                continue;
            } else if (command == "load") {
                continue;
            } else if (command == "stop") {
                break;
            } else {
                cout << "Unknown command: " << command << endl;
            }
        }
    }
};

int main() {
    CommandsExecution execution;
    execution.inputReader();
    return 0;
}
