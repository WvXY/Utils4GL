#include <array>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
//    char board[6][8] = {"."};
    

    for(int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}