#include <iostream>
#include <conio.h>       // For `_getch` to capture user input without echo
#include <iomanip>       // For formatted output (e.g., setting width in console)
#include <string>        // For working with strings
#include <queue>         // For using priority queues
#include <unordered_map> // For hash table-based key-value data structure
using namespace std;     // To avoid writing `std::` repeatedly
// Error handling class for validation of inputs such as email, password, and more
class ErrorHandling
{
public:
    // Validates email format
    bool emailValidation(const string &email)
    {
        const string gmailSuffix = "@gmail.com"; // Define required email suffix
        if (email.empty())                       // Check if email is empty
        {
            return false;
        }
        for (char ch : email) // Check if email contains any spaces
        {
            if (isspace(ch))
            {
                return false;
            }
        }
        if (email.size() <= gmailSuffix.size() + 4) // Minimum valid email length
        {
            return false;
        }
        // Split the email into prefix and suffix
        string prefix = email.substr(0, email.size() - gmailSuffix.size());
        string suffix = email.substr(email.size() - gmailSuffix.size());
        // Ensure suffix matches "@gmail.com"
        if (suffix != gmailSuffix)
        {
            return false;
        }
        // Check prefix for valid characters
        for (char ch : prefix)
        {
            if (!islower(ch) && !isdigit(ch) && ch != '.' && ch != '_' && ch != '-')
            {
                return false;
            }
        }
        return true; // Email is valid
    }
    // Validates password length
    bool passwordValidation(const string &str)
    {
        bool isLengthValid = str.length() >= 6; // Check if password has at least 6 characters

        if (!isLengthValid) // If password is too short, print an error message
        {
            cout << "\n\n\tPassword must be at least 6 characters long.\n\n";
        }
        return isLengthValid;
    }
    // Validates username (ensures it's not empty)
    bool nameValidation(string str)
    {
        if (str.empty()) // Check if the username is empty
        {
            return false;
        }
        return true;
    }
    // Validates menu choice to ensure it contains only digits
    bool menuChoice(string choice)
    {
        for (int i = 0; i < choice.size(); ++i)
        {
            if (!isdigit(choice[i])) // Ensure all characters are digits
            {
                return false;
            }
        }
        if (choice.empty()) // Ensure choice is not empty
        {
            return false;
        }
        for (int i = 0; i < choice.size(); ++i)
        {
            if (choice[i] == ' ') // Ensure no spaces in the choice
            {
                return false;
            }
        }
        return true;
    }
    // Custom logic for handling password input with visibility toggle
    void passLogic(string &password, string promptText)
    {
        char pass[32] = {0}; // Temporary buffer to store password characters
        char ch;             // To capture each input character
        bool enter = false;  // Flag to indicate when the user presses 'Enter'
        int i = 0;           // Index for the password array
        bool show = false;   // Toggle for password visibility (hidden/show)
        cout << promptText;  // Display the prompt message

        while (!enter) // Loop until the user presses 'Enter'
        {
            ch = _getch(); // Get a single character from the user
            // Check for valid password characters (letters and digits)
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
            {
                pass[i] = ch; // Store the character in the buffer
                if (show)     // If password is visible, display the character
                {
                    cout << ch;
                }
                else // If password is hidden, display '*'
                {
                    cout << "*";
                }
                i++; // Move to the next character
            }
            if (ch == '\b' && i >= 1) // Handle backspace
            {
                cout << "\b \b"; // Remove the last character from the console
                i--;
            }
            if (ch == '\r') // Enter key pressed
            {
                enter = true;
            }
            if (ch == '\t') // Tab key toggles visibility
            {
                show = !show;               // Flip the show flag
                cout << "\r" << promptText; // Reset to the beginning of the line
                for (int j = 0; j < i; j++)
                {
                    cout << (show ? pass[j] : '*'); // Re-display the password
                }
            }
        }

        pass[i] = '\0';  // Null-terminate the password string
        password = pass; // Assign the password to the passed reference
    }
    // Validates if an ID contains only digits and no spaces
    bool idValidation(string idString)
    {
        for (int i = 0; i < idString.size(); ++i)
        {
            if (!isdigit(idString[i])) // Ensure all characters are digits
            {
                return false;
            }
        }
        if (idString.empty()) // Ensure ID is not empty
        {
            return false;
        }
        for (int i = 0; i < idString.size(); ++i)
        {
            if (idString[i] == ' ') // Ensure no spaces in the ID
            {
                return false;
            }
        }
        return true;
    }
};
// Instantiate an object of the ErrorHandling class
ErrorHandling errorHandler;
// User structure to store user-related data
struct User
{
    string username;                                // Username of the user
    string password;                                // Password of the user
    string email;                                   // Email address of the user
    unordered_map<int, int> tickets;                // Map of event ID to the number of tickets purchased
    unordered_map<int, int> canceledVipTickets;     // Map of canceled VIP tickets by event ID
    unordered_map<int, int> canceledRegularTickets; // Map of canceled regular tickets by event ID
};
// Event structure to store event-related data
struct Event
{
    int eventId;                                                      // Unique identifier for the event
    string eventName;                                                 // Name of the event
    int totalSeats;                                                   // Total number of seats available
    int vipSeats;                                                     // Number of VIP seats available
    int regularSeats;                                                 // Number of regular seats available
    priority_queue<int> vipSeatsQueue;                                // Priority queue for VIP seats (highest priority first)
    priority_queue<int, vector<int>, greater<int>> regularSeatsQueue; // Min-heap for regular seats
                                                                      // Default constructor to initialize an empty event
    Event() : eventId(0), eventName(""), totalSeats(0), vipSeats(0), regularSeats(0) {}
    // Parameterized constructor to initialize an event with specific details
    Event(int id, const string &name, int total, int vip, int regular)
        : eventId(id), eventName(name), totalSeats(total), vipSeats(vip), regularSeats(regular) {}
};
// Global data structures to store users, events, and admins
unordered_map<string, User> users;  // Map of username to user data
unordered_map<int, Event> events;   // Map of event ID to event data
unordered_map<string, bool> admins; // Map of admin username to admin status (true/false)
// Function prototypes (to be implemented elsewhere)
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
// Utility function to convert a string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str; // Create a copy to modify
    for (int i = 0; i < lowerStr.length(); i++)
    {
        lowerStr[i] = tolower(lowerStr[i]); // Convert each character to lowercase
    }
    return lowerStr;
}
// Function to display all user data (except admins)
void displayAllUserData()
{
    if (users.empty()) // Check if there are no users
    {
        cout << "No users available.\n";
        return;
    }
    cout << "User Data:\n";            // Header for user data
    for (const auto &userPair : users) // Loop through all users
    {
        const string &username = userPair.first; // Extract the username
        const User &user = userPair.second;      // Extract the user data
        // Skip displaying data for admin users
        if (admins.find(username) != admins.end() && admins.at(username)) // Skip admins
        {
            continue;
        }

        cout << "Username: " << user.username << "\nEmail: " << user.email << "\nTickets:\n";
        // Check if the user has no tickets
        if (user.tickets.empty())
        {
            cout << "  No tickets bought.\n";
        }
        else
        {
            // Loop through the user's tickets
            for (const auto &ticketPair : user.tickets)
            {
                int eventID = ticketPair.first;
                // Extract the total tickets for the event
                int totalTickets = ticketPair.second;

                int vipTickets = 0;
                int regularTickets = 0;
                // Get event details using the event ID
                const Event &event = events.at(eventID);
                // Check if any VIP tickets were canceled and update the count
                if (user.canceledVipTickets.find(eventID) != user.canceledVipTickets.end())
                {
                    vipTickets = user.canceledVipTickets.at(eventID);
                }
                // Check if any regular tickets were canceled and update the count
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
// Function to update a user's profile information
void updateUserProfile(const string &username)
{
    // Check if the user exists in the system
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
            // Validate the choice
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }
        // Update username
        if (choice == "1")
        {
            while (true)
            {
                cout << "Enter new username: ";
                getline(cin, newValue);
                // Validate the new username
                if (errorHandler.nameValidation(newValue))
                {
                    break;
                }
                cout << "\n\tInvalid username\n\n";
            }
            // Update the username
            users[username].username = newValue;
            cout << "\n\tUsername updated successfully!\n";
        }

        else if (choice == "2")
        {
            while (true)
            {
                // Get new password
                errorHandler.passLogic(newValue, "Enter new Password: ");
                // Validate the new password
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
                // Validate the new email
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
// Function to handle user signup
void signup()
{
    string username, password, email;

    while (true)
    {

        while (true)
        {
            cout << "\nEnter Username: ";
            getline(cin, username);
            // Check if the username is valid
            if (errorHandler.nameValidation(username))
            {
                break;
            }
            else
            {
                cout << "\n\n\tInvalid Username. Please try again.\n";
            }
        }
        // Convert username to lowercase for case-insensitive comparison
        string lowerUsername = toLowerCase(username);
        // Check if the username already exists
        bool usernameExists = false;
        for (const auto &user : users)
        {
            // Compare usernames ignoring case
            if (toLowerCase(user.first) == lowerUsername)
            {
                // Mark username as existing
                usernameExists = true;
                break;
            }
        }

        if (usernameExists)
        {
            cout << "\n\tUsername already exists. Please try again.\n";
            continue;
        }
        // Redundant validation check (could be removed)
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
        // Get and validate the password
        errorHandler.passLogic(password, "Enter Password: ");
        // Check if the password meets criteria
        if (errorHandler.passwordValidation(password))
        {
            break;
        }
    }

    while (true)
    {
        cout << "\nEnter Valid Email: ";
        getline(cin, email);
        // Convert email to lowercase for case-insensitive comparison
        string lowerEmail = toLowerCase(email);

        bool emailExists = false;
        for (const auto &user : users)
        {
            // Compare emails ignoring case
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
        // Validate email format
        if (errorHandler.emailValidation(email))
        {
            break;
        }
        else
        {
            cout << "\n\tInvalid Email. Please try again.\n";
        }
    }
    // Create a new user object and add it to the users map
    User newUser = {username, password, email, {}}; // Initialize a new user
    // Add the user to the map using the username as the key
    users[username] = newUser;

    cout << "\n\tSignup Successfully!";
}
// Function to handle user login
void login()
{
    string username, password;
    while (true)
    {
        cout << "\nEnter Username: ";
        getline(cin, username);
        // Validate username format
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
            // Call a function to display the user menu (assumed to exist)
            displayUserMenu(user.first);
            break;
        }
    }

    if (!userFound)
    {
        cout << "\n\n\tInvalid username or password!\n";
    }
}
// Function to handle admin login process
bool adminLogin(string &loggedInAdmin)
{
    string username, password;
    while (true)
    {
        cout << "\nEnter Admin Username: ";
        getline(cin, username);
        // Validate username using error handling utility
        if (errorHandler.nameValidation(username))
        {
            break;
        }
        else
        {
            cout << "\n\n\tInvalid Username. Please try again.\n";
        }
    }
    // Prompt for admin password with masking
    errorHandler.passLogic(password, "Enter Admin Password: ");

    // Check if username exists in the admins map
    if (admins.find(username) == admins.end())
    {
        cout << "\n\n\tAdmin not found.\n";
        return false;
    }
    // Validate password against the stored password
    if (users[username].password != password)
    {
        cout << "Incorrect password.\n";
        return false;
    }
    // Successful login; assign logged-in admin username
    loggedInAdmin = username;
    return true;
}
// Function to update the password of an existing admin
void updateAdminPassword(const string &adminUsername)
{
    // Ensure the admin username exists in the admins map
    if (admins.find(adminUsername) == admins.end())
    {
        cout << "You must be an admin to access this feature.\n";
        return;
    }

    string newPassword;
    while (true)
    {
        errorHandler.passLogic(newPassword, "Enter new Password: ");
        // Validate the new password using error handling utility
        if (errorHandler.passwordValidation(newPassword))
            break;
    }
    // Update the password in the users map
    users[adminUsername].password = newPassword;
    cout << "\n\n\tPassword updated successfully!\n";
}
// Function to add a new admin user
void addNewAdmin()
{
    string newAdminUsername, password, email;
    while (true)
    {
        cout << "\nEnter new admin's Username: ";
        getline(cin, newAdminUsername);
        // Ensure username is valid and does not already exist
        if (errorHandler.nameValidation(newAdminUsername) && admins.find(newAdminUsername) == admins.end())
            break;
        cout << "\n\tInvalid Username. Please try again.\n";
    }

    while (true)
    {
        errorHandler.passLogic(password, "Enter new admin's Password: ");
        // Validate the password
        if (errorHandler.passwordValidation(password))
            break;
    }

    while (true)
    {
        cout << "\nEnter new admin's Valid Email: ";
        getline(cin, email);
        // Validate the email address
        if (errorHandler.emailValidation(email))
            break;
        cout << "\n\tInvalid Email. Please try again.\n";
    }
    // Create a new User object for the admin
    User newAdmin = {newAdminUsername, password, email, {}};
    // Add the new admin to the users and admins maps
    users[newAdminUsername] = newAdmin;
    admins[newAdminUsername] = true;
    cout << "\n\tNew admin added successfully!\n";
}
// Function to remove an existing admin user
// Function to remove an existing admin user
void removeAdmin()
{
    string targetAdminUsername;
    cout << "\nEnter the username of the admin to remove: ";
    getline(cin, targetAdminUsername);

    if (admins.size() <= 1)
    {
        cout << "Atleast one admin exists!" << endl;
        return;
    }
    // Check if the username exists in the admins map
    if (admins.find(targetAdminUsername) != admins.end())
    {
        // Remove the admin from the admins and users maps
        admins.erase(targetAdminUsername);
        users.erase(targetAdminUsername);
        cout << "\n\tAdmin removed successfully!\n";
    }
    else
    {
        cout << "\n\tAdmin not found or cannot be removed.\n";
    }
}
// Function to display and manage the admin panel for updating admin-specific settings
void updateadminPanel(const string &adminUsername)
{
    // Check if the user has admin access
    if (admins.find(adminUsername) == admins.end())
    {
        cout << "\n\tAccess Denied. Only admins can access this panel.\n";
        return;
    }

    while (true)
    {
        cout << "\n\t\t ________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Update own Password" << setw(16) << "|\n";
        cout << "\t\t| [2]   |     Add New Admin" << setw(22) << "|\n";
        cout << "\t\t| [3]   |     Remove an Admin" << setw(20) << "|\n";
        cout << "\t\t| [4]   |     Exit Admin Panel" << setw(19) << "|\n";
        cout << "\t\t|__|_____________|\n";

        string option;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, option);
            // Validate the menu choice using the error handling utility
            if (errorHandler.menuChoice(option))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (option == "1")
        {
            // Update the admin's own password
            updateAdminPassword(adminUsername);
        }
        else if (option == "2")
        {
            // Add a new admin to the system
            addNewAdmin();
        }
        else if (option == "3")
        {
            // Remove an existing admin
            removeAdmin();
        }
        else if (option == "4")
        {
            cout << "\nExiting Update Admin Panel...\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid choice. Please try again.\n";
        }
    }
}
// Function to display the main admin menu and handle admin functionalities
void displayAdminMenu()
{
    string loggedInAdmin;
    // Attempt to log in as an admin
    if (!adminLogin(loggedInAdmin))
    {
        return;
    }
    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Add Event" << setw(26) << "|\n";
        cout << "\t\t| [2]   |     Update Event" << setw(23) << "|\n";
        cout << "\t\t| [3]   |     Manage Tickets" << setw(21) << "|\n";
        cout << "\t\t| [4]   |     View All Users" << setw(21) << "|\n";
        cout << "\t\t| [5]   |     View All Events" << setw(20) << "|\n";
        cout << "\t\t| [6]   |     View All Users Data" << setw(16) << "|\n";
        cout << "\t\t| [7]   |     Manage Admin" << setw(23) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        string option;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, option);
            // Validate the menu choice using the error handling utility
            if (errorHandler.menuChoice(option))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (option == "1")
        {
            // Add a new event
            addEvent();
        }
        else if (option == "2")
        {
            // Update an existing event
            updateEvent();
        }
        else if (option == "3")
        {
            // Manage event tickets
            manageTickets();
        }
        else if (option == "4")
        {
            // View all registered users
            viewAllUsers();
        }
        else if (option == "5")
        {
            // View all available events
            displayAllEvents();
        }
        else if (option == "6")
        {
            // View detailed user data
            displayAllUserData();
        }
        else if (option == "7")
        {
            // Access the admin management panel
            updateadminPanel(loggedInAdmin);
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
// Function to search for events based on the event name
void searchEvents()
{
    // Check if there are any events in the system
    if (events.empty())
    {
        cout << "No events available to search.\n";
        return;
    }

    string query;
    cout << "Enter event name to search: ";
    getline(cin, query);

    bool found = false;
    cout << "Search results:\n";
    // Iterate through all events to find matches
    for (const auto &event : events)
    {
        int eventID = event.first;
        const Event &e = event.second;
        // Check if the event name contains the query string
        if (e.eventName.find(query) != string::npos)
        {
            cout << "Event ID: " << eventID << ", Event Name: " << e.eventName << '\n';
            // Display seat availability for the event
            if (e.vipSeatsQueue.empty() && e.regularSeatsQueue.empty())
            {
                cout << "No seats left.\n";
            }
            else
            {
                cout << "VIP Seats Available: " << e.vipSeatsQueue.size() << ", Regular Seats Available: " << e.regularSeatsQueue.size() << "\n";
            }
            found = true;
        }
    }

    if (!found)
    {
        cout << "No events found matching the query.\n";
    }
}
// Function to display all available events
void displayAllEvents()
{
    // Check if there are any events to display
    if (events.empty())
    {
        cout << "No events available.\n";
        return;
    }

    cout << "All events:\n";
    // Iterate through all events and display their details
    for (const auto &event : events)
    {
        int eventID = event.first;
        const Event &e = event.second;
        cout << "Event ID: " << eventID
             << ", Event Name: " << e.eventName
             << ", VIP Seats Available: " << e.vipSeatsQueue.size()
             << ", Regular Seats Available: " << e.regularSeatsQueue.size() << '\n';
    }
}
// Function to view all registered users in the system
void viewAllUsers()
{
    // Check if there are any users in the system
    if (users.empty())
    {
        cout << "\nNo users are registered yet.\n";
    }
    else
    {
        cout << "\nRegistered Users:\n";
        // Iterate through all users and display their information
        for (const auto &user : users)
        {
            cout << "Username: " << user.second.username << ", Email: " << user.second.email << "\n";
        }
    }
}
// Function to buy tickets for an event
void buyTickets(const string &username)
{
    // Check if there are any events available for booking
    if (events.empty())
    {
        cout << "No events available.\n";
        return;
    }

    string eventIDStr;
    int eventID;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);
    // Validate the event ID input and check if the event exists
    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }
    // Convert the valid string to an integer
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
        // Handle ticket booking based on the user's choice
        if (choice == "1" || choice == "2")
        {
            string numberOfTicketsStr;
            int numberOfTickets;

            cout << "Enter number of tickets: ";
            getline(cin, numberOfTicketsStr);
            // Validate the ticket quantity input
            if (!errorHandler.idValidation(numberOfTicketsStr))
            {
                cout << "Number of tickets must be a positive integer.\n";
                continue;
            }
            numberOfTickets = stoi(numberOfTicketsStr);
            // Handle VIP ticket booking
            if (choice == "1")
            {
                if (event.vipSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough VIP seats available. Total VIP seats left: " << event.vipSeatsQueue.size() << ".\n";
                    return;
                }
                // Book the requested number of VIP tickets
                while (vipTicketsBooked < numberOfTickets && !event.vipSeatsQueue.empty())
                {
                    event.vipSeatsQueue.pop();
                    vipTicketsBooked++;
                }

                users[username].canceledVipTickets[eventID] += vipTicketsBooked;
            }
            // Handle Regular ticket booking
            else if (choice == "2")
            {
                if (event.regularSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough Regular seats available. Total Regular seats left: " << event.regularSeatsQueue.size() << ".\n";
                    return;
                }
                // Book the requested number of Regular tickets
                while (regularTicketsBooked < numberOfTickets && !event.regularSeatsQueue.empty())
                {
                    event.regularSeatsQueue.pop();
                    regularTicketsBooked++;
                }

                users[username].canceledRegularTickets[eventID] += regularTicketsBooked;
            }
            // Confirm successful booking
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
// Function to allow a user to cancel tickets for a specific event
void cancelTicket(const string &username)
{
    string eventIDStr;
    int eventID;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);
    // Validate the event ID and ensure the event exists
    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }
    // Convert the valid event ID string to an integer
    eventID = stoi(eventIDStr);
    // Check if the user exists in the system
    if (users.find(username) == users.end())
    {
        cout << "User not found.\n";
        return;
    }
    // Check if the user has tickets for the specified event
    if (users[username].tickets.find(eventID) == users[username].tickets.end())
    {
        cout << "You have not bought any tickets for this event.\n";
        return;
    }
    // Reference to the event object
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
            // Retrieve the number of tickets the user has already canceled for this event
            int vipTicketsBooked = users[username].canceledVipTickets[eventID];
            int regularTicketsBooked = users[username].canceledRegularTickets[eventID];
            // Check if the user has any tickets of the selected type
            if ((choice == "1" && vipTicketsBooked == 0) || (choice == "2" && regularTicketsBooked == 0))
            {
                cout << "You do not have any " << (choice == "1" ? "VIP" : "Regular") << " tickets for this event.\n";
                continue;
            }

            string numberOfTicketsStr;
            int numberOfTickets;

            cout << "Enter number of tickets to cancel: ";
            getline(cin, numberOfTicketsStr);
            // Validate the input for ticket quantity
            if (!errorHandler.idValidation(numberOfTicketsStr))
            {
                cout << "Number of tickets must be a positive integer.\n";
                continue;
            }
            // Convert the valid string to an integer
            numberOfTickets = stoi(numberOfTicketsStr);
            // Check if the user has enough tickets of the selected type to cancel
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
            // Process VIP ticket cancellation
            if (choice == "1")
            {
                int seatsToCancel = min(numberOfTickets, vipTicketsBooked);
                // Add canceled VIP seats back to the queue
                for (int i = 0; i < seatsToCancel; ++i)
                {
                    event.vipSeatsQueue.push(0);
                }
                // Update user and event data
                users[username].canceledVipTickets[eventID] -= seatsToCancel;
                users[username].tickets[eventID] -= seatsToCancel;
                // Remove the event from the user's ticket list if no tickets remain
                if (users[username].tickets[eventID] == 0)
                {
                    users[username].tickets.erase(eventID);
                }

                cout << "VIP tickets cancelled successfully!\n";
                return;
            }
            // Process Regular ticket cancellation
            else if (choice == "2")
            {
                int seatsToCancel = min(numberOfTickets, regularTicketsBooked);
                // Add canceled Regular seats back to the queue
                for (int i = 0; i < seatsToCancel; ++i)
                {
                    event.regularSeatsQueue.push(0);
                }
                // Update user and event data
                users[username].canceledRegularTickets[eventID] -= seatsToCancel;
                users[username].tickets[eventID] -= seatsToCancel;
                // Remove the event from the user's ticket list if no tickets remain
                if (users[username].tickets[eventID] == 0)
                {
                    users[username].tickets.erase(eventID);
                }

                cout << "Regular tickets cancelled successfully!\n";
                return;
            }
        }
        else if (choice == "0") // If the user chooses to exit.
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
// Function to display all tickets purchased by the user.
void viewTickets(const string &username)
{
    if (users.find(username) == users.end()) // Checks if the user exists in the `users` map.
    {
        cout << "User not found.\n";
        return;
    }

    const User &user = users[username]; // Retrieve the user object.
// Check if the user has no tickets.
    if (user.tickets.empty())
    {
        cout << "You have not purchased any tickets.\n";
        return;
    }

    cout << "Your tickets:\n";
    for (const auto &ticket : user.tickets)   // Iterate through all tickets the user owns
    {
        int eventID = ticket.first; // Get event ID
        int totalTickets = ticket.second; // Get total number of tickets for this event.
        if (events.find(eventID) == events.end())  // If the event is deleted, notify the user.
        {
            cout << "Event ID " << eventID << " has been deleted and no longer exists.\n";
            continue; // Skip this event
        }

        int vipTickets = 0;
        int regularTickets = 0;
 // Check if VIP tickets were canceled for this event.
        if (user.canceledVipTickets.find(eventID) != user.canceledVipTickets.end())
        {
            vipTickets = user.canceledVipTickets.at(eventID);
        }
 // Check if Regular tickets were canceled for this event.
        if (user.canceledRegularTickets.find(eventID) != user.canceledRegularTickets.end())
        {
            regularTickets = user.canceledRegularTickets.at(eventID);
        }
// Display ticket details for the event
        cout << "\nEvent ID: " << eventID
             << ",\tEvent Name: " << events[eventID].eventName
             << ",\tVIP Tickets: " << vipTickets
             << ",\tRegular Tickets: " << regularTickets << '\n';
    }
}
// Function to add a new event.
void addEvent()
{
    string eventIDStr, totalSeatsStr, vipSeatsStr;
    int eventID;
    string eventName;
    int totalSeats, vipSeats, regularSeats;

    while (true) // Loop to validate event ID.
    {
        cout << "Enter event ID: ";
        getline(cin, eventIDStr);
        if (errorHandler.idValidation(eventIDStr))  // Validate event ID
        {
            eventID = stoi(eventIDStr); // Convert to integer if valid.
            break;
        }
        cout << "\n\tInvalid event ID. Please enter a numeric event ID with no spaces.\n";
    }
// Check if the event ID already exists.
    if (events.find(eventID) != events.end())
    {
        cout << "\n\tEvent ID already exists. Please choose a different event ID.\n";
        return;
    }

    while (true) // Loop to validate event name.
    {
        cout << "\nEnter Event Name: ";
        getline(cin, eventName);
        if (errorHandler.nameValidation(eventName))
        {
            break;
        }
        else
        {
            cout << "\n\n\tInvalid Event Name. Please try again.\n";
        }
    }
    while (true) // Loop to validate total seats.
    {
        cout << "Enter total seats: ";
        getline(cin, totalSeatsStr);

        if (errorHandler.idValidation(totalSeatsStr)) // Validate total seats.
        {
            totalSeats = stoi(totalSeatsStr); // Convert to integer if valid.

            if (totalSeats > 0) // Check if total seats are positive.
            {
                break;
            }
            else
            {
                cout << "Total seats must be a positive number. Please enter again.\n";
                continue;
            }
        }
        else
        {
            cout << "Invalid number of total seats. Please enter a numeric value with no spaces.\n";
            continue;
        }
    }

    while (true) // Loop to validate VIP seats.
    {
        cout << "Enter number of VIP seats: ";
        getline(cin, vipSeatsStr);

        if (errorHandler.idValidation(vipSeatsStr)) // Validate VIP seats.
        {
            vipSeats = stoi(vipSeatsStr); // Convert to integer if valid.

            if (vipSeats >= 0 && vipSeats <= totalSeats) // Check if within range.
            {
                regularSeats = totalSeats - vipSeats;  // Calculate regular seats.
                break;
            }
            else
            {
                cout << "VIP seats must be between 0 and the total number of seats. Please enter again.\n";
                continue;
            }
        }
        else
        {
            cout << "Invalid number of VIP seats. Please enter a numeric value with no spaces.\n";
            continue;
        }
    }

    regularSeats = totalSeats - vipSeats; // Create a new Event object with the provided details.
    Event newEvent = {eventID, eventName, totalSeats, vipSeats, regularSeats}; 
    for (int i = 1; i <= vipSeats; ++i) // Populate VIP seats queue.
    {
        newEvent.vipSeatsQueue.push(i);
    }
    for (int i = 1; i <= regularSeats; ++i) // Populate Regular seats queue.
    {
        newEvent.regularSeatsQueue.push(i);
    }

    events[eventID] = newEvent; // Add the new event to the `events` map.

    cout << "Event added successfully!\n";
}
// Function to update or delete an event
void updateEvent()
{
    string eventIDStr;
    int eventID;
    cout << "Enter event ID to update or delete: ";
    getline(cin, eventIDStr);
// Validate the event ID and check if it exists in the events map
    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);
    while (true) // Start a loop for displaying choices
    {
        cout << "\n\t\t ________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Update the event" << setw(19) << "|\n";
        cout << "\t\t| [2]   |     Delete the event" << setw(19) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|__|_____________|\n";
        string choice;
        cout << "Enter your choice: ";
        getline(cin, choice);

        if (choice == "1") // If the user chooses to update the event
        {
            string newEventName;
            while (true)
            {
                cout << "\nEnter new Event Name: ";
                getline(cin, newEventName);
                if (errorHandler.nameValidation(newEventName))  // Validate the name
                {
                    break;
                }
                else
                {
                    cout << "\n\n\tInvalid Event Name. Please try again.\n";
                }
            }

            events[eventID].eventName = newEventName;  // Update the event name in the events map
            cout << "Event updated successfully!\n";
        }
        else if (choice == "2")
        {
            for (auto &userPair : users)  // Loop through all users and remove this event from their tickets
            {
                User &user = userPair.second; 
                user.tickets.erase(eventID); // Remove the event from user's ticket list
                user.canceledVipTickets.erase(eventID);  // Remove the event from canceled VIP tickets
                user.canceledRegularTickets.erase(eventID);// Remove the event from canceled regular tickets
            }

            events.erase(eventID); // Remove the event from the events map 
            cout << "Event deleted successfully!\n";
            break;
        }
        else if (choice == "0")
        {
            cout << "\nExiting...\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
// Function to manage tickets for a specific event
void manageTickets()
{
    string eventIDStr;
    int eventID;

    while (true)
    {
        cout << "Enter event ID to manage tickets: ";
        getline(cin, eventIDStr);

        if (errorHandler.idValidation(eventIDStr)) // Validate the event ID
        {
            eventID = stoi(eventIDStr); // Convert the event ID string to integer
            break;
        }
        else
        {
            cout << "Invalid event ID. Please enter a numeric event ID with no spaces.\n";
            continue;
        }
    }
 // Check if the event exists in the events map
    if (events.find(eventID) == events.end())
    {
        cout << "Event not found.\n";
        return;
    }

    Event &event = events[eventID]; // Reference to the event object from the events map
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

            if (errorHandler.idValidation(seatsStr)) // Validate the number of seats
            {
                seats = stoi(seatsStr); // Convert string to integer
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
                    events[eventID].vipSeatsQueue.push(events[eventID].vipSeats + 1 + i);  // Add VIP seat to the queue
                }
                events[eventID].vipSeats += seats; // Update the total number of VIP seats
            }
            else if (option == "2")
            {
                for (int i = 0; i < seats; ++i)
                {
                    events[eventID].regularSeatsQueue.push(events[eventID].regularSeats + 1 + i);  // Add Regular seat to the queue
                }
                events[eventID].regularSeats += seats; // Update the total number of Regular seats
            }
            events[eventID].totalSeats += seats; // Update the total number of seats for the event
            cout << "Seats added successfully.\n";
        }
        else if (choice == "2")
        {
            if (option == "1")
            {
                if (seats <= event.vipSeatsQueue.size()) // Check if there are enough VIP seats to remove
                {
                    for (int i = 0; i < seats; ++i)
                    {
                        events[eventID].vipSeatsQueue.pop(); // Remove a VIP seat from the queue
                    }
                    events[eventID].vipSeats -= seats; //  Decrease the total number of VIP seats
                    events[eventID].totalSeats -= seats; // Decrease the total number of seats
                    cout << "VIP seats removed successfully.\n";
                }
                else
                {
                    cout << "Not enough VIP seats available to remove.\n";
                }
            }
            else if (option == "2")
            {
                if (seats <= event.regularSeatsQueue.size()) // Check if there are enough Regular seats to remove
                {
                    for (int i = 0; i < seats; ++i)
                    {
                        events[eventID].regularSeatsQueue.pop(); // Remove a Regular seat from the queue
                    }
                    events[eventID].regularSeats -= seats; // Decrease the total number of Regular seats
                    events[eventID].totalSeats -= seats; // Decrease the total number of seats
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

// Function to set up default admin users in the system
void setupDefaultAdmin()
{
    string defaultAdminUsername1 = "asad";
    string defaultAdminPassword1 = "55181";
    string defaultAdminEmail1 = "admin1@example.com";

    if (users.find(defaultAdminUsername1) == users.end()) // Check if the admin user already exists
    {
        User adminUser = {defaultAdminUsername1, defaultAdminPassword1, defaultAdminEmail1, {}}; // Create a new admin user
        users[defaultAdminUsername1] = adminUser; // Add admin user to the users map
        admins[defaultAdminUsername1] = true; // Mark this user as an admin
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
    string defaultAdminPassword3 = "53759";
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
    setupDefaultAdmin(); // Set up default admin users
    while (true) // Infinite loop to keep the system running until the user exits
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
            if (errorHandler.menuChoice(choice)) // Validate the input choice
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