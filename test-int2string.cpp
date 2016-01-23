#include "Map.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;


/*********************************
 *                               *
 *        Smallberg's Test       *
 *                               *
 *********************************/

void test() {
    Map m;
    assert(m.insert(123, "Fred"));
    assert(m.insert(456, "Ethel"));
    assert(m.size() == 2);
    string s;
    assert(m.get(123, s)  &&  s == "Fred");
    s = "";
    int i1;
    assert(m.get(0, i1, s)  &&
           ((i1 == 123  &&  s == "Fred")  ||  (i1 == 456  &&  s == "Ethel")));
    int i2;
    assert(m.get(1, i2, s)  &&  i1 != i2  &&
           ((i2 == 123  &&  s == "Fred")  ||  (i2 == 456  &&  s == "Ethel")));
}

int main() {
    test();
    cout << "Passed all tests" << endl;
}

/*********************************
 *                               *
 *    End of Smallberg's Test    *
 *                               *
 *********************************/
