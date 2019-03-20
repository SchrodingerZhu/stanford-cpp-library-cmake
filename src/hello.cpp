#include <iostream>
#include <collections/vector.h>
#include <graphics/gwindow.h>
#include <io/tokenscanner.h>
#include <io/console.h>
#include <collections/set.h>
using namespace std;

int main() {
    Vector<int> tt;
    TokenScanner tsc(cin);
    GWindow t;
    Set<int> mm {};
    t.setSize(2000, 2000);
    t.setWindowTitle("123");
    t.drawLine(0, 0, 500, 800);
    int a = tsc.getChar();
    mm.insert(123);
    for(auto i : mm) {
        cout << i << std::endl;
    }
    std::cout << a << std::endl;
    return 0;
}
