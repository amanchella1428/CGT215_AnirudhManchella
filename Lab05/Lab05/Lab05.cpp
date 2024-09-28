#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Setting the values of each in the cipher
char settingCipher(char ch, const vector<char>& cipher) {
    //uppercase code
    if (ch >= 'A' && ch <= 'Z') {
        return cipher[ch - 'A'];
    }
    //lowercase code, and then make it back to uppercase
    else if (ch >= 'a' && ch <= 'z') {
        char upperCaseChar = ch - 32;
        return cipher[upperCaseChar - 'A'] + 32;
    }
    //if not a letter, then return the character (things like commas, semicolons, and more)
    else {
        return ch;
    }
}

int main() {
    //The cipher vector (Given in the lab document)
    vector<char> cipher = { 'V','F','X','B','L','I','T','Z','J','R','P','H','D','K','N','O','W','S','G','U','Y','Q','M','A','C','E' };

    //Input string (Diddn't tell to put it in a function or do input check, so I didn't. But it can easily be done)
    string text;
    cout << "Enter a string (max 128 characters): ";
    getline(cin, text);

    //repeating the process for each character
    for (size_t i = 0; i < text.length(); i++) {
        cout << settingCipher(text[i], cipher);
    }

    cout << endl;

    return 0;
}
