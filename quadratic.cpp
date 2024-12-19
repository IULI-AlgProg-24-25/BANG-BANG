#include <iostream>
#include <cmath>
#include <complex>

// Function to solve the quadratic equation
void solveQuadratic(double a, double b, double c) {
    if (a == 0) {
        std::cout << "Coefficient 'a' cannot be zero. Not a quadratic equation." << std::endl;
        return;
    }

    double discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {
        // Two real and distinct roots
        double root1 = (-b + sqrt(discriminant)) / (2 * a);
        double root2 = (-b - sqrt(discriminant)) / (2 * a);
        std::cout << "Roots are real and distinct:" << std::endl;
        std::cout << "Root 1: " << root1 << std::endl;
        std::cout << "Root 2: " << root2 << std::endl;
    } else if (discriminant == 0) {
        // One real and repeated root
        double root = -b / (2 * a);
        std::cout << "Roots are real and repeated:" << std::endl;
        std::cout << "Root: " << root << std::endl;
    } else {
        // Complex roots
        std::complex<double> root1((-b / (2 * a)), sqrt(-discriminant) / (2 * a));
        std::complex<double> root2((-b / (2 * a)), -sqrt(-discriminant) / (2 * a));
        std::cout << "Roots are complex and distinct:" << std::endl;
        std::cout << "Root 1: " << root1 << std::endl;
        std::cout << "Root 2: " << root2 << std::endl;
    }
}

int main() {
    double a, b, c;
    std::cout << "Enter coefficients a, b, and c of the quadratic equation (ax^2 + bx + c = 0):" << std::endl;
    std::cin >> a >> b >> c;

    solveQuadratic(a, b, c);

    return 0;
}
