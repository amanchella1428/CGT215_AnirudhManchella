#include <iostream>
using namespace std;
int main()
{
	//variables
	float A;
	float B;
	float X;

	//Inputs
	cout << "Welcome to the Batcomputer Iteration I. I will solve Ax+B=0 for x.";
	cout << "\nPlease enter a value for A: ";
	cin >> A;
	cout << "\nPlease enter a value for B: ";
	cin >> B;

	X = (0 - B) / A;

	cout << "A = " << A;
	cout << "B = " << B;
	cout << "Final value of x = " << X;
}