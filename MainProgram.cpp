#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <stdexcept>

using namespace std;

// ============================================================================
// LAB 1: INHERITANCE AND POLYMORPHISM
// ============================================================================

class Shape {
protected:
    std::string name;

public:
    Shape(const std::string& n) : name(n) {}

    // TODO 1: Virtual Destructor
    virtual ~Shape() {}

    virtual double area() const = 0;

    virtual std::string describe() const {
        return name + " with area " + std::to_string(area());
    }

    std::string getName() const { return name; }
};

class Circle : public Shape {
private:
    double radius;

public:
    // TODO 2: Constructor
    Circle(double r) : Shape("Circle"), radius(r) {}

    // TODO 3: Override area()
    double area() const override {
        return 3.14159265358979323846 * radius * radius;
    }
};

class Rectangle : public Shape {
protected:
    double width;
    double height;

public:
    // TODO 4: Constructor
    Rectangle(double w, double h) : Shape("Rectangle"), width(w), height(h) {}

    // TODO 5: Override area()
    double area() const override {
        return width * height;
    }
};

class Square : public Rectangle {
public:
    // TODO 6: Constructor
    Square(double side) : Rectangle(side, side) {
        name = "Square";
    }
};

// TODO 7: totalArea
double totalArea(const std::vector<Shape*>& shapes) {
    double total = 0.0;
    for (const Shape* s : shapes) {
        if (s) total += s->area();
    }
    return total;
}

// TODO 8: largestShapeName
std::string largestShapeName(const std::vector<Shape*>& shapes) {
    if (shapes.empty()) return "";
    const Shape* largest = nullptr;
    double maxArea = -1.0;
    for (const Shape* s : shapes) {
        if (s && s->area() > maxArea) {
            maxArea = s->area();
            largest = s;
        }
    }
    return largest ? largest->getName() : "";
}

void main_lab1() {
    std::cout << "=== Lab 1: Inheritance and Polymorphism ===" << std::endl;
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(2.0));
    shapes.push_back(new Rectangle(3.0, 4.0));
    shapes.push_back(new Square(5.0));

    for (const Shape* s : shapes) {
        std::cout << s->describe() << std::endl;
    }

    std::cout << "Total area: " << totalArea(shapes) << std::endl;
    std::cout << "Largest:    " << largestShapeName(shapes) << std::endl;

    for (Shape* s : shapes) delete s;
    std::cout << std::endl;
}

// ============================================================================
// LAB 2: DESTRUCTORS, COPY CONSTRUCTORS, AND ASSIGNMENT
// ============================================================================

int g_constructorCount     = 0;
int g_destructorCount      = 0;
int g_copyConstructorCount = 0;
int g_assignmentCount      = 0;

int getConstructorCount()     { return g_constructorCount; }
int getDestructorCount()      { return g_destructorCount; }
int getCopyConstructorCount() { return g_copyConstructorCount; }
int getAssignmentCount()      { return g_assignmentCount; }

void resetAllCounters() {
    g_constructorCount     = 0;
    g_destructorCount      = 0;
    g_copyConstructorCount = 0;
    g_assignmentCount      = 0;
}

class DynamicBuffer {
private:
    char* m_data;
    int   m_length;

public:
    DynamicBuffer(const char* input) {
        g_constructorCount++;
        m_length = strlen(input);
        m_data = new char[m_length + 1];
        strcpy(m_data, input);
    }

    ~DynamicBuffer() {
        g_destructorCount++;
        delete[] m_data;
    }

    DynamicBuffer(const DynamicBuffer& other) {
        g_copyConstructorCount++;
        m_length = other.m_length;
        m_data = new char[m_length + 1];
        strcpy(m_data, other.m_data);
    }

    DynamicBuffer& operator=(const DynamicBuffer& other) {
        g_assignmentCount++;
        if (this == &other) {
            return *this;
        }
        delete[] m_data;
        m_length = other.m_length;
        m_data = new char[m_length + 1];
        strcpy(m_data, other.m_data);
        return *this;
    }

    const char* getData() const { return m_data; }
    int getLength() const { return m_length; }

