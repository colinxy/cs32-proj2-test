// testMap.cpp


#include "Map.h"
#include <iostream>
#include <cassert>
#include <map>
#include <cstdlib>
#include <ctime>
#include <functional>

using namespace std;


/*********************************
 *                               *
 *        Smallberg's Test       *
 *                               *
 *********************************/

// make sure you have include guards
#include "Map.h"
#include <type_traits>

#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }

static_assert(std::is_default_constructible<Map>::value,
        "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
        "Map must be copy-constructible.");


void ThisFunctionWillNeverBeCalled() {
    CHECKTYPE(&Map::operator=,      Map& (Map::*)(const Map&));
    CHECKTYPE(&Map::empty,          bool (Map::*)() const);
    CHECKTYPE(&Map::size,           int  (Map::*)() const);
    CHECKTYPE(&Map::insert,         bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::update,         bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::erase,          bool (Map::*)(const KeyType&));
    CHECKTYPE(&Map::contains,       bool (Map::*)(const KeyType&) const);
    CHECKTYPE(&Map::get,            bool (Map::*)(const KeyType&, ValueType&) const);
    CHECKTYPE(&Map::get,            bool (Map::*)(int, KeyType&, ValueType&) const);
    CHECKTYPE(&Map::swap,           void (Map::*)(Map&));

    CHECKTYPE(combine,  bool (*)(const Map&, const Map&, Map&));
    CHECKTYPE(subtract, void (*)(const Map&, const Map&, Map&));
}

void test() {
    Map m;
    // For an empty map:
    assert(m.size() == 0);      // test size
    assert(m.empty());          // test empty
    assert(!m.erase("Ricky"));  // nothing to erase
    assert(m.insert("Fred", 123));
    assert(m.insert("Ethel", 456));
    assert(m.size() == 2);
    double d = 42;
    assert(m.get("Fred", d)  &&  d == 123);
    d = 42;
    string s1;
    assert(m.get(0, s1, d)  &&
           ((s1 == "Fred"  &&  d == 123)  ||  (s1 == "Ethel"  &&  d == 456)));
    string s2;
    assert(m.get(1, s2, d)  &&  s1 != s2  &&
           ((s2 == "Fred"  &&  d == 123)  ||  (s2 == "Ethel"  &&  d == 456)));
}

/*********************************
 *                               *
 *    End of Smallberg's Test    *
 *                               *
 *********************************/


