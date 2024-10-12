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

enum FillOption { FILL, FRAME };

struct Color {
    map<string, pair<string, char> > colors = {
        {"red", {"\033[31m", 'R'}},
        {"green", {"\033[32m", 'G'}},
        {"yellow", {"\033[33m", 'Y'}},
        {"blue", {"\033[34m", 'B'}},
        {"magenta", {"\033[35m", 'M'}},
        {"cyan", {"\033[36m", 'C'}},
        {"white", {"\033[37m", 'W'}}
    };

    string reset = "\033[0m";

    string getColorCode(const string &color) const {
        if (colors.find(color) != colors.end()) {
            return colors.at(color).first;
        }
        return reset;
    }

    string applyColor(const string &color, char symbol) const {
        return getColorCode(color) + symbol + reset;
    }

    char getSymbol(const string &color) const {
        for (const auto &c: colors) {
            if (c.first == color) {
                return c.second.second;
            }
        }
        return '*';
    }
};


string fillOptionType(FillOption fillOption) {
    switch (fillOption) {
        case FILL:
            return "FILL";
        case FRAME:
            return "FRAME";
        default:
            return "UNKNOWN";
    }
}


struct Board {
    vector<vector<char> > grid;
    Color colors;

    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {
    }
    string applyColorBasedOnSymbol(char symbol) {
        switch (symbol) {
            case 'R':
                return colors.applyColor("red", 'R');
            case 'G':
                return colors.applyColor("green", 'G');
            case 'B':
                return colors.applyColor("blue", 'B');
            case 'Y':
                return colors.applyColor("yellow", 'Y');
            case 'M':
                return colors.applyColor("magenta", 'M');
            case 'C':
                return colors.applyColor("cyan", 'C');
            case 'W':
                return colors.applyColor("white", 'W');
            case ' ':
                return string(1, ' ');
            default:
                    return colors.applyColor("white", symbol);
        }
    }

