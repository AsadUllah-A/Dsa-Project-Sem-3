#include <iostream>
#include <conio.h>
#include <iomanip>
#include <string>
#include <queue>
#include <unordered_map>
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

    void passLogic(string &password, string promptText)
    {
        char pass[32] = {0};
        char ch;
        bool enter = false;
        int i = 0;
        bool show = false;

        cout << promptText;

        while (!enter)
        {
            ch = _getch();

            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
            {
                pass[i] = ch;
                if (show)
                {
                    cout << ch;
                }
                else
                {
                    cout << "*";
                }
                i++;
            }

            if (ch == '\b' && i >= 1)
            {
                cout << "\b \b";
                i--;
            }

            if (ch == '\r')
            {
                enter = true;
            }

            if (ch == '\t')
            {
                show = !show;
                cout << "\r" << promptText;
                for (int j = 0; j < i; j++)
                {
                    cout << (show ? pass[j] : '*');
                }
            }
        }

        pass[i] = '\0';
        password = pass;
    }

    bool idValidation(string idString)
    {
        for (int i = 0; i < idString.size(); ++i)
        {
            if (!isdigit(idString[i]))
            {
                return false;
            }
        }
        if (idString.empty())
        {
            return false;
        }
        for (int i = 0; i < idString.size(); ++i)
        {
            if (idString[i] == ' ')
            {
                return false;
            }
        }
        return true;
    }
};
ErrorHandling errorHandler;

struct User
{
    string username;
    string password;
    string email;
    unordered_map<int, int> tickets;
    unordered_map<int, int> canceledVipTickets;
    unordered_map<int, int> canceledRegularTickets;
};

struct Event
{
    int eventId;
    string eventName;
    int totalSeats;
    int vipSeats;
    int regularSeats;
    priority_queue<int> vipSeatsQueue;
    priority_queue<int, vector<int>, greater<int>> regularSeatsQueue;
    Event() : eventId(0), eventName(""), totalSeats(0), vipSeats(0), regularSeats(0) {}

    Event(int id, const string &name, int total, int vip, int regular)
        : eventId(id), eventName(name), totalSeats(total), vipSeats(vip), regularSeats(regular) {}
};

unordered_map<string, User> users;
unordered_map<int, Event> events;
unordered_map<string, bool> admins;
void setupDefaultAdmin();
void updateUserProfile(const string &username);
void signup();
void login();
bool adminLogin(string &loggedInAdmin);
void searchEvents();
bool adminLogin();
void buyTickets(const string &username);
void viewTickets(const string &username);
void cancelTicket(const string &username);
void addEvent();
void updateEvent();
void manageTickets();
void viewAllUsers();
void displayUserMenu(const string &username);
void addNewAdmin();
void removeAdmin();
void updateadminPanel(const string &adminUsername);
void updateAdminPassword(const string &adminUsername);
void displayAdminMenu();
void displayAllEvents();
void displayAllUserData();

void displayAllUserData()
{
    if (users.empty())
    {
        cout << "No users available.\n";
        return;
    }

    cout << "User Data:\n";
    for (const auto &userPair : users)
    {
        const string &username = userPair.first;
        const User &user = userPair.second;

        if (admins.find(username) != admins.end() && admins.at(username))
        {
            continue;
        }

        cout << "Username: " << user.username << "\nEmail: " << user.email << "\nTickets:\n";

        if (user.tickets.empty())
        {
            cout << "  No tickets bought.\n";
        }
        else
        {
            for (const auto &ticketPair : user.tickets)
            {
                int eventID = ticketPair.first;
                int totalTickets = ticketPair.second;

                int vipTickets = 0;
                int regularTickets = 0;
                const Event &event = events.at(eventID);

                if (user.canceledVipTickets.find(eventID) != user.canceledVipTickets.end())
                {
                    vipTickets = user.canceledVipTickets.at(eventID);
                }

                if (user.canceledRegularTickets.find(eventID) != user.canceledRegularTickets.end())
                {
                    regularTickets = user.canceledRegularTickets.at(eventID);
                }

                cout << "\nEvent ID: " << eventID
                     << ",\tEvent Name: " << event.eventName
                     << ",\tVIP Tickets: " << vipTickets
                     << ",\tRegular Tickets: " << regularTickets << '\n';
            }
        }

        cout << "-------------------------------\n";
    }
}

