#include <iostream>
#include "specialString.h"
using namespace std;
const string ENDSTRING="dude";
const int CHAR_NUMBER=4;

int main()
{
    char speChar[CHAR_NUMBER];
    speChar[0]=first_C;
    speChar[1]=second_C;
    speChar[2]=third_C;
    speChar[3]=fourth_C;
    for(int i=0;i<sizeof(myWord)/sizeof(char);i++)
        cout << myWord[i];
    string strSpecial(speChar,4);
    cout<<strSpecial<<ENDSTRING<< endl;
    return 0;
}
