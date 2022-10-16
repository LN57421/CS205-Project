#include <iostream>
using namespace std;

template <typename string> class ArrayList {
private:
  string value[255];
  int size;

public:
  ArrayList() { size = 0; }

  void add(string t) {
    value[size] = t;
    size++;
  }
  
  void clear() {
    string value[255];
    size = 0;
  }

  string get(int index) { return value[index]; }

  void set(int index, string t) {
    if (index == 0 || index == 1) {
      cout << "Sorry, const value is not permitted to change." << endl;
      return;
    }
    value[index] = t;
  }

  int getSize() { return size; }

  bool contains(string t) { return indexOf(t) != -1; }

  bool isEmpty() { return size == 0; }

  int indexOf(string t) {
    for (int i = 0; i < size; ++i) {
      if (value[i] == t) {
        return i;
      }
    }
    return -1;
  }
};