    void print() {
        for (auto &row : grid) {
            for (char c : row) {
                string coloredSymbol = applyColorBasedOnSymbol(c);
                cout << coloredSymbol;
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
protected:
    string color;
    char colorSymbol;
    Color colors;

public:
    string getColor() const {
        return color;
    }

    void setColor(const string &c) {
        color = c;
        colorSymbol = colors.getSymbol(c);
    }

    char getColorSymbol() const {
        return colorSymbol;
    }


    virtual void draw(vector<vector<char> > &grid) = 0;

    virtual void print() const = 0;

    virtual string getType() const = 0;

    virtual string getParams() const = 0;

    virtual bool validBorder() const = 0;

    virtual int getX() const { return 0; }
    virtual int getY() const { return 0; }

    virtual void setX(int x) {
    }

    virtual void setY(int y) {
    }

    virtual bool coordinateContains(int cx, int cy) const = 0;

    virtual ~Shape() {
    };
};


class Rectangle : public Shape {
private:
    int x, y, height, width;
    FillOption fillOption;

public:
    Rectangle(int x, int y, int height, int width, FillOption fillOption, const string &colorName)
        : x(x), y(y), height(height), width(width), fillOption(fillOption) {
        setColor(colorName);
    }

    void setX(int newX) override {
        x = newX;
    }

    void setY(int newY) override {
        y = newY;
    }

    void setHeight(int newH) {
        height = newH;
    }

    void setWidth(int newW) {
        width = newW;
    }

    void draw(vector<vector<char> > &grid) override {
        char symbol = getColorSymbol();
        for (int i = 0; i < width; ++i) {
            if (y >= 0 && y < BOARD_HEIGHT && x + i >= 0 && x + i < BOARD_WIDTH) {
                grid[y][x + i] = symbol;
            }
            if (y + height - 1 >= 0 && y + height - 1 < BOARD_HEIGHT && x + i >= 0 && x + i < BOARD_WIDTH) {
                grid[y + height - 1][x + i] = symbol;
            }
        }
        for (int j = 0; j < height; ++j) {
            if (y + j >= 0 && y + j < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                grid[y + j][x] = symbol;
            }
            if (y + j >= 0 && y + j < BOARD_HEIGHT && x + width - 1 >= 0 && x + width - 1 < BOARD_WIDTH) {
                grid[y + j][x + width - 1] = symbol;
            }
        }
        if (fillOption == FILL) {
            for (int i = 1; i <= width - 1; ++i) {
                for (int j = 1; j <= height - 1; ++j) {
                    if (y + j >= 0 && y + j < BOARD_HEIGHT && x + i >= 0 && x + i < BOARD_HEIGHT) {
                        grid[y + j][x + i] = symbol;
                    }
                }
            }
        }
    }

    void print() const override {
        cout << "Rectangle x: " << x << " y: " << y << " height: " << height << " width: " << width
                << " Fill option: " << fillOptionType(fillOption) << " Color: " << color << endl;
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

    bool coordinateContains(int cx, int cy) const override {
        if (fillOption == FILL) {
            return (cx >= x && cx <= x + width) && (cy >= y && cy <= y + height);
        } else if (fillOption == FRAME) {
            return (cx == x || cx == x + width) && (cy >= y && cy <= y + height) ||
                   (cy == y || cy == y + height) && (cx >= x && cx <= x + width);
        }
        return false;
    }
};

class Circle : public Shape {
private:
    int x, y;
    int radius;
    FillOption fillOption;

public:
    Circle(int x, int y, int radius, FillOption fillOption, const string &colorName) : x(x), y(y), radius(radius),
        fillOption(fillOption) {
        setColor(colorName);
    }

    void setX(int newX) override {
        x = newX;
    }

    void setY(int newY) override {
        y = newY;
    }

    void setRadius(int newR) {
        radius = newR;
    }


    void draw(vector<vector<char> > &grid) override {
        char symbol = getColorSymbol();
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            for (int j = 0; j < BOARD_HEIGHT; ++j) {
                double distance = sqrt(pow(i - x, 2) + pow((j - y) * 2, 2));
                if (fillOption == FRAME) {
                    if (fabs(distance - radius) < 0.5) {
                        grid[j][i] = symbol;
                    }
                } else if (fillOption == FILL) {
                    if (distance <= radius) {
                        grid[j][i] = symbol;
                    }
                }
            }
        }
    }

    void print() const override {
        cout << "Circle x: " << x << " y: " << y << " radius: " << radius << " Fill option: " <<
                fillOptionType(fillOption) << " Color: " << color << endl;
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

    bool coordinateContains(int cx, int cy) const override {
        double distance = sqrt(pow(cx - x, 2) + pow((cy - y) * 2, 2));

        if (fillOption == FILL) {
            return distance <= radius;
        } else if (fillOption == FRAME) {
            return fabs(distance - radius) < 0.5;
        }
        return false;
    }
};

class Line : public Shape {
private:
    bool change = false;
    char customSymbol;
    int x1, y1, x2, y2;
    bool isTriangle;

public:
    Line(int x1, int y1, int x2, int y2, bool isTriangle, const string &colorName)
        : x1(x1), y1(y1), x2(x2), y2(y2), isTriangle(isTriangle) {
        setColor(colorName);
        customSymbol = getColorSymbol();
    }

    bool changeSymbol() const {
        return change;
    }

    void setCustomSymbol(char symbol) {
        customSymbol = symbol;
        change = true;
    }

    int getX() const override { return x1; }
    int getY() const override { return y1; }
    void setX(int newX) override { x1 = newX; }
    void setY(int newY) override { y1 = newY; }

    int getX2() const { return x2; }
    int getY2() const { return y2; }
    void setX2(int newX2) { x2 = newX2; }
    void setY2(int newY2) { y2 = newY2; }

    void draw(vector<vector<char> > &grid) override {
        char symbol = getColorSymbol();
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
                if (changeSymbol()) {
                    grid[gridY][gridX] = customSymbol;
                } else {
                    grid[gridY][gridX] = symbol;
                }
            }


            prevX = gridX;
            prevY = gridY;

            x += xIncrement;
            y += yIncrement;
        }
    }

    void print() const override {
        cout << "Line x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << " Color: " << color << endl;
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

    bool coordinateContains(int cx, int cy) const override {
        double dx = x2 - x1;
        double dy = y2 - y1;
        double lenSquare = dx * dx + dy * dy;
        if (lenSquare == 0) {
            return (cx == x1 && cy == y1);
        }
        double t = ((cx - x1) * dx + (cy - y1) * dy) / lenSquare;
        t = max(0.0, min(1.0, t));
        double closestX = x1 + t * dx;
        double closestY = y1 + t * dy;
        double distance = sqrt(pow(cx - closestX, 2) + pow(cy - closestY, 2));
        return distance < 0.5;
    }
};

class Triangle : public Shape {
private:
    bool change = false;
    char customSymbol;
    int x1, y1, x2, y2, x3, y3;
    FillOption fillOption;

    bool edgeIntersecting(int y1, int y2, int y) const {
        return (y1 <= y && y2 > y) || (y2 <= y && y1 > y);
    }

    int intersectionX(int y1, int y2, int x1, int x2, int y) const {
        return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    }

    void scanlineAlgorithm(vector<vector<char> > &grid, char symbol) const {
        int yMin = min({y1, y2, y3});
        int yMax = max({y1, y2, y3});

        for (int y = yMin; y <= yMax; ++y) {
            vector<int> intersections;
            if (edgeIntersecting(y1, y2, y)) {
                intersections.push_back(intersectionX(y1, y2, x1, x2, y));
            }
            if (edgeIntersecting(y2, y3, y)) {
                intersections.push_back(intersectionX(y2, y3, x2, x3, y));
            }
            if (edgeIntersecting(y3, y1, y)) {
                intersections.push_back(intersectionX(y3, y1, x3, x1, y));
            }

            if (intersections.size() >= 2) {
                sort(intersections.begin(), intersections.end());
                int start = intersections[0];
                int end = intersections.back();

                for (int x = start; x <= end; ++x) {
                    if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                        grid[y][x] = symbol;
                    }
                }
            }
        }
    }


    void drawLines(vector<vector<char> > &grid, char symbol) const {
        Line line1(x1, y1, x2, y2, true, color);
        line1.setCustomSymbol(symbol);
        line1.draw(grid);
        Line line2(x2, y2, x3, y3, true, color);
        line2.setCustomSymbol(symbol);
        line2.draw(grid);
        Line line3(x3, y3, x1, y1, true, color);
        line3.setCustomSymbol(symbol);
        line3.draw(grid);
    }

    bool triangleEdges(int сx, int сy) const {
        Line edge1(x1, y1, x2, y2, false, color);
        Line edge2(x2, y2, x3, y3, false, color);
        Line edge3(x3, y3, x1, y1, false, color);

        return edge1.coordinateContains(сx, сy) ||
               edge2.coordinateContains(сx, сy) ||
               edge3.coordinateContains(сx, сy);
    }

public:
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, FillOption fillOption, const string &colorName)
        : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), fillOption(fillOption) {
        setColor(colorName);
        customSymbol = getColorSymbol();
    }