void updateUserProfile(const string &username)
{
    if (users.find(username) == users.end())
    {
        cout << "\n\tUser not found.";
        return;
    }

    string choice;
    string newValue;
    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Update Username" << setw(20) << "|\n";
        cout << "\t\t| [2]   |     Update Password" << setw(20) << "|\n";
        cout << "\t\t| [3]   |     Update Email" << setw(23) << "|\n";
        cout << "\t\t| [4]   |     Exit" << setw(31) << "|\n";
        cout << "\n\t\t ______________________________________________\n";
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "1")
        {
            while (true)
            {
                cout << "Enter new username: ";
                getline(cin, newValue);
                if (errorHandler.nameValidation(newValue))
                {
                    break;
                }
                cout << "\n\tInvalid username\n\n";
            }
            users[username].username = newValue;
            cout << "\n\tUsername updated successfully!\n";
        }

        else if (choice == "2")
        {
            while (true)
            {
                errorHandler.passLogic(newValue, "Enter new Password: ");
                if (errorHandler.passwordValidation(newValue))
                {
                    break;
                }
            }
            users[username].password = newValue;
            cout << "\n\tPassword updated successfully!\n";
        }
        else if (choice == "3")
        {
            while (true)
            {
                cout << "Enter new email: ";
                getline(cin, newValue);
                if (errorHandler.emailValidation(newValue))
                {
                    break;
                }
                else
                {
                    cout << "\n\tInvalid Email. Please try again.\n\n";
                }
            }
            users[username].email = newValue;
            cout << "\n\tEmail updated successfully!\n";
        }
        else if (choice == "4")
        {
            cout << "\nExiting User Panel...\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid choice. Please try again.\n";
        }
    }
}

void signup()
{
    string username, password, email;

    while (true)
    {

        while (true)
        {
            cout << "\nEnter Username: ";
            getline(cin, username);
            if (errorHandler.nameValidation(username))
            {
                break;
            }
            else
            {
                cout << "\n\n\tInvalid Username. Please try again.\n";
            }
        }

        string lowerUsername = toLowerCase(username);

        bool usernameExists = false;
        for (const auto &user : users)
        {
            if (toLowerCase(user.first) == lowerUsername)
            {
                usernameExists = true;
                break;
            }
        }

        if (usernameExists)
        {
            cout << "\n\tUsername already exists. Please try again.\n";
            continue;
        }

        if (errorHandler.nameValidation(username))
        {
            break;
        }
        else
        {
            cout << "\n\tInvalid Username. Please try again.\n";
        }
    }

    while (true)
    {
        errorHandler.passLogic(password, "Enter Password: ");
        if (errorHandler.passwordValidation(password))
        {
            break;
        }
    }

    while (true)
    {
        cout << "\nEnter Valid Email: ";
        getline(cin, email);

        string lowerEmail = toLowerCase(email);

        bool emailExists = false;
        for (const auto &user : users)
        {
            if (toLowerCase(user.second.email) == lowerEmail)
            {
                emailExists = true;
                break;
            }
        }

        if (emailExists)
        {
            cout << "\n\tEmail already exists. Please try again.\n";
            continue;
        }

        if (errorHandler.emailValidation(email))
        {
            break;
        }
        else
        {
            cout << "\n\tInvalid Email. Please try again.\n";
        }
    }

    User newUser = {username, password, email, {}};
    users[username] = newUser;

    cout << "\n\tSignup Successfully!";
}

void login()
{
    string username, password;
    while (true)
    {
        cout << "\nEnter Username: ";
        getline(cin, username);
        if (errorHandler.nameValidation(username))
        {
            break;
        }
        else
        {
            cout << "\n\n\tInvalid Username. Please try again.\n";
        }
    }

    errorHandler.passLogic(password, "Enter Password: ");
    string lowerUsername = toLowerCase(username);
    bool userFound = false;
    for (const auto &user : users)
    {
        if (toLowerCase(user.first) == lowerUsername && user.second.password == password)
        {
            userFound = true;
            cout << "\n\n\tWelcome, " << user.first << ".\n";
            displayUserMenu(user.first);
            break;
        }
    }

    if (!userFound)
    {
        cout << "\n\n\tInvalid username or password!\n";
    }
}

