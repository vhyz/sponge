#include <sponge/SharedBuffer.h>
#include <iostream>

using namespace sponge;
using namespace std;

int main() {
    SharedMutableBuffer buffer(10, 'a');
    const SharedMutableBuffer<std::vector<char>>::ValueType* b = buffer.begin();
    cout << b->size() << endl;
    for (size_t i = 0; i < b->size(); ++i) {
        cout << BufferCast<char*>(b)[i];
    }
    cout << endl;
}