    int getX() const override { return x1; }
    int getY() const override { return y1; }
    void setX(int newX) override { x1 = newX; }
    void setY(int newY) override { y1 = newY; }

    int getX2() const { return x2; }
    int getY2() const { return y2; }
    void setX2(int newX2) { x2 = newX2; }
    void setY2(int newY2) { y2 = newY2; }

    int getX3() const { return x3; }
    int getY3() const { return y3; }
    void setX3(int newX3) { x3 = newX3; }
    void setY3(int newY3) { y3 = newY3; }

    bool changeSymbol() const {
        return change;
    }

    void setCustomSymbol(char symbol) {
        customSymbol = symbol;
        change = true;
    }

    bool validBorder() const override {
        return (x1 < BOARD_WIDTH && x1 >= 0 && y1 < BOARD_HEIGHT && y1 >= 0) ||
               (x2 < BOARD_WIDTH && x2 >= 0 && y2 < BOARD_HEIGHT && y2 >= 0) ||
               (x3 < BOARD_WIDTH && x3 >= 0 && y3 < BOARD_HEIGHT && y3 >= 0);
    }

    void draw(vector<vector<char> > &grid) override {
        char symbol;
        if (change) {
            symbol = customSymbol;
        } else {
            symbol = getColorSymbol();
        }
        drawLines(grid, symbol);

        if (fillOption == FILL) {
            scanlineAlgorithm(grid, symbol);
        }
    }

    void print() const override {
        cout << "Triangle x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << " x3: " << x3 << " y3: " <<
                y3 << " Fill option: " << fillOptionType(fillOption) << " Color: " << color << endl;
    }

    string getType() const override {
        return "Triangle";
    }

    string getParams() const override {
        return to_string(x1) + ' ' + to_string(y1) + ' ' + to_string(x2) + ' ' + to_string(y2) + ' ' + to_string(x3) +
               ' ' + to_string(y3);
    }

    bool coordinateContains(int cx, int cy) const override {
        if (fillOption == FILL) {
            double divider = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
            double lambda1 = ((y2 - y3) * (cx - x3) + (x3 - x2) * (cy - y3)) / divider;
            double lambda2 = ((y3 - y1) * (cx - x3) + (x1 - x3) * (cy - y3)) / divider;
            double lambda3 = 1 - lambda1 - lambda2;
            return lambda1 >= -0.2 && lambda2 >= -0.2 && lambda3 >= -0.2;
        } else if (fillOption == FRAME) {
            return triangleEdges(cy, cx);
        }
        return false;
    }
};