void my_test() {
    // default constructor
    Map m;

    /*
     * empty map
     */
    // Map::size
    assert(m.size() == 0);
    // Map::empty
    assert(m.empty());
    // Map::update
    assert(!m.update("bullshit", 1.234));
    // Map::erase
    assert(!m.erase("bullshit"));
    // Map::contains
    assert(!m.contains("bullshit"));
    double d = 10.1;
    string s = "bullshit";
    // Map::get
    assert(!m.get(s, d) && s == "bullshit" && d == 10.1);
    assert(!m.get(0, s, d) && s == "bullshit" && d == 10.1);
    assert(!m.get(-1, s, d) && s == "bullshit" && d == 10.1);

    /*
     * non-empty map
     */
    // Map::insert
    assert(m.insert("bullshit", 12.345));
    assert(!m.insert("bullshit", 1234.5));
    // Map::size
    assert(m.size() == 1);
    // Map::empty
    assert(!m.empty());
    // Map::contains
    assert(m.contains("bullshit"));
    // Map::update
    assert(m.update("bullshit", 1234.5));
    // Map::size
    assert(m.size() == 1);
    // Map::get
    d = 12.345;   // double
    assert(m.get("bullshit", d) && d == 1234.5);

    assert(m.insert("angry bird", 98.765));
    assert(m.size() == 2);
    d = 0;
    assert(m.get("angry bird", d) && d == 98.765);
    s = "string";
    d = 0.01;
    // current in map:
    // "bullshit" => 1234.5
    // "angry bird" => 98.765
    assert(!m.get(-1, s, d) && s == "string" && d == 0.01);
    assert(m.get(0, s, d) &&
           ((s == "bullshit" && d == 1234.5) ||
            (s == "angry bird" && d == 98.765)));
    s = "string";
    d = 0.01;
    assert(m.get(1, s, d) &&
           ((s == "bullshit" && d == 1234.5) ||
            (s == "angry bird" && d == 98.765)));

    assert(m.insertOrUpdate("cut the rope", 975.31));
    d = 1.2;
    assert(m.get("cut the rope", d) && d == 975.31);
    assert(m.size() == 3);
    assert(m.contains("cut the rope"));

    // Map::erase
    assert(m.erase("angry bird"));
    assert(m.size() == 2);
    // Map::update
    assert(!m.update("angry bird", 98.7));
    assert(m.size() == 2);

    /*
     * copy constructor
     */
    Map other(m);
    assert(other.size() == 2);
    // currently in map
    // "bullshit" => 1234.5
    // "cut the rope" => 975.31
    s = "string";
    d = 0.01;
    assert(!other.get(-1, s, d) && s == "string" && d == 0.01);
    assert(other.get(0, s, d) &&
           ((s == "bullshit" && d == 1234.5) ||
            (s == "cut the rope" && d == 975.31)));
    s = "string";
    d = 0.01;
    assert(other.get(1, s, d) &&
           ((s == "bullshit" && d == 1234.5) ||
            (s == "cut the rope" && d == 975.31)));

    /*
     * assignment operator
     */
    Map assigned;
    assigned = m;
    assert(other.size() == 2);
    // currently in map
    // "bullshit" => 1234.5
    // "cut the rope" => 975.31
    s = "string";
    d = 0.01;
    assert(!assigned.get(-1, s, d) && s == "string" && d == 0.01);
    assert(assigned.get(0, s, d) &&
           ((s == "bullshit" && d == 1234.5) ||
            (s == "cut the rope" && d == 975.31)));
    s = "string";
    d = 0.01;
    assert(assigned.get(1, s, d) &&
           ((s == "bullshit" && d == 1234.5) ||
            (s == "cut the rope" && d == 975.31)));

    /*
     * Map::swap
     */
    Map newMap;
    assert(newMap.insert("a", 1));
    assert(newMap.insert("b", 2));
    assert(newMap.insert("c", 3));
    assert(newMap.size() == 3);

    Map newMapAnother;
    assert(newMapAnother.insert("z", 26));
    assert(newMapAnother.insert("y", 25));
    assert(newMapAnother.size() == 2);

    newMap.swap(newMapAnother);
    assert(newMap.size() == 2);
    assert(newMapAnother.size() == 3);

    d = 0.01;
    assert(newMapAnother.get("a", d) && d == 1);
    d = 0.01;
    assert(newMapAnother.get("b", d) && d == 2);
    d = 0.01;
    assert(newMapAnother.get("c", d) && d == 3);
    d = 0.01;
    assert(!newMapAnother.get("z", d) && d == 0.01);
    d = 0.01;
    assert(!newMapAnother.get("y", d) && d == 0.01);

    d = 0.01;
    assert(newMap.get("z", d) && d == 26);
    d = 0.01;
    assert(newMap.get("y", d) && d == 25);
    d = 0.01;
    assert(!newMap.get("a", d) && d == 0.01);
    d = 0.01;
    assert(!newMap.get("b", d) && d == 0.01);
    d = 0.01;
    assert(!newMap.get("c", d) && d == 0.01);

    newMapAnother.swap(newMap);
    assert(newMap.size() == 3);
    assert(newMapAnother.size() == 2);
}


