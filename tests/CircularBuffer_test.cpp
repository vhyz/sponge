#include"../SpongeBob/CircularBuffer.h"
#include <iostream>

using namespace std;

class test {
   public:
    test() { cout << "test" << endl; }
    test(const test&) { cout << "copy" << endl; }
    test(test&&) { cout << "move" << endl; }
    ~test() { cout << "~test" << endl; }
};

int main() {
    CircularBuffer<test> buffer(5);
    {
        test t;
        for (int i = 0; i < 10; ++i) {
            cout << buffer.size() << endl;
            cout << buffer.capacity() << endl;
            buffer.push_back(t);
        }
    }
    cout << "?" << endl;
}