bool adminLogin(string &loggedInAdmin)
{
    string username, password;
    while (true)
    {
        cout << "\nEnter Admin Username: ";
        getline(cin, username);
        if (errorHandler.nameValidation(username))
        {
            break;
        }
        else
        {
            cout << "\n\n\tInvalid Username. Please try again.\n";
        }
    }
    errorHandler.passLogic(password, "Enter Admin Password: ");

    if (admins.find(username) == admins.end())
    {
        cout << "\n\n\tAdmin not found.\n";
        return false;
    }
    if (users[username].password != password)
    {
        cout << "Incorrect password.\n";
        return false;
    }

    loggedInAdmin = username;
    return true;
}

void updateAdminPassword(const string &adminUsername)
{
    if (admins.find(adminUsername) == admins.end())
    {
        cout << "You must be an admin to access this feature.\n";
        return;
    }

    string newPassword;
    while (true)
    {
        errorHandler.passLogic(newPassword, "Enter new Password: ");
        if (errorHandler.passwordValidation(newPassword))
            break;
    }
    users[adminUsername].password = newPassword;
    cout << "\n\n\tPassword updated successfully!\n";
}

void viewAllUsers()
{
    if (users.empty())
    {
        cout << "\nNo users are registered yet.\n";
    }
    else
    {
        cout << "\nRegistered Users:\n";
        for (const auto &user : users)
        {
            cout << "Username: " << user.second.username << ", Email: " << user.second.email << "\n";
        }
    }
}

