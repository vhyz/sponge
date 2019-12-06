#include <sponge/SharedBuffer.h>
#include <iostream>
#include <string>

using namespace sponge;
using namespace std;

void f(SharedMutableBuffer b) {
    cout << b.size() << endl;
    for (size_t i = 0; i < b.size(); ++i) {
        cout << BufferCast<char*>(&b)[i];
    }
    cout << endl;
}

void g(SharedConstBuffer b) {
    cout << b.size() << endl;
    for (size_t i = 0; i < b.size(); ++i) {
        cout << BufferCast<const char*>(&b)[i];
    }
    cout << endl;
}

int main() {
    vector<char> vec(10,'b');
    SharedMutableBuffer b(std::make_shared<vector<char>>(10, 'a'));
    cout << b.size() << endl;
    for (size_t i = 0; i < b.size(); ++i) {
        cout << BufferCast<char*>(&b)[i];
    }
    cout << endl;
    b.setDataPtr(std::make_shared<vector<char>>(std::move(vec)));
    f(b);
    g(b);
    auto c = b;
    f(c);
    c = b;
    f(c);
}