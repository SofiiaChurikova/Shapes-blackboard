#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <cmath>
#include <stack>
#include <fstream>
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

    virtual string getType() const = 0;

    virtual string getParams() const = 0;

    virtual bool validBorder() const = 0;


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

    string getType() const override {
        return "Rectangle";
    }

    string getParams() const override {
        return to_string(x) + " " + to_string(y) + " " + to_string(height) + " " + to_string(width);
    }

    bool validBorder() const override {
        return (x < BOARD_WIDTH && x + width > 0 &&
                y < BOARD_HEIGHT && y + height > 0);
    }
};

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

    string getType() const override {
        return "Circle";
    }

    string getParams() const override {
        return to_string(x) + " " + to_string(y) + " " + to_string(radius);
    }

    bool validBorder() const override {
        bool withinBoard = radius * 2 <= BOARD_WIDTH && radius * 2 <= BOARD_HEIGHT;
        bool partOfBoard = (x + radius >= 0 && x - radius < BOARD_WIDTH &&
                            y + radius >= 0 && y - radius < BOARD_HEIGHT);
        return withinBoard && partOfBoard;
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

    string getType() const override {
        return "Line";
    }

    string getParams() const override {
        return to_string(x1) + ' ' + to_string(y1) + ' ' + to_string(x2) + ' ' + to_string(y2);
    }

    bool validBorder() const override {
        return (x1 < BOARD_WIDTH && x1 >= 0 && y1 < BOARD_HEIGHT && y1 >= 0) ||
               (x2 < BOARD_WIDTH && x2 >= 0 && y2 < BOARD_HEIGHT && y2 >= 0);
    }
};

class Triangle : public Shape {
private:
    int x1, y1, x2, y2, x3, y3;

public:
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3)
        : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {
    }

    bool validBorder() const override {
        return (x1 < BOARD_WIDTH && x1 >= 0 && y1 < BOARD_HEIGHT && y1 >= 0) ||
               (x2 < BOARD_WIDTH && x2 >= 0 && y2 < BOARD_HEIGHT && y2 >= 0) ||
               (x3 < BOARD_WIDTH && x3 >= 0 && y3 < BOARD_HEIGHT && y3 >= 0);
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

    string getType() const override {
        return "Triangle";
    }

    string getParams() const override {
        return to_string(x1) + ' ' + to_string(y1) + ' ' + to_string(x2) + ' ' + to_string(y2) + ' ' + to_string(x3) +
               ' ' + to_string(y3);
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

    void addShape(shared_ptr<Shape> shape) {
        for (const auto &newShape: shapes) {
            if (newShape.second->getType() == shape->getType() && newShape.second->getParams() == shape->getParams()) {
                cout << "Shape " << shape->getType() << " with params " << shape->getParams() << " already exists." <<
                        endl;
                return;
            }
        }
        if (!shape->validBorder()) {
            cout << "Shape outside of the board." << endl;
            return;
        }
        shapes[ID] = shape;
        shapeStack.push(ID);
        ID++;
    }

    void drawBoard() {
        board.clear();
        if (!shapes.empty()) {
            for (const auto &shape: shapes) {
                shape.second->draw(board.grid);
            }
        }
        board.print();
    }

    void undoShape() {
        if (!shapeStack.empty()) {
            int lastShape = shapeStack.top();
            shapeStack.pop();
            shapes.erase(lastShape);
            ID--;
        } else {
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

    const map<int, shared_ptr<Shape> > &getShapes() const {
        return shapes;
    }
};

class ShapeParser {
private:
    ShapeCommands &shapeCommands;

public:
    ShapeParser(ShapeCommands &sc) : shapeCommands(sc) {
    }

    void parseAddShapes(istringstream &iss) {
        string shapeType;
        iss >> shapeType;

        string extraArg;

        if (shapeType == "rectangle" || shapeType == "Rectangle") {
            int x, y, height, width;
            if (!(iss >> x >> y >> height >> width)) {
                throw invalid_argument("Invalid number of arguments for Rectangle. Expected 4.");
            }
            if (iss >> extraArg) {
                throw invalid_argument("Too many arguments for Rectangle. Expected 4.");
            }
            shapeCommands.addShape(make_shared<Rectangle>(x, y, height, width));
        } else if (shapeType == "circle" || shapeType == "Circle") {
            int x, y, radius;
            if (!(iss >> x >> y >> radius)) {
                throw invalid_argument("Invalid number of arguments for Circle. Expected 3.");
            }
            if (iss >> extraArg) {
                throw invalid_argument("Too many arguments for Circle. Expected 3.");
            }
            shapeCommands.addShape(make_shared<Circle>(x, y, radius));
        } else if (shapeType == "line" || shapeType == "Line") {
            int x1, y1, x2, y2;
            if (!(iss >> x1 >> y1 >> x2 >> y2)) {
                throw invalid_argument("Invalid number of arguments for Line. Expected 4.");
            }
            if (iss >> extraArg) {
                throw invalid_argument("Too many arguments for Line. Expected 4.");
            }
            shapeCommands.addShape(make_shared<Line>(x1, y1, x2, y2));
        } else if (shapeType == "triangle" || shapeType == "Triangle") {
            int x1, y1, x2, y2, x3, y3;
            if (!(iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3)) {
                throw invalid_argument("Invalid number of arguments for Triangle. Expected 6.");
            }
            if (iss >> extraArg) {
                throw invalid_argument("Too many arguments for Triangle. Expected 6.");
            }
            shapeCommands.addShape(make_shared<Triangle>(x1, y1, x2, y2, x3, y3));
        } else {
            throw invalid_argument("Unknown shape.");
        }
    }
};

class FileParser {
private:
    ShapeParser shapeParser;
    ShapeCommands &shapeCommands;

public:
    FileParser(ShapeCommands &sc) : shapeCommands(sc), shapeParser(sc) {
    }

    void saveShapes(const string &filePath) {
        ofstream file(filePath);
        if (!file.is_open()) {
            cout << "Failed to open file for saving." << endl;
            return;
        }
        for (const auto &shape: shapeCommands.getShapes()) {
            int ID = shape.first;
            shared_ptr<Shape> sh = shape.second;
            file << "ID: " << ID << " Type: " << sh->getType() << ' ' << sh->getParams() << endl;
        }
        file.close();
    }

    void loadBoard(const string &filePath) {
        cout <<
                "Be careful! If there are any figures on the board, they will be cleared, even if an error occurs. Do you "
                "want to continue?" << endl;
        string answer;
        getline(cin, answer);

        if (answer != "yes") {
            cout << "Cancel the command" << endl;
            return;
        }

        shapeCommands.clearShapes();
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "Failed to open file for loading." << endl;
            return;
        }

        string line;
        bool isValid = true;
        while (getline(file, line)) {
            istringstream iss(line);
            string idText, typeText, shapeType;
            int id;

            if (!(iss >> idText >> id >> typeText >> shapeType)) {
                cout << "Invalid file format: " << line << endl;
                isValid = false;
                break;
            }
            string shapeParams;
            getline(iss, shapeParams);
            if (shapeParams.empty()) {
                cout << "Invalid shape parameters in file: " << line << endl;
                isValid = false;
                break;
            }

            try {
                istringstream shapeCommand(shapeType + " " + shapeParams);
                shapeParser.parseAddShapes(shapeCommand);
            } catch (const exception &e) {
                cout << "Error: " << e.what() << endl;
                isValid = false;
                break;
            }
        }

        if (!isValid) {
            shapeCommands.clearShapes();
        } else {
            cout << "Board loaded from " << filePath << endl;
        }

        file.close();
    }
};

class CommandsExecution {
private:
    Board board;
    ShapeCommands shapeCommands;
    ShapeParser shapeParser;
    FileParser fileParser;

public:
    CommandsExecution()
        : shapeParser(shapeCommands), fileParser(shapeCommands) {
    }

    void inputReader() {
        string input;

        while (true) {
            cout << "Enter a command: ";
            getline(cin, input);
            string command;
            istringstream iss(input);
            iss >> command;

            try {
                if (command == "draw") {
                    shapeCommands.drawBoard();
                } else if (command == "list") {
                    shapeCommands.listShapes();
                } else if (command == "shapes") {
                    shapeCommands.allShapes();
                } else if (command == "add") {
                    shapeParser.parseAddShapes(iss);
                } else if (command == "undo") {
                    shapeCommands.undoShape();
                } else if (command == "clear") {
                    shapeCommands.clearShapes();
                } else if (command == "save") {
                    string filePath;
                    iss >> filePath;
                    fileParser.saveShapes(filePath);
                } else if (command == "load") {
                    string filePath;
                    iss >> filePath;
                    fileParser.loadBoard(filePath);
                } else if (command == "stop") {
                    break;
                } else {
                    cout << "Unknown command: " << command << endl;
                }
            } catch (const invalid_argument &e) {
                cout << "Error: " << e.what() << endl;
            } catch (const exception &e) {
                cout << "Unexpected error: " << e.what() << endl;
            }
        }
    }
};

int main() {
    CommandsExecution execution;
    execution.inputReader();
    return 0;
}
