#include <iostream>
#include <set>

using namespace std;
int main() {
    // empty set container
    set<int> s1;

    // insert elements in random order
    //dsfsdfsdf
    //sdfsdfsdfs
     //sdfsdfsdfs
     //sdfsdfsdfs
      //sdfsdfsdfs
     s1.insert(1);
    s1.insert(2);
    s1.insert(3);
    s1.insert(4);
    s1.insert(5);

    s1.insert(1);
    s1.insert(2);
    s1.insert(3);
    s1.insert(4);
    s1.insert(5);
//////////
    s1.erase(2);

    // printing set s1
    set<int, greater<int> >::iterator itr;
    cout << "\nThe set s1 is : \n";
    for (itr = s1.begin();
         itr != s1.end(); ++itr)
    {
        cout << ',' << *itr;
    }
    cout << endl;

    return 0;
}
