#include <iostream>

using namespace std;

//The main menu
void printMenu(int& choice, int& enterProgram) {
	if (enterProgram == 1) {
		cout << "Welcome to simple calculator  solving for C where A _ B = C." << endl;
		cout << "please select an operation to perform: " << endl;
		cout << "1. Multiply" << endl;
		cout << "2. Divide" << endl;
		cout << "3. Add" << endl;
		cout << "4. Subtract" << endl;
		cout << "5. Exit" << endl;
		cout << "\nYour Selection: ";
		cin >> choice;
		while (choice > 5 || choice < 1) {
			cout << "INVALID INPUT. PLEASE ENTER PROPER INPUT. " << endl;
			cin >> choice;
		}
	}
}

//function to get the choices
void getChoices(float& A, float& B) {
	cout << "Please note commutative property and when it does and does not apply." << endl;
	cout << "Please enter the first value:";
	cin >> A;
	cout << "Please enter the second value:";
	cin >> B;
	// The rest of this function is an exercise to the reader
}

//function for the first choice
void multiplyFunction(float A, float B) {
	float C;
	C = A * B;
	cout << A << " * " << B << " = " << C << endl;
}

//function for the second choice
void divideFunction(float A, float B) {
	if (B == 0) {
		cout << "CANNOT DIVIDE BY 0" << endl;
	}
	else {
		float C;

		C = A / B;
		cout << A << " / " << B << " = " << C << endl;
	}
}

//function for the third choice
void addFunction(float A, float B) {
	float C;
	C = A + B;
	cout << A << " + " << B << " = " << C << endl;
}

//function for the fourth choice
void subtractFunction(float A, float B) {
	float C;
	C = A - B;
	cout << A << " - " << B << " = " << C << endl;
}

//function for the fifth choice
void exitFunction() {
	cout << "STOPPING PROGRAM" << endl;
}

//Input check
void askAgain(int& enterProgram) {
	if (enterProgram == 0) {
		cout << "Would you like to calculate another operation?" << endl;
		cout << "1. YES" << endl;
		cout << "2. NO" << endl;
		cin >> enterProgram;
		while (enterProgram != 1 && enterProgram != 2) {
			cout << "INVALID INPUT. PLEASE ENTER PROPER INPUT. " << endl;
			cin >> enterProgram;
		}
	}
}

//Main function
int main() {

	//variable declaration
	int enterProgram = 1;
	int choice;
	float A;
	float B;

	while (enterProgram == 1) {
		//main menu choices
		printMenu(choice, enterProgram);

		//Getting user input
		getChoices(A, B);

		//Program decisions
		if (choice == 1) {
			multiplyFunction(A, B);
		}
		if (choice == 2) {
			divideFunction(A, B);
		}
		if (choice == 3) {
			addFunction(A, B);
		}
		if (choice == 4) {
			subtractFunction(A, B);
		}
		if (choice == 5) {
			exitFunction();
		}

		//resetting the user choice
		enterProgram = 0;
		askAgain(enterProgram);
	}
	return 0;
}