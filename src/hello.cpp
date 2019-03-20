#include <iostream>
#include <collections/vector.h>
#include <graphics/gwindow.h>
#include <io/tokenscanner.h>
#include <io/console.h>
using namespace std;

int main() {
    Vector<int> tt;
    TokenScanner tsc(cin);
    GWindow t;
    t.setSize(2000, 2000);
    t.setWindowTitle("123");
    t.drawLine(0, 0, 500, 500);
    char a = tsc.getChar();
    std::cout << a << std::endl;
    return 0;
}
