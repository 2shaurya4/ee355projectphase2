
#include "misc.h"

void printMe(string type){
    if (type == "banner"){
        char banner[] = \
"   ######## ########   #######        ##    ###    ##    ##         ########   #######   #######  ##    ## \n \
     ##    ##     ## ##     ##       ##   ## ##   ###   ##         ##     ## ##     ## ##     ## ##   ## \n \
     ##    ##     ## ##     ##       ##  ##   ##  ####  ##         ##     ## ##     ## ##     ## ##  ## \n \
     ##    ########  ##     ##       ## ##     ## ## ## ## ####### ########  ##     ## ##     ## ##### \n \
     ##    ##   ##   ##     ## ##    ## ######### ##  ####         ##     ## ##     ## ##     ## ##  ## \n \
     ##    ##    ##  ##     ## ##    ## ##     ## ##   ###         ##     ## ##     ## ##     ## ##   ## \n \
     ##    ##     ##  #######   ######  ##     ## ##    ##         ########   #######   #######  ##    ##\n";
        cout << endl << banner << endl << endl;
    }
}

string codeName(string str1, string str2) {
    string result = str1 + str2;
    string noSpaces = "";
    for (string::size_type i = 0; i < result.length(); i++) {
        if (result[i] != ' ') {
            noSpaces += result[i];
        }
    }
    for (string::size_type i = 0; i < noSpaces.length(); i++) {
        if (noSpaces[i] >= 'A' && noSpaces[i] <= 'Z') {
            noSpaces[i] = noSpaces[i] + 32;
        }
    }
    return noSpaces;
}
