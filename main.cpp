#include <iostream>
#include <set>

using namespace std;
int main() {
    // empty set container
    bool sigueLocal = true;
    for (int i = 0; (i < 10 && sigueLocal); ++i) {
        cout<<"kk\n";
        if(i == 2){
            sigueLocal = false;
        }
    }
    cout<<"fin\n";
    return 0;
}
