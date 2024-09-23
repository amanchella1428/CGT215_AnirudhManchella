#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to encode a character using the cipher
char encodeChar(char ch, const vector<char>& cipher) {
    if (ch >= 'A' && ch <= 'Z') {
        return cipher[ch - 'A']; // Encode uppercase letter
    }
    else if (ch >= 'a' && ch <= 'z') {
        // Convert lowercase to uppercase, encode, then convert back to lowercase
        char upperCaseChar = ch - 32;
        return cipher[upperCaseChar - 'A'] + 32;
    }
    else {
        return ch; // If not a letter, return the character as is
    }
}

int main() {
    // The cipher vector
    vector<char> cipher = { 'V','F','X','B','L','I','T','Z','J','R','P','H','D','K','N','O','W','S','G','U','Y','Q','M','A','C','E' };

    // Input string
    string text;
    cout << "Enter a string (max 128 characters): ";
    getline(cin, text);

    // Iterate over the input string and encode each character
    for (size_t i = 0; i < text.length(); i++) {
        cout << encodeChar(text[i], cipher);
    }

    cout << endl;

    return 0;
}
