#include <iostream>
#include <iomanip>

using namespace std;
class ErrorHandling
{
public:
    bool menuChoice(string choice)
    {
        for (int i = 0; i < choice.size(); ++i)
        {
            if (!isdigit(choice[i]))
            {
                return false;
            }
        }
        if (choice.empty())
        {
            return false;
        }
        for (int i = 0; i < choice.size(); ++i)
        {
            if (choice[i] == ' ')
            {
                return false;
            }
        }
        return true;
    }
};

int main()
{
    cout << "\n\n\t**** Event Ticket Reservation System ****\n";
    cout << "\n\t\t ______________________________________________\n";
    cout << "\t\t|       |" << setw(40) << "|\n";
    cout << "\t\t| [1]   |     Sign Up" << setw(28) << "|\n";
    cout << "\t\t| [2]   |     Login" << setw(30) << "|\n";
    cout << "\t\t| [3]   |     Admin Panel" << setw(24) << "|\n";
    cout << "\t\t| [4]   |     Search Events" << setw(22) << "|\n";
    cout << "\t\t| [5]   |     Exit" << setw(31) << "|\n";
    cout << "\t\t||_|\n";
    
    return 0;
}