    void setData(const char* newData) {
        delete[] m_data;
        m_length = strlen(newData);
        m_data = new char[m_length + 1];
        strcpy(m_data, newData);
    }

    void print() const {
        cout << "DynamicBuffer[\"" << m_data << "\", length=" << m_length << "]" << endl;
    }
};

DynamicBuffer createBufferCopy(DynamicBuffer buf) {
    return buf;
}

void main_lab2() {
    cout << "=== Lab 2: Destructors, Copy Constructors & Assignment ===" << endl;
    cout << "--- Part 1: Constructor & Destructor ---" << endl;
    {
        DynamicBuffer b1("Hello");
        b1.print();
        cout << "b1 created. Constructor count: " << getConstructorCount() << endl;
    }
    cout << "b1 destroyed. Destructor count: " << getDestructorCount() << endl << endl;

    resetAllCounters();
    cout << "--- Part 2: Copy Constructor ---" << endl;
    {
        DynamicBuffer original("Deep Copy Test");
        DynamicBuffer copied(original);

        cout << "Original: "; original.print();
        cout << "Copied:   "; copied.print();
        cout << "Copy constructor count: " << getCopyConstructorCount() << endl;

        original.setData("Modified Original");
        cout << "After modifying original:" << endl;
        cout << "Original: "; original.print();
        cout << "Copied:   "; copied.print();
    }
    cout << endl;

    resetAllCounters();
    cout << "--- Part 3: Assignment Operator ---" << endl;
    {
        DynamicBuffer a("Alpha");
        DynamicBuffer b("Beta");
        cout << "Before assignment:" << endl;
        cout << "a: "; a.print();
        cout << "b: "; b.print();

        b = a;
        cout << "After b = a:" << endl;
        cout << "a: "; a.print();
        cout << "b: "; b.print();
        cout << "Assignment count: " << getAssignmentCount() << endl;

        a.setData("Changed A");
        cout << "After modifying a:" << endl;
        cout << "a: "; a.print();
        cout << "b: "; b.print();
    }
    cout << endl;

    cout << "--- Part 4: Self-Assignment ---" << endl;
    {
        DynamicBuffer s("SelfTest");
        s = s;
        cout << "After self-assignment: "; s.print();
    }
    cout << endl;
}

// ============================================================================
// LAB 3: ENCAPSULATION AND INVARIANTS
// ============================================================================

class Temperature {
private:
    double celsius_;
public:
    explicit Temperature(double celsius) {
        if(celsius < -273.15) {
            throw invalid_argument("Temperature cannot be below absolute zero.");
        }
        celsius_ = celsius;
    }

    double getCelsius() const {
        return celsius_;
    }

    double getFahrenheit() const {
        return celsius_ * 9.0 / 5.0 + 32.0;
    }

    void setCelsius(double celsius) {
        if(celsius < -273.15){
            throw invalid_argument("Temperature cannot be below absolute zero.");
        }
        celsius_ = celsius;
    }
};

class BankAccount {
private:
    string owner_;
    double balance_;
public:
    BankAccount(const string& owner, double initialBalance) {
        if(owner.empty()){
            throw invalid_argument("Owner name cannot be empty.");
        }
        if(initialBalance < 0){
            throw invalid_argument("Initial balance cannot be negative.");
        }
        owner_ = owner;
        balance_ = initialBalance;
    }
    
    string getOwner() const {
        return owner_;
    }

    double getBalance() const {
        return balance_;
    }

    void deposit(double amount) {
        if(amount <= 0){
            throw invalid_argument("Deposit amount must be positive");
        }
        balance_ += amount;
    }

    void withdraw(double amount) {
        if(amount <= 0){
            throw invalid_argument("Withdrawal amount must be positive");
        }
        if(amount > balance_){
            throw runtime_error("Insufficient funds for withdrawal.");
        }
        balance_ -= amount;
    }

    void transfer(BankAccount& other, double amount) {
        this->withdraw(amount);
        other.deposit(amount);
    }
};

class Password {
private:
    string password_;

    static bool hasDigit(const string& s) {
        for(char ch : s){
            if(isdigit(ch)) return true;
        }
        return false;
    }

