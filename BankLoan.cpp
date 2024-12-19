#include <iostream>
#include <iomanip> // For fixed and setprecision
using namespace std;

int main() {
    // Input variables
    double loanAmount, annualInterestRate, monthlyPayment;

    // Prompt user for input
    cout << "Enter the loan amount: ";
    cin >> loanAmount;

    cout << "Enter the annual interest rate (as a percentage, e.g., 7.2): ";
    cin >> annualInterestRate;

    cout << "Enter the monthly payment: ";
    cin >> monthlyPayment;

    // Convert annual interest rate to monthly interest rate (decimal)
    double monthlyInterestRate = (annualInterestRate / 12) / 100;

    // Check if the monthly payment is too low
    if (monthlyPayment <= loanAmount * monthlyInterestRate) {
        cout << "Monthly payment is too low. The loan amount will increase with this payment." << endl;
        return 1; // Exit the program
    }

    // Variables to track the number of months and remaining balance
    int months = 0;
    double totalInterest = 0.0;
    double balance = loanAmount;

    // Repayment process
    while (balance > 0) {
        // Calculate interest for the current month
        double interest = balance * monthlyInterestRate;
        totalInterest += interest;

        // Calculate payment toward the principal
        double principalPayment = monthlyPayment - interest;

        // Update the remaining balance
        balance -= principalPayment;

        // Increment the month counter
        months++;

        // If the balance becomes negative due to rounding, set it to zero
        if (balance < 0) {
            balance = 0;
        }

        // Output monthly details (optional, for debugging or information)
        cout << "Month " << months << ": Interest = " << fixed << setprecision(2) << interest
             << ", Principal = " << principalPayment
             << ", Remaining Balance = " << balance << endl;
    }

    // Output the total number of months and total interest paid
    cout << "It will take " << months << " months to repay the loan." << endl;
    cout << "Total interest paid: $" << fixed << setprecision(2) << totalInterest << endl;

    return 0;
}
