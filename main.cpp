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
                continue;
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