    static void validate(const string& pwd) {
        if(pwd.length() < 8) {
            throw invalid_argument("Password must be at least 8 characters long");
        }
        if(!hasDigit(pwd)){
            throw invalid_argument("Password must contain at least 1 digit");
        }
    }
public:
    explicit Password(const string& pwd) {
        validate(pwd);
        password_ = pwd;
    }

    void change(const string& oldPassword, const string& newPassword) {
        if(password_ != oldPassword){
            throw invalid_argument("Current password does not match.");
        }
        validate(newPassword);
        password_ = newPassword;
    }

    bool matches(const string& attempt) const {
        return password_ == attempt;
    }

    size_t getLength() const {
        return password_.length();
    }
};

void main_lab3() {
    cout << "=== Lab 3: Encapsulation and Invariants ===" << endl;
    cout << "--- Temperature ---" << endl;
    try {
        Temperature t(100.0);
        cout << "Celsius: " << t.getCelsius() << endl;
        cout << "Fahrenheit: " << t.getFahrenheit() << endl;
        t.setCelsius(-40.0);
        cout << "Updated Celsius: " << t.getCelsius() << endl;
        cout << "Updated Fahrenheit: " << t.getFahrenheit() << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    try {
        Temperature bad(-300.0);
    } catch (const invalid_argument& e) {
        cout << "Caught expected error: " << e.what() << endl;
    }
    cout << endl;

    cout << "--- BankAccount ---" << endl;
    try {
        BankAccount alice("Alice", 1000.0);
        BankAccount bob("Bob", 500.0);
        cout << alice.getOwner() << " balance: " << alice.getBalance() << endl;
        alice.deposit(200.0);
        alice.transfer(bob, 300.0);
        cout << "After transfer:" << endl;
        cout << "  Alice: " << alice.getBalance() << endl;
        cout << "  Bob:   " << bob.getBalance() << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    cout << endl;

    cout << "--- Password ---" << endl;
    try {
        Password pw("Secure99");
        cout << "Password length: " << pw.getLength() << endl;
        cout << "Matches 'wrong': " << pw.matches("wrong") << endl;
        cout << "Matches 'Secure99': " << pw.matches("Secure99") << endl;
        pw.change("Secure99", "NewPass1");
        cout << "Password changed successfully." << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    cout << endl;
}

// ============================================================================
// LAB 4: MORE DETAILS ABOUT CLASSES
// ============================================================================

class RectangleClass; 

class Point {
private:
    double x;
    double y;
public:
    Point(double x, double y) : x(x), y(y) {}

    double getX() const { return x; }   
    double getY() const { return y; }   

    void display() const { 
        std::cout << "(" << x << ", " << y << ")"; 
    }

    friend class RectangleClass;
};

class RectangleClass {
private:
    Point topLeft;
    Point bottomRight;
public:
    RectangleClass(double x1, double y1, double x2, double y2)
        : topLeft(x1, y1), bottomRight(x2, y2) {}  

    double getWidth() const { 
        return bottomRight.getX() - topLeft.getX(); 
    }  

    double getHeight() const { 
        return topLeft.getY() - bottomRight.getY(); 
    }  

    double getArea() const { 
        return getWidth() * getHeight(); 
    }  

    void display() const {
        std::cout << "TopLeft: "; topLeft.display();
        std::cout << " BottomRight: "; bottomRight.display();
    }

    friend bool isSameSize(const RectangleClass& r1, const RectangleClass& r2);
};

bool isSameSize(const RectangleClass& r1, const RectangleClass& r2) {
    return r1.getArea() == r2.getArea();  
}

class ConstDemo {
private:
    int value;
public:
    ConstDemo(int v) : value(v) {}  

    int getValue() const { return value; }  

    void doubleValue() { value *= 2; }  

    int constGetDouble() const { return value * 2; }  
};

void main_lab4() {
    std::cout << "=== Lab 4: More Details about Classes ===" << std::endl;
    Point p(3, 4);
    p.display();
    std::cout << std::endl;

    RectangleClass r1(0, 10, 5, 0);
    RectangleClass r2(0, 8, 5, 2);

    r1.display(); std::cout << std::endl;
    r2.display(); std::cout << std::endl;

    std::cout << "Same size? " << isSameSize(r1, r2) << std::endl;

    ConstDemo c(10);
    std::cout << "Value: " << c.getValue() << std::endl;
    c.doubleValue();
    std::cout << "Double via const metot: " << c.constGetDouble() << std::endl;
    std::cout << std::endl;
}

// ============================================================================
// LAB 5: DYNAMIC MEMORY MANAGEMENT & STATIC CLASSES
// ============================================================================

class IntArray {
private:
    int* data;       
    int  capacity;   
    int  count;      

public:
    IntArray(int cap);
    ~IntArray();
    IntArray(const IntArray& other);
    IntArray& operator=(const IntArray& other);
    bool add(int value);
    int get(int index) const;
    int size() const;
    int getCapacity() const;
    bool isEmpty() const;
    bool removeLast();
};

class Tracker {
private:
    static int objectCount;
    Tracker() = delete;

public:
    static void objectCreated();
    static void objectDestroyed();
    static int getActiveCount();
    static void resetCount();
};

int Tracker::objectCount = 0;

void Tracker::objectCreated() { objectCount++; }
void Tracker::objectDestroyed() { objectCount--; }
int Tracker::getActiveCount() { return objectCount; }
void Tracker::resetCount() { objectCount = 0; }

IntArray::IntArray(int cap) : capacity(cap), count(0) {
    data = new int[capacity];
    Tracker::objectCreated();
}

IntArray::~IntArray() {
    delete[] data;
    Tracker::objectDestroyed();
}

IntArray::IntArray(const IntArray& other) : capacity(other.capacity), count(other.count) {
    data = new int[capacity];
    for (int i = 0; i < count; i++) {
        data[i] = other.data[i];
    }
    Tracker::objectCreated();
}

IntArray& IntArray::operator=(const IntArray& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        count = other.count;
        data = new int[capacity];
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

bool IntArray::add(int value) {
    if (count < capacity) {
        data[count] = value;
        count++;
        return true;
    }
    return false;
}

int IntArray::get(int index) const {
    if (index >= 0 && index < count) {
        return data[index];
    }
    return -1;
}

int IntArray::size() const { return count; }

int IntArray::getCapacity() const { return capacity; }

bool IntArray::isEmpty() const { return count == 0; }

bool IntArray::removeLast() {
    if (count > 0) {
        count--;
        return true;
    }
    return false;
}

void main_lab5() {
    cout << "=== Lab 5: Dynamic Memory & Static Classes ===" << endl;
    cout << "[1] Creating IntArray with capacity 5..." << endl;
    IntArray arr(5);
    cout << "    Active objects: " << Tracker::getActiveCount() << endl;

    cout << "[2] Adding elements: 10, 20, 30" << endl;
    arr.add(10); arr.add(20); arr.add(30);
    cout << "    Size: " << arr.size() << ", Capacity: " << arr.getCapacity() << endl;

    cout << "[3] Elements: ";
    for (int i = 0; i < arr.size(); i++) { cout << arr.get(i) << " "; }
    cout << endl;

    cout << "[4] Copy constructing arr2 from arr..." << endl;
    IntArray arr2(arr);
    cout << "    Active objects: " << Tracker::getActiveCount() << endl;

    cout << "[5] Creating arr3(2), then assigning arr to arr3..." << endl;
    IntArray arr3(2);
    cout << "    Active objects: " << Tracker::getActiveCount() << endl;
    arr3 = arr;

    cout << "[6] Removing last from arr..." << endl;
    arr.removeLast();
    cout << "    arr size after removeLast: " << arr.size() << endl;

    {
        IntArray temp(3);
        cout << "    Inside scope - Active objects: " << Tracker::getActiveCount() << endl;
    }
    cout << "    After scope  - Active objects: " << Tracker::getActiveCount() << endl;
    cout << endl;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    main_lab1();
    main_lab2();
    main_lab3();
    main_lab4();
    main_lab5();
    return 0;
}