void test_non_member() {
    /*
     * combine
     */
    // no conflict in keys
    Map m1;
    m1.insert("Fred", 123);
    m1.insert("Ethel", 345);
    m1.insert("Lucy", 789);
    Map m2;
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);

    Map result;
    result.insert("useless", 86420.13579);
    assert(combine(m1, m2, result));

    assert(result.size() == 4);
    double d = 0.01;
    assert(!result.get("useless", d) && d == 0.01);
    d = 0.01;
    assert(result.get("Fred", d) && d == 123);
    d = 0.01;
    assert(result.get("Ricky", d) && d == 321);
    d = 0.01;
    assert(result.get("Lucy", d) && d == 789);
    d = 0.01;
    assert(result.get("Ethel", d) && d == 345);

    // conflict in keys
    m1 = Map();
    m1.insert("Fred", 123);
    m1.insert("Ethel", 345);
    m1.insert("Lucy", 789);
    m2 = Map();
    m2.insert("Lucy", 654);
    m2.insert("Ricky", 321);

    result = Map();
    result.insert("useless", 86420.13579);
    assert(!combine(m1, m2, result));

    assert(result.size() == 3);
    d = 0.01;
    assert(!result.get("useless", d) && d == 0.01);
    d = 0.01;
    assert(result.get("Fred", d) && d == 123);
    d = 0.01;
    assert(result.get("Ricky", d) && d == 321);
    d = 0.01;
    assert(!result.get("Lucy", d) && d == 0.01);
    d = 0.01;
    assert(result.get("Ethel", d) && d == 345);

    /*
     * aliasing
     */
    // aliasing for m1
    m1 = Map();
    m1.insert("Fred", 123);
    m1.insert("Ethel", 345);
    m1.insert("Lucy", 789);
    m2 = Map();
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);

    assert(combine(m1, m2, m1));
    assert(m1.size() == 4);
    d = 0.01;
    assert(!m1.get("useless", d) && d == 0.01);
    d = 0.01;
    assert(m1.get("Fred", d) && d == 123);
    d = 0.01;
    assert(m1.get("Ricky", d) && d == 321);
    d = 0.01;
    assert(m1.get("Lucy", d) && d == 789);
    d = 0.01;
    assert(m1.get("Ethel", d) && d == 345);

    // aliasing for m2
    m1 = Map();
    m1.insert("Fred", 123);
    m1.insert("Ethel", 345);
    m1.insert("Lucy", 789);
    m2 = Map();
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);

    assert(combine(m1, m2, m2));
    assert(m2.size() == 4);
    d = 0.01;
    assert(!m2.get("useless", d) && d == 0.01);
    d = 0.01;
    assert(m2.get("Fred", d) && d == 123);
    d = 0.01;
    assert(m2.get("Ricky", d) && d == 321);
    d = 0.01;
    assert(m2.get("Lucy", d) && d == 789);
    d = 0.01;
    assert(m2.get("Ethel", d) && d == 345);

    /*
     * subtract
     */
    m1 = Map();
    m1.insert("Fred", 123);
    m1.insert("Ethel", 345);
    m1.insert("Lucy", 789);
    m2 = Map();
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);
    m2.insert("Ethel", 654);

    result = Map();
    result.insert("useless", 86420.13579);
    subtract(m1, m2, result);
    assert(result.size() == 1);
    d = 0.01;
    assert(!result.get("useless", d) && d == 0.01);
    d = 0.01;
    assert(result.get("Fred", d) && d == 123);
    d = 0.01;
    assert(!result.get("Ricky", d) && d == 0.01);
    d = 0.01;
    assert(!result.get("Lucy", d) && d == 0.01);
    d = 0.01;
    assert(!result.get("Ethel", d) && d == 0.01);

    /*
     * aliasing
     */
    // aliasing for m1
    m1 = Map();
    m1.insert("Fred", 123);
    m1.insert("Ethel", 345);
    m1.insert("Lucy", 789);
    m2 = Map();
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);
    m2.insert("Ethel", 654);

    subtract(m1, m2, m1);
    assert(m1.size() == 1);
    d = 0.01;
    assert(!m1.get("useless", d) && d == 0.01);
    d = 0.01;
    assert(m1.get("Fred", d) && d == 123);
    d = 0.01;
    assert(!m1.get("Ricky", d) && d == 0.01);
    d = 0.01;
    assert(!m1.get("Lucy", d) && d == 0.01);
    d = 0.01;
    assert(!m1.get("Ethel", d) && d == 0.01);

    // aliasing for m2
    m1 = Map();
    m1.insert("Fred", 123);
    m1.insert("Ethel", 345);
    m1.insert("Lucy", 789);
    m2 = Map();
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);
    m2.insert("Ethel", 654);

    subtract(m1, m2, m2);
    assert(m2.size() == 1);
    d = 0.01;
    assert(!m2.get("useless", d) && d == 0.01);
    d = 0.01;
    assert(m2.get("Fred", d) && d == 123);
    d = 0.01;
    assert(!m2.get("Ricky", d) && d == 0.01);
    d = 0.01;
    assert(!m2.get("Lucy", d) && d == 0.01);
    d = 0.01;
    assert(!m2.get("Ethel", d) && d == 0.01);
}


/*
 * Thanks to Carl from stackoverflow
 * http://stackoverflow.com/a/12468109/5478848
 */
std::string rand_string( size_t length ) {
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}


// test against STL
void lib_test() {
    srand(time(nullptr));

    map<string, double> map_lib;
    Map map;

    const int SIZE = 100;

    string rand_strs[SIZE];
    for (int i = 0; i < SIZE; ++i)
        rand_strs[i] = rand_string(rand() % 30 + 1);

    for (int i = 0; i < SIZE; ++i) {
        string s = rand_strs[i];
        double d = rand() % 1000;

        // does not insert when the key is present
        map_lib.insert(pair<string, double>(s, d));
        // equivalent to STL insert
        map.insert(s, d);
        assert(map_lib.size() == (size_t)map.size());
    }

    int size_actual = map_lib.size();

    // Map::get
    for (int i = 0; i < size_actual; ++i) {
        string s;
        double d;

        assert(map.get(i, s, d));
        assert(map_lib.at(s) == d);
    }

    // Map::erase
    assert(map.size() == size_actual);
    for (auto it = map_lib.begin(); it != map_lib.end(); ) {
        string s = it->first;
        double d = it->second;
        double d_map = -100;
        assert(map.get(s, d_map) && d == d_map);

        assert(map.erase(s));
        it = map_lib.erase(it);
        assert(map_lib.size() == (size_t)map.size());
    }
    assert(map.size() == 0);
}


int main() {
    test();

    my_test();
    test_non_member();
    lib_test();

    cout << "Passed all tests" << endl;

    return 0;
}
