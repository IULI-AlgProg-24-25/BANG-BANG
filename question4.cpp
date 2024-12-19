#include <iostream> // a. Include header file iostream
using namespace std; // b. Allow the use of cin, cout, and endl without std::

int main() {
    // c. Declare variables
    double num1, num2, num3, average;

    // d. Assign values to variables
    num1 = 75.35;
    num2 = -35.56;
    num3 = 15.76;

    // e. Compute the average
    average = (num1 + num2 + num3) / 3;

    // f. Output the values of num1, num2, num3, and average
    cout << "num1: " << num1 << endl;
    cout << "num2: " << num2 << endl;
    cout << "num3: " << num3 << endl;
    cout << "average: " << average << endl;

    return 0; // g. Return statement
}
