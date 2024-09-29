#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <math.h>
#include <stack>

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
    void clear() {
        for (auto &row: grid) {
            for (char &c: row) {
                c = ' ';
            }
        }
    }
};

class Shape {
public:
    virtual void draw(vector<vector<char> > &grid) const = 0;
    virtual void print() const = 0;

    virtual ~Shape() {
    };
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
    void print() const override {
        cout << "Rectangle x: " << x << " y: " << y << " height: " << height << " width: " << width << endl;
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
    void print() const override {
        cout << "Circle x: " << x << " y: " << y << " radius: " << radius << endl;
    }
};

class Line : public Shape {
private:
    int x1, y1, x2, y2;
    bool isTriangle;

public:
    Line(int x1, int y1, int x2, int y2, bool isTriangle = false)
        : x1(x1), y1(y1), x2(x2), y2(y2), isTriangle(isTriangle) {
    }

    void draw(vector<vector<char> > &grid) const override {
        int deltaX = x2 - x1;
        int deltaY = y2 - y1;
        int steps = max(abs(deltaX), abs(deltaY));
        float xIncrement = deltaX / static_cast<float>(steps);
        float yIncrement = deltaY / static_cast<float>(steps);
        float x = x1;
        float y = y1;

        int prevX;
        int prevY;

        for (int i = 0; i <= steps; ++i) {
            int gridX = round(x);
            int gridY = round(y);

            if (isTriangle) {
                if (gridX == prevX && gridY == prevY) {
                    x += xIncrement;
                    y += yIncrement;
                    continue;
                }
            } else {
                if ((gridX == prevX && gridY == prevY) || (gridX == prevX || gridY == prevY)) {
                    x += xIncrement;
                    y += yIncrement;
                    continue;
                }
            }

            if (gridX >= 0 && gridX < BOARD_WIDTH && gridY >= 0 && gridY < BOARD_HEIGHT) {
                grid[gridY][gridX] = '*';
            }

            prevX = gridX;
            prevY = gridY;

            x += xIncrement;
            y += yIncrement;
        }
    }
    void print() const override {
        cout << "Line x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << endl;
    }
};

class Triangle : public Shape {
private:
    int x1, y1, x2, y2, x3, y3;

public:
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3)
        : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {
    }

    void draw(vector<vector<char> > &grid) const override {
        Line line1(x1, y1, x2, y2, true);
        line1.draw(grid);
        Line line2(x2, y2, x3, y3, true);
        line2.draw(grid);
        Line line3(x3, y3, x1, y1, true);
        line3.draw(grid);
    }
    void print() const override {
        cout << "Triangle x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << " x3: " << x3 << " y3: " <<
                y3 << endl;;
    }
};

class ShapeCommands {
private:
    Board board;
    map<int, shared_ptr<Shape> > shapes;
    int ID = 1;
    stack<int> shapeStack;

public:
    void listShapes() const {
        if (shapes.empty()) {
            cout << "No shapes added." << endl;
            return;
        }
        for (const auto &shape: shapes) {
            cout << "ID: " << shape.first << ", ";
            shape.second->print();
        }
    }

    void allShapes() const {
        cout
                << "Rectangle: x, y, height, width \n"
                << "Circle: x, y, radius \n"
                << "Triangle: x1, y1, x2, y2, x3, y3 \n"
                << "Line: x1, y1, x2, y2 " << endl;
    }
    void addShape(shared_ptr<Shape>shape) {
        shapes[ID] = shape;
        shapeStack.push(ID);
        ID++;
    }

    void drawBoard() {
        board.clear();
        if (!shapes.empty()) {
            for (const auto &shape : shapes) {
                shape.second->draw(board.grid);
            }
        }
        board.print();
    }

    void undoShape() {
        if(!shapeStack.empty()) {
            int lastShape = shapeStack.top();
            shapeStack.pop();
            shapes.erase(lastShape);
            ID--;
        }
        else {
            cout << "There is nothing to undo!" << endl;
        }
    }

    void clearShapes() {
        board.clear();
        shapes.clear();
        while (!shapeStack.empty()) {
            shapeStack.pop();
        }
        ID = 1;
    }
};
class ShapeParser {
private:
    ShapeCommands& shapeCommands;
public:
    ShapeParser(ShapeCommands& sc) : shapeCommands(sc) {}
    void parseAddShapes(istringstream& iss) {
        string shapeType;
        iss >> shapeType;
        if (shapeType == "rectangle") {
            int x, y, height, width;
            iss >> x >> y >> height >> width;
            shapeCommands.addShape(make_shared<Rectangle>(x, y, height, width));
        }
        else if (shapeType == "circle") {
            int x, y, radius;
            iss >> x >> y >> radius;
            shapeCommands.addShape(make_shared<Circle>(x, y, radius));
        }
        else if (shapeType == "triangle") {
            int x1, y1, x2, y2, x3, y3;
            iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
            shapeCommands.addShape(make_shared<Triangle>(x1, y1, x2, y2, x3, y3));
        }
        else if (shapeType == "line") {
            int x1, y1, x2, y2;
            iss >> x1 >> y1 >> x2 >> y2;
            shapeCommands.addShape(make_shared<Line>(x1, y1, x2, y2, false));
        }
    }

};

class CommandsExecution {
private:
    Board board;
    ShapeCommands shapeCommands;
    ShapeParser shapeParser;

public:
    CommandsExecution() : shapeParser(shapeCommands) {}

    void inputReader() {
        string input;

        while (true) {
            cout << "Enter a command: ";
            getline(cin, input);
            string command;
            istringstream iss(input);
            iss >> command;

            if (command == "draw") {
                shapeCommands.drawBoard();
            } else if (command == "list") {
                shapeCommands.listShapes();
            } else if (command == "shapes") {
                shapeCommands.allShapes();
            } else if (command == "add") {
                shapeParser.parseAddShapes(iss);
            } else if (command == "undo"){
                shapeCommands.undoShape();
            } else if (command == "clear") {
                shapeCommands.clearShapes();
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