class ShapeCommands {
private:
    Board board;
    map<int, shared_ptr<Shape> > shapes;
    int ID = 1;
    stack<int> shapeStack;
    weak_ptr<Shape> select;

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

    void selectByID(int id) {
        for (const auto &shape: shapes) {
            if (shape.first == id) {
                select = shape.second;
                shape.second->print();
                return;
            }
        }
        cout << "Shape with ID " << id << " not found." << endl;
    }

    void selectByCoordinates(int cx, int cy) {
        for (const auto &shape: shapes) {
            if (shape.second->coordinateContains(cx, cy)) {
                select = shape.second;
                cout << "Shape selected: ID " << shape.first << " ";
                shape.second->print();
                return;
            }
        }
        cout << "Shape was not found at coordinates (" << cx << ", " << cy << ")" << endl;
    }

    void remove() {
        if (auto selectedShape = select.lock()) {
            for (auto &shape: shapes) {
                if (shape.second == selectedShape) {
                    int id = shape.first;
                    cout << id << " " << shape.second->getType() << " removed" << endl;
                    shapes.erase(id);
                    select.reset();
                    return;
                }
            }
        } else {
            cout << "No shape selected to remove." << endl;
        }
    }

    void paint(string color) {
        if (auto selectedShape = select.lock()) {
            for (auto &shape: shapes) {
                if (shape.second == selectedShape) {
                    shape.second->setColor(color);
                    cout << "ID: " << shape.first << " Shape: " << shape.second->getType() << " Color: " <<
                            shape.second->getColor() << endl;
                    return;
                }
            }
        } else {
            cout << "No shape selected to paint." << endl;
        }
    }

    void move(int x, int y) {
        if (auto selectedShape = select.lock()) {
            for (auto &shape: shapes) {
                if (shape.second == selectedShape) {
                    int deltaX, deltaY;

                    if (shape.second->getType() == "Line") {
                        deltaX = x - shape.second->getX();
                        deltaY = y - shape.second->getY();
                        shape.second->setX(x);
                        shape.second->setY(y);

                        Line *line = static_cast<Line *>(shape.second.get());
                        line->setX2(line->getX2() + deltaX);
                        line->setY2(line->getY2() + deltaY);
                    } else if (shape.second->getType() == "Triangle") {
                        deltaX = x - shape.second->getX();
                        deltaY = y - shape.second->getY();
                        shape.second->setX(x);
                        shape.second->setY(y);

                        Triangle *triangle = static_cast<Triangle *>(shape.second.get());
                        triangle->setX2(triangle->getX2() + deltaX);
                        triangle->setY2(triangle->getY2() + deltaY);
                        triangle->setX3(triangle->getX3() + deltaX);
                        triangle->setY3(triangle->getY3() + deltaY);
                    } else {
                        shape.second->setX(x);
                        shape.second->setY(y);
                    }

                    cout << "ID: " << shape.first << " Shape: " << shape.second->getType() << " moved." << endl;
                    return;
                }
            }
        } else {
            cout << "No shape selected to move." << endl;
        }
    }
    void edit(istringstream &iss) {
        if (auto selectedShape = select.lock()) {
            if (selectedShape->getType() == "Line") {
                char newSymbol;
                if (iss >> newSymbol) {
                    Line *line = static_cast<Line *>(selectedShape.get());
                    line->setCustomSymbol(newSymbol);
                    cout << "Line symbol changed to '" << newSymbol << "'." << endl;
                } else {
                    cout << "Please provide a valid symbol for the line!" << endl;
                }
            }
            else if (selectedShape->getType() == "Triangle") {
                char newSymbol;
                if (iss >> newSymbol) {
                    Triangle *triangle = static_cast<Triangle *>(selectedShape.get());
                    triangle->setCustomSymbol(newSymbol);
                    cout << "Triangle symbol changed to '" << newSymbol << "'." << endl;
                } else {
                    cout << "Please provide a valid symbol for the triangle!" << endl;
                }
            }
            else if (selectedShape->getType() == "Circle" || selectedShape->getType() == "Rectangle") {
                int par1, par2;
                if (iss >> par1) {
                    if (selectedShape->getType() == "Circle") {
                        Circle *circle = static_cast<Circle *>(selectedShape.get());
                        if (!(iss >> par2)) {
                            if (par1 > 0 && circle->validBorder()) {
                                circle->setRadius(par1);
                                cout << "Radius of circle changed." << endl;
                            } else {
                                cout << "Error: invalid radius or shape will go out of the board." << endl;
                            }
                        } else {
                            cout << "Error: invalid argument count for circle." << endl;
                        }
                    }
                    else if (selectedShape->getType() == "Rectangle") {
                        Rectangle *rectangle = static_cast<Rectangle *>(selectedShape.get());
                        if (iss >> par2) {
                            if (par1 > 0 && par2 > 0 && rectangle->validBorder()) {
                                rectangle->setHeight(par1);
                                rectangle->setWidth(par2);
                                cout << "Size of rectangle changed." << endl;
                            } else {
                                cout << "Error: invalid size or shape will go out of the board." << endl;
                            }
                        } else {
                            cout << "Error: invalid argument count for rectangle." << endl;
                        }
                    }
                } else {
                    cout << "Error: provide valid params for the shape." << endl;
                }
            } else {
                cout << "You cannot edit this shape!" << endl;
            }
        } else {
            cout << "No shape selected for editing." << endl;
        }
    }
};

