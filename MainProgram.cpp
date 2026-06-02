// ============================================================
// Lab: Dynamic Memory Management & Static Classes
// Course: Object-Oriented Programming for Engineers
// Level: 2nd Year Engineering
// Duration: 50 minutes
// ============================================================

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class IntArray {
private:
    int* data;
    int capacity;
    int count;

public:
    IntArray(int cap) {
        capacity = cap;
        count = 0;
        data = new int[capacity];
        Tracker::objectCreated();
    }

    ~IntArray() {
        delete[] data;
        Tracker::objectDestroyed();
    }

    IntArray(const IntArray& other) {
        capacity = other.capacity;
        count = other.count;
        data = new int[capacity];

        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }

        Tracker::objectCreated();
    }

    IntArray& operator=(const IntArray& other) {
        if (this == &other) return *this;

        delete[] data;

        capacity = other.capacity;
        count = other.count;
        data = new int[capacity];

        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }

        return *this;
    }

    bool add(int value) {
        if (count < capacity) {
            data[count++] = value;
            return true;
        }
        return false;
    }

    int get(int index) const {
        if (index >= 0 && index < count) return data[index];
        return -1;
    }

    int size() const {
        return count;
    }

    int getCapacity() const {
        return capacity;
    }

    bool isEmpty() const {
        return count == 0;
    }

    bool removeLast() {
        if (count > 0) {
            count--;
            return true;
        }
        return false;
    }
};

class Tracker {
private:
    static int objectCount;

public:
    Tracker() = delete;

    static void objectCreated() {
        objectCount++;
    }

    static void objectDestroyed() {
        objectCount--;
    }

    static int getActiveCount() {
        return objectCount;
    }

    static void resetCount() {
        objectCount = 0;
    }
};

int Tracker::objectCount = 0;

int main() {
    cout << "=== Dynamic Memory & Static Classes Lab ===" << endl;
    cout << endl;

    cout << "[1] Creating IntArray with capacity 5..." << endl;
    IntArray arr(5);
    cout << "    Active objects: " << Tracker::getActiveCount() << endl;

    cout << "[2] Adding elements: 10, 20, 30" << endl;
    arr.add(10);
    arr.add(20);
    arr.add(30);

    cout << "    Size: " << arr.size()
         << ", Capacity: " << arr.getCapacity() << endl;

    cout << "[3] Elements: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr.get(i) << " ";
    }
    cout << endl;

    cout << "[4] Copy constructing arr2 from arr..." << endl;
    IntArray arr2(arr);
    cout << "    Active objects: " << Tracker::getActiveCount() << endl;
    cout << "    arr2 size: " << arr2.size() << endl;

    cout << "[5] Creating arr3(2), then assigning arr to arr3..." << endl;
    IntArray arr3(2);
    arr3 = arr;
    cout << "    arr3 size after assignment: " << arr3.size() << endl;

    cout << "[6] Removing last from arr..." << endl;
    arr.removeLast();
    cout << "    arr size after removeLast: " << arr.size() << endl;

    cout << "[7] Testing scope-based destruction..." << endl;
    {
        IntArray temp(3);
        temp.add(99);
        cout << "    Inside scope - Active objects: "
             << Tracker::getActiveCount() << endl;
    }
    cout << "    After scope  - Active objects: "
         << Tracker::getActiveCount() << endl;

    cout << endl;
    cout << "=== Lab Complete ===" << endl;

    return 0;
}
