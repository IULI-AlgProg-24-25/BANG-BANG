#include <iostream>
using namespace std;

int main() {
    int length, width, area, perimeter;

    // Prompt user to enter the length
    cout << "Enter the length: ";
    cin >> length;
    cout << endl;

    // Prompt user to enter the width
    cout << "Enter the width: ";
    cin >> width;
    cout << endl;

    // Calculate the area and perimeter
    area = length * width;
    perimeter = 2 * (length + width);

    // Output the area and perimeter
    cout << "Area = " << area << endl;
    cout << "Perimeter = " << perimeter << endl;

    return 0;
}
