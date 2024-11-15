#include <iostream>
#include <iomanip>

using namespace std;
class ErrorHandling
{
public:
    bool emailValidation(const string &email)
    {
        const string gmailSuffix = "@gmail.com";

        if (email.empty())
        {
            return false;
        }

        for (char ch : email)
        {
            if (isspace(ch))
            {
                return false;
            }
        }

        if (email.size() <= gmailSuffix.size() + 4)
        {
            return false;
        }

        string prefix = email.substr(0, email.size() - gmailSuffix.size());
        string suffix = email.substr(email.size() - gmailSuffix.size());

        if (suffix != gmailSuffix)
        {
            return false;
        }

        for (char ch : prefix)
        {
            if (!islower(ch) && !isdigit(ch) && ch != '.' && ch != '_' && ch != '-')
            {
                return false;
            }
        }
        return true;
    }
    bool passwordValidation(const string &str)
    {
        bool isLengthValid = str.length() >= 6;

        if (!isLengthValid)
        {
            cout << "\n\n\tPassword must be at least 6 characters long.\n\n";
        }

        return isLengthValid;
    }

    bool nameValidation(string str)
    {
        if (str.empty())
        {
            return false;
        }
        return true;
    }
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