void buyTickets(const string &username)
{
    if (events.empty())
    {
        cout << "No events available.\n";
        return;
    }

    string eventIDStr;
    int eventID;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);

    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);

    Event &event = events[eventID];
    int vipTicketsBooked = 0;
    int regularTicketsBooked = 0;

    while (true)
    {
        cout << "\n\t\t ________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Buy VIP tickets" << setw(20) << "|\n";
        cout << "\t\t| [2]   |     Buy Regular tickets" << setw(16) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|__|_____________|\n";

        string choice;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "1" || choice == "2")
        {
            string numberOfTicketsStr;
            int numberOfTickets;

            cout << "Enter number of tickets: ";
            getline(cin, numberOfTicketsStr);

            if (!errorHandler.idValidation(numberOfTicketsStr))
            {
                cout << "Number of tickets must be a positive integer.\n";
                continue;
            }
            numberOfTickets = stoi(numberOfTicketsStr);

            if (choice == "1")
            {
                if (event.vipSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough VIP seats available. Total VIP seats left: " << event.vipSeatsQueue.size() << ".\n";
                    return;
                }

                while (vipTicketsBooked < numberOfTickets && !event.vipSeatsQueue.empty())
                {
                    event.vipSeatsQueue.pop();
                    vipTicketsBooked++;
                }

                users[username].canceledVipTickets[eventID] += vipTicketsBooked;
            }
            else if (choice == "2")
            {
                if (event.regularSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough Regular seats available. Total Regular seats left: " << event.regularSeatsQueue.size() << ".\n";
                    return;
                }

                while (regularTicketsBooked < numberOfTickets && !event.regularSeatsQueue.empty())
                {
                    event.regularSeatsQueue.pop();
                    regularTicketsBooked++;
                }

                users[username].canceledRegularTickets[eventID] += regularTicketsBooked;
            }

            if ((choice == "1" && vipTicketsBooked == numberOfTickets) || (choice == "2" && regularTicketsBooked == numberOfTickets))
            {
                users[username].tickets[eventID] += numberOfTickets;
                cout << "Tickets booked successfully! ";

                if (choice == "1")
                    cout << "VIP: " << vipTicketsBooked << "\n";
                else if (choice == "2")
                    cout << "Regular: " << regularTicketsBooked << "\n";

                return;
            }
        }
        else if (choice == "0")
        {
            cout << "\nExiting...\n";
            return;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
void cancelTicket(const string &username)
{
    string eventIDStr;
    int eventID;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);

    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);

    if (users.find(username) == users.end())
    {
        cout << "User not found.\n";
        return;
    }

    if (users[username].tickets.find(eventID) == users[username].tickets.end())
    {
        cout << "You have not bought any tickets for this event.\n";
        return;
    }

    Event &event = events[eventID];

    while (true)
    {
        cout << "\n\t\t ________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Cancel VIP tickets" << setw(17) << "|\n";
        cout << "\t\t| [2]   |     Cancel Regular tickets" << setw(13) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|__|_____________|\n";

        string choice;
        cout << "Enter your choice: ";
        getline(cin, choice);

        if (choice == "1" || choice == "2")
        {
            int vipTicketsBooked = users[username].canceledVipTickets[eventID];
            int regularTicketsBooked = users[username].canceledRegularTickets[eventID];

            if ((choice == "1" && vipTicketsBooked == 0) || (choice == "2" && regularTicketsBooked == 0))
            {
                cout << "You do not have any " << (choice == "1" ? "VIP" : "Regular") << " tickets for this event.\n";
                continue;
            }

            string numberOfTicketsStr;
            int numberOfTickets;

            cout << "Enter number of tickets to cancel: ";
            getline(cin, numberOfTicketsStr);

            if (!errorHandler.idValidation(numberOfTicketsStr))
            {
                cout << "Number of tickets must be a positive integer.\n";
                continue;
            }

            numberOfTickets = stoi(numberOfTicketsStr);

            if (choice == "1" && vipTicketsBooked < numberOfTickets)
            {
                cout << "You don't have that many VIP tickets.\n";
                return;
            }
            if (choice == "2" && regularTicketsBooked < numberOfTickets)
            {
                cout << "You don't have that many Regular tickets.\n";
                return;
            }

            if (choice == "1")
            {
                int seatsToCancel = min(numberOfTickets, vipTicketsBooked);

                for (int i = 0; i < seatsToCancel; ++i)
                {
                    event.vipSeatsQueue.push(0);
                }
                users[username].canceledVipTickets[eventID] -= seatsToCancel;
                users[username].tickets[eventID] -= seatsToCancel;
                if (users[username].tickets[eventID] == 0)
                {
                    users[username].tickets.erase(eventID);
                }

                cout << "VIP tickets cancelled successfully!\n";
                return;
            }
            else if (choice == "2")
            {
                int seatsToCancel = min(numberOfTickets, regularTicketsBooked);

                for (int i = 0; i < seatsToCancel; ++i)
                {
                    event.regularSeatsQueue.push(0);
                }
                users[username].canceledRegularTickets[eventID] -= seatsToCancel;
                users[username].tickets[eventID] -= seatsToCancel;
                if (users[username].tickets[eventID] == 0)
                {
                    users[username].tickets.erase(eventID);
                }

                cout << "Regular tickets cancelled successfully!\n";
                return;
            }
        }
        else if (choice == "0")
        {
            cout << "\nExiting...\n";
            return;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
void viewTickets(const string &username)
{
    if (users.find(username) == users.end())
    {
        cout << "User not found.\n";
        return;
    }

    const User &user = users[username];

    if (user.tickets.empty())
    {
        cout << "You have not purchased any tickets.\n";
        return;
    }

    cout << "Your tickets:\n";
    for (const auto &ticket : user.tickets)
    {
        int eventID = ticket.first;
        int totalTickets = ticket.second;
        if (events.find(eventID) == events.end())
        {
            cout << "Event ID " << eventID << " has been deleted and no longer exists.\n";
            continue;
        }

        int vipTickets = 0;
        int regularTickets = 0;

        if (user.canceledVipTickets.find(eventID) != user.canceledVipTickets.end())
        {
            vipTickets = user.canceledVipTickets.at(eventID);
        }

        if (user.canceledRegularTickets.find(eventID) != user.canceledRegularTickets.end())
        {
            regularTickets = user.canceledRegularTickets.at(eventID);
        }

        cout << "\nEvent ID: " << eventID
             << ",\tEvent Name: " << events[eventID].eventName
             << ",\tVIP Tickets: " << vipTickets
             << ",\tRegular Tickets: " << regularTickets << '\n';
    }
}
void manageTickets()
{
    string eventIDStr;
    int eventID;

    while (true)
    {
        cout << "Enter event ID to manage tickets: ";
        getline(cin, eventIDStr);

        if (errorHandler.idValidation(eventIDStr))
        {
            eventID = stoi(eventIDStr);
            break;
        }
        else
        {
            cout << "Invalid event ID. Please enter a numeric event ID with no spaces.\n";
            continue;
        }
    }

    if (events.find(eventID) == events.end())
    {
        cout << "Event not found.\n";
        return;
    }

    Event &event = events[eventID];
    cout << "Managing tickets for Event ID: " << event.eventId << ", Event Name: " << event.eventName << '\n';
    cout << "Total seats: " << event.totalSeats << ", VIP seats: " << event.vipSeats << ", Regular seats: " << event.regularSeats << '\n';
    cout << "Available VIP seats: " << event.vipSeatsQueue.size() << ", Available Regular seats: " << event.regularSeatsQueue.size() << '\n';

    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Add seats" << setw(26) << "|\n";
        cout << "\t\t| [2]   |     Remove seats" << setw(23) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t||_|\n";

        string choice;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (choice == "0" || errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "0")
        {
            cout << "Exiting ticket management.\n";
            break;
        }

        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     VIP Seats" << setw(26) << "|\n";
        cout << "\t\t| [2]   |     Regular Seats" << setw(22) << "|\n";
        cout << "\t\t| [0]   |     Back to Main Menu" << setw(19) << "|\n";
        cout << "\t\t||_|\n";

        string option;
        while (true)
        {
            cout << "Enter seat type: ";
            getline(cin, option);
            if (option == "0" || errorHandler.menuChoice(option))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (option == "0")
        {
            cout << "Returning to main menu.\n";
            continue;
        }

        string seatsStr;
        int seats;

        while (true)
        {
            cout << "Enter number of seats to " << (choice == "1" ? "add: " : "remove: ");
            getline(cin, seatsStr);

            if (errorHandler.idValidation(seatsStr))
            {
                seats = stoi(seatsStr);
                break;
            }
            else
            {
                cout << "Invalid number of seats. Please enter a numeric value with no spaces.\n";
            }
        }

        if (choice == "1")
        {
            if (option == "1")
            {
                for (int i = 0; i < seats; ++i)
                {
                    events[eventID].vipSeatsQueue.push(events[eventID].vipSeats + 1 + i);
                }
                events[eventID].vipSeats += seats;
            }
            else if (option == "2")
            {
                for (int i = 0; i < seats; ++i)
                {
                    events[eventID].regularSeatsQueue.push(events[eventID].regularSeats + 1 + i);
                }
                events[eventID].regularSeats += seats;
            }
            events[eventID].totalSeats += seats;
            cout << "Seats added successfully.\n";
        }
        else if (choice == "2")
        {
            if (option == "1")
            {
                if (seats <= event.vipSeatsQueue.size())
                {
                    for (int i = 0; i < seats; ++i)
                    {
                        events[eventID].vipSeatsQueue.pop();
                    }
                    events[eventID].vipSeats -= seats;
                    events[eventID].totalSeats -= seats;
                    cout << "VIP seats removed successfully.\n";
                }
                else
                {
                    cout << "Not enough VIP seats available to remove.\n";
                }
            }
            else if (option == "2")
            {
                if (seats <= event.regularSeatsQueue.size())
                {
                    for (int i = 0; i < seats; ++i)
                    {
                        events[eventID].regularSeatsQueue.pop();
                    }
                    events[eventID].regularSeats -= seats;
                    events[eventID].totalSeats -= seats;
                    cout << "Regular seats removed successfully.\n";
                }
                else
                {
                    cout << "Not enough Regular seats available to remove.\n";
                }
            }
        }

        cout << "Current state for Event ID: " << event.eventId << ", Event Name: " << event.eventName << '\n';
        cout << "Total seats: " << event.totalSeats << ", VIP seats: " << event.vipSeats << ", Regular seats: " << event.regularSeats << "\n";
        cout << "Available VIP seats: " << event.vipSeatsQueue.size() << ", Available Regular seats: " << event.regularSeatsQueue.size() << "\n";
    }
}
void displayUserMenu(const string &username)
{
    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Buy Tickets" << setw(24) << "|\n";
        cout << "\t\t| [2]   |     View Tickets" << setw(23) << "|\n";
        cout << "\t\t| [3]   |     Cancel Tickets" << setw(21) << "|\n";
        cout << "\t\t| [4]   |     View All Events" << setw(20) << "|\n";
        cout << "\t\t| [5]   |     Update Profile" << setw(21) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t||_|\n";

        string option;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, option);
            if (errorHandler.menuChoice(option))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }
        if (option == "1")
        {
            buyTickets(username);
        }
        else if (option == "2")
        {
            viewTickets(username);
        }
        else if (option == "3")
        {
            cancelTicket(username);
        }
        else if (option == "4")
        {
            displayAllEvents();
        }
        else if (option == "5")
        {
            updateUserProfile(username);
        }
        else if (option == "0")
        {
            cout << "Exit User Panel\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
string toLowerCase(const string &str)
{
    string lowerStr = str; // Create a copy to modify
    for (int i = 0; i < lowerStr.length(); i++)
    {
        lowerStr[i] = tolower(lowerStr[i]);
    }
    return lowerStr;
}

void setupDefaultAdmin()
{
    string defaultAdminUsername1 = "asad";
    string defaultAdminPassword1 = "55181";
    string defaultAdminEmail1 = "admin1@example.com";

    if (users.find(defaultAdminUsername1) == users.end())
    {
        User adminUser = {defaultAdminUsername1, defaultAdminPassword1, defaultAdminEmail1, {}};
        users[defaultAdminUsername1] = adminUser;
        admins[defaultAdminUsername1] = true;
        cout << "Default admin account created successfully!\n";
    }
    else
    {
        cout << "Default admin already exists.\n";
    }

    string defaultAdminUsername2 = "ahmed";
    string defaultAdminPassword2 = "55223";
    string defaultAdminEmail2 = "admin2@example.com";

    if (users.find(defaultAdminUsername2) == users.end())
    {
        User adminUser = {defaultAdminUsername2, defaultAdminPassword2, defaultAdminEmail2, {}};
        users[defaultAdminUsername2] = adminUser;
        admins[defaultAdminUsername2] = true;
        cout << "Default admin account created successfully!\n";
    }
    else
    {
        cout << "Default admin already exists.\n";
    }

    string defaultAdminUsername3 = "irtaza";
    string defaultAdminPassword3 = "55181";
    string defaultAdminEmail3 = "admin3@example.com";

    if (users.find(defaultAdminUsername3) == users.end())
    {
        User adminUser = {defaultAdminUsername3, defaultAdminPassword3, defaultAdminEmail3, {}};
        users[defaultAdminUsername3] = adminUser;
        admins[defaultAdminUsername3] = true;
        cout << "Default admin account created successfully!\n";
    }
    else
    {
        cout << "Default admin already exists.\n";
    }
}

int main()
{
    setupDefaultAdmin();
    while (true)
    {
        cout << "\n\n\t\t    **** Event Ticket Reservation System ****\n";
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Sign Up" << setw(28) << "|\n";
        cout << "\t\t| [2]   |     Login" << setw(30) << "|\n";
        cout << "\t\t| [3]   |     Admin Panel" << setw(24) << "|\n";
        cout << "\t\t| [4]   |     Search Events" << setw(22) << "|\n";
        cout << "\t\t| [5]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        string choice;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "1")
        {
            signup();
        }
        else if (choice == "2")
        {
            login();
        }
        else if (choice == "4")
        {
            searchEvents();
        }
        else if (choice == "3")
        {
            displayAdminMenu();
        }
        else if (choice == "5")

        {
            cout << "\n\tExiting... Have a great day!\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid option! Please try again.\n";
        }
    }
    return 0;
}