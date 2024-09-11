#include <iostream>
using namespace std;


// Print out the menu of choices for the user to select from
void printMenu() {
	cout << "Please Select which operation to perform:" << endl;
	cout << "\t1. Factorial" << endl;
	cout << "\t2. Arithmetic Series" << endl;
	cout << "\t3. Geometric Series" << endl;
	cout << "\t4. Exit" << endl;
	cout << "Your Selection: ";
}

//Facotrial function
void factorial() {
	int startNum;
	int finalNum = 1;

	cout << "Please enter a positive number to calculate: " << endl;
	cin >> startNum;
	//Input check
	while (startNum < 0) {
		cout << "INVALID INPUT. PLEASE ENTER PROPER INPUT. " << endl;
		cin >> startNum;
	}

	//Logic
	for (int i = 1; i <= startNum; i++) {
		finalNum = finalNum * i;
	}

	//output
	cout << "Your final answer is: " << finalNum << endl;
}
void arithmetic() {
	float startNum;
	float differenceNum;
	float finalNum = 0;
	float addons;
	int forLoopControl;
	

	//First two don't need input check because they just need to be the right data type
	cout << "Please enter your starting number: " << endl;
	cin >> startNum;

	cout << "Please enter the difference: " << endl;
	cin >> differenceNum;
	
	cout << "Please enter number of elements (Must be a positive whole number): " << endl;
	cin >> forLoopControl;
	while (forLoopControl < 1) {
		cout << "INVALID INPUT. PLEASE ENTER PROPER INPUT. " << endl;
		cin >> forLoopControl;
	}

	for (int i = 0; i < forLoopControl; i++) {
		addons = startNum + i * differenceNum;
		finalNum = finalNum + addons;
		cout << addons << " + ";

	}

	//Ouput
	cout << " =  " << finalNum << endl;

}
void geometric() {
	float startNum;
	float differenceNum;
	float finalNum = 0;
	float addons;
	int forLoopControl;

	//First two don't need input check because they just need to be the right data type
	cout << "Please enter your starting number: " << endl;
	cin >> startNum;

	cout << "Please enter the number to multiply each time: " << endl;
	cin >> differenceNum;

	cout << "Please enter number of elements (Must be a positive whole number): " << endl;
	cin >> forLoopControl;
	while (forLoopControl < 1) {
		cout << "INVALID INPUT. PLEASE ENTER PROPER INPUT. " << endl;
		cin >> forLoopControl;
	}

	//Logic
	for (int i = 0; i < forLoopControl; i++) {
		addons = startNum * pow(differenceNum, i);
		finalNum = finalNum + addons;
		cout << addons << " + ";
	}

	//output
	cout << " =  " << finalNum << endl;

}
int main() {
	int choice;
	char again;
	
	do {
		printMenu();
		cin >> choice;
		
		//Input Check
		while (choice > 3 || choice < 1) {
			cout << "INVALID INPUT. PLEASE ENTER PROPER INPUT. " << endl;
			cin >> choice;
		}

		if (choice == 1) {
			factorial();
		}
		else if (choice == 2) {
			arithmetic();
		}
		else if (choice == 3) {
			geometric();
		}
		cout << "Go Again? [Y/N] ";
		cin >> again;
	} while (again == 'y' || again == 'Y');

	if (again == 'n' || again == 'N') {
		cout << "Thank you!!" << endl;
	}



	//This is Great, but no
		//// Quit if user chooses to exit (or any invalid choice)
		//if (choice > 3 || choice < 1) {
		//	return 0;
		//}
}