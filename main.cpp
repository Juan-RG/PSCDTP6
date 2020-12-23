#include <iostream>
#include <set>

using namespace std;
int main() {
    // empty set container
    multiset<int> aa;
    aa.insert(1);
    aa.insert(1);
    aa.insert(1);
    aa.insert(2);
    aa.insert(2);
    multiset<int>::iterator itr;
    //aa.erase(1);
    auto itry = aa.find(2);
    if(itry!=aa.end()){
        aa.erase(*itry);
    }
    for (itr = aa.begin(); itr != aa.end(); ++itr) {
        if(*itr == 2){
            aa.erase(itr);
        }
        //cout << *itr <<"paso\n";
    }

    multiset<int>::iterator itrt;
    for (itrt = aa.begin(); itrt != aa.end(); ++itrt) {
        cout << *itrt <<"paso\n";
    }
    cout<<"fin\n";
    return 0;
}