class ShapeParser {
private:
    ShapeCommands &shapeCommands;
    FillOption fillOption;

    string toLowerCase(const string &str) {
        string result = str;
        for (char &c: result) {
            c = tolower(c);
        }
        return result;
    }

public:
    ShapeParser(ShapeCommands &sc) : shapeCommands(sc) {
    }

    void parseAddShapes(istringstream &iss) {
        string shapeType, shapeColor;
        iss >> shapeType;
        toLowerCase(shapeType);
        iss >> shapeColor;

        string extraArg;

        if (shapeType == "rectangle" || shapeType == "circle" ||
            shapeType == "triangle") {
            string fillMode;
            iss >> fillMode;

            if (fillMode == "fill") {
                fillOption = FILL;
            } else if (fillMode == "frame") {
                fillOption = FRAME;
            } else {
                throw invalid_argument("Incorrect form! Use 'fill' or 'frame'");
            }

            if (shapeType == "rectangle") {
                int x, y, height, width;
                if (!(iss >> x >> y >> height >> width)) {
                    throw invalid_argument("Invalid number of arguments for Rectangle. Expected 4.");
                }
                if (iss >> extraArg) {
                    throw invalid_argument("Too many arguments for Rectangle. Expected 4.");
                }
                shapeCommands.addShape(make_shared<Rectangle>(x, y, height, width, fillOption, shapeColor));
            } else if (shapeType == "circle") {
                int x, y, radius;
                if (!(iss >> x >> y >> radius)) {
                    throw invalid_argument("Invalid number of arguments for Circle. Expected 3.");
                }
                if (iss >> extraArg) {
                    throw invalid_argument("Too many arguments for Circle. Expected 3.");
                }
                shapeCommands.addShape(make_shared<Circle>(x, y, radius, fillOption, shapeColor));
            } else if (shapeType == "triangle") {
                int x1, y1, x2, y2, x3, y3;
                if (!(iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3)) {
                    throw invalid_argument("Invalid number of arguments for Triangle. Expected 6.");
                }
                if (iss >> extraArg) {
                    throw invalid_argument("Too many arguments for Triangle. Expected 6.");
                }
                shapeCommands.addShape(make_shared<Triangle>(x1, y1, x2, y2, x3, y3, fillOption, shapeColor));
            }
        } else if (shapeType == "line") {
            int x1, y1, x2, y2;
            if (!(iss >> x1 >> y1 >> x2 >> y2)) {
                throw invalid_argument("Invalid number of arguments for Line. Expected 4.");
            }
            if (iss >> extraArg) {
                throw invalid_argument("Too many arguments for Line. Expected 4.");
            }
            shapeCommands.addShape(make_shared<Line>(x1, y1, x2, y2, false, shapeColor));
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
                } else if (command == "select") {
                    int idOrX, y;
                    if (iss >> idOrX) {
                        if (iss >> y) {
                            shapeCommands.selectByCoordinates(idOrX, y);
                        } else {
                            shapeCommands.selectByID(idOrX);
                        }
                    } else {
                        cout << "Invalid input for select command." << endl;
                    }
                } else if (command == "remove") {
                    shapeCommands.remove();
                } else if (command == "paint") {
                    string color;
                    iss >> color;
                    shapeCommands.paint(color);
                } else if (command == "move") {
                    int x, y;
                    iss >> x >> y;
                    shapeCommands.move(x, y);
                } else if (command == "edit") {
                    shapeCommands.edit(iss);
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
