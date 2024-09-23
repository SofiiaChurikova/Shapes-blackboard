#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>

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

class Shape {
public:
    virtual void draw(vector<vector<char> > &grid) const = 0;

    virtual ~Shape() {
    };
};

class Triangle : public Shape {
private:
    int x, y, height;

public:
    Triangle(int x, int y, int height) : x(x), y(y), height(height) {
    }

    void draw(vector<vector<char> > &grid) const override {
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

class Rectangle : public Shape {
private:
    int x, y, height, width;

public:
    Rectangle(int x, int y, int height, int width)
        : x(x), y(y), height(height), width(width) {
    }

    void draw(vector<vector<char> > &grid) const override {
        for (int i = 0; i < width; ++i) {
            if (y >= 0 && y < BOARD_HEIGHT && x + i >= 0 && x + i < BOARD_WIDTH) {
                grid[y][x + i] = '*';
            }
            if (y + height - 1 >= 0 && y + height - 1 < BOARD_HEIGHT && x + i >= 0 && x + i < BOARD_WIDTH) {
                grid[y + height - 1][x + i] = '*';
            }
        }
        for (int j = 0; j < height; ++j) {
            if (y + j >= 0 && y + j < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                grid[y + j][x] = '*';
            }
            if (y + j >= 0 && y + j < BOARD_HEIGHT && x + width - 1 >= 0 && x + width - 1 < BOARD_WIDTH) {
                grid[y + j][x + width - 1] = '*';
            }
        }
    }
};

// need to implement area restriction
class Circle : public Shape {
private:
    int x, y;
    int radius;

public:
    Circle(int x, int y, int radius) : x(x), y(y), radius(radius) {
    }

    void draw(vector<vector<char> > &grid) const override {
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            for (int j = 0; j < BOARD_HEIGHT; ++j) {
                double distance = sqrt(pow(i - x, 2) + pow(j - y, 2));
                if (fabs(distance - radius) < 0.5) {
                    grid[j][i] = '*';
                }
            }
        }
    }
};

class Line : public Shape {
private:
    int x1, y1, x2, y2;

public:
    Line(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {
    }

    void draw(vector<vector<char> > &grid) const override {
        // i'll make it with slope
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
                // Triangle triangle(10, 1, 5);
                // triangle.draw(board.grid);
                // Rectangle rectangle(3, 2, 5, 6);
                // rectangle.draw(board.grid);
                Circle circle(3, 2, 4);
                circle.draw(board.grid);
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
