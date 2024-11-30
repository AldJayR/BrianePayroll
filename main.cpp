#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <conio.h>
#include <cmath>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include "colors.h"

using namespace std;

// Forward Declarations
struct Date;
struct Employee;
struct EmployeeHours;
struct EmployeeData;
struct Admin;
struct SalaryBreakdown;

// Admin instance
Admin& getAdminInstance();

// Main functions
void displayEmployee(const Employee& emp);
void displayEmployeeHours(const EmployeeHours& hours);
void addEmployee(unordered_map<string, EmployeeData>& employees);
void displayAllEmployees(const unordered_map<string, EmployeeData>& employees);
void calculateEmployeeSalary(unordered_map<string, EmployeeData>& employees);
void searchEmployees(unordered_map<string, EmployeeData>& employees);
void updateEmployee(unordered_map<string, EmployeeData>& employeesMap);
void deleteEmployee(unordered_map<string, EmployeeData>& employeesMap);

// Helper functions
SalaryBreakdown calculateSalary(const EmployeeData& data);
string getValidatedInput(const string& prompt, const string& errorMsg, function<bool(const string&)> validator);
int get_int(const string& prompt);
bool parseDate(const string& input, Date& date);
string get_password(const string prompt);
size_t hashPassword(const string& password);
string toLower(const string& str);

// File I/O
void saveToFile(const unordered_map<string, EmployeeData>& employees, const string& filename);
void loadFromFile(unordered_map<string, EmployeeData>& employees, const string& filename);

struct Date
{
    int year, month, day;

    string toString() const;
};

struct Employee
{
    string id;
    string name;
    string sex;
    string maritalStatus;
    Date birthday;
    string address;
    int dependents;
    string position;
    string department;

    void display() const;
};

struct EmployeeHours
{
    int overtimeHours;
    int holidayDays;
    int absentDays;
    int lateDays;

    void display() const;
};

struct EmployeeData
{
    Employee employee;
    EmployeeHours hours;
};

struct SalaryBreakdown
{
    double grossPay;
    double taxDeductions;
    double penalties;
    double netSalary;
};

struct Admin
{
    string username;
    size_t password;

    void displayAdminInfo() const
    {
        cout << "Admin Username: " << username << '\n';
    }

    bool login()
    {
        string inputUsername = getValidatedInput("Enter username: ",
                                          "Username cannot be empty.",
                                          [](const string& username) { return !username.empty(); });

        cout << "Enter password: ";
        string inputPassword = get_password("Enter password: ");

        if (inputUsername == username && hashPassword(inputPassword) == password)
        {
            cout << "Login successful!\n";
            return true;
        }
        else
        {
            cout << "Invalid username or password.\n";
            return false;
        }
    }

    bool confirm()
    {
        cout << "\nEnter admin password: ";
        string inputPassword = get_password("Enter admin password: ");

        if (inputPassword == "0") return false;

        return (hashPassword(inputPassword) == password);
    }

private:
    Admin() : username("defaultAdmin"), password(hashPassword("defaultPassword")) {}

    Admin(const Admin&) = delete;
    Admin& operator=(const Admin&) = delete;

    friend Admin& getAdminInstance();
};

Admin& getAdminInstance()
{
    static Admin instance;
    return instance;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    unordered_map<string, EmployeeData> employees;
    Admin& admin = getAdminInstance();
    bool isLoggedIn = false;
    string filename = "employees.txt";

    loadFromFile(employees, filename);

    do
    {
        if (admin.login())
            isLoggedIn = true;
    }
    while (!isLoggedIn);



    while (true)
    {
        system("CLS");
        cout << "\n" << CYAN << "╔═══════════════════════════════════════════╗\n";
        cout << "║         Puffy Fowlers™ Payroll            ║\n";
        cout << "╚═══════════════════════════════════════════╝" << RESET << "\n\n";

        int choice;

        cout << YELLOW << "╔═══════════════════════════════════════════╗\n";
        cout << "║  1. ➕ Add Employee                       ║\n";
        cout << "║  2. 👥 Display Employees                  ║\n";
        cout << "║  3. 💾 Calculate Employee Salary          ║\n";
        cout << "║  4. 🔍 Search Employees                   ║\n";
        cout << "║  5. 🗑️ Delete Employees                   ║\n";
        cout << "║  6. ✏️ Update Employees                   ║\n";
        cout << "║  7. 🚪 Exit                               ║\n";
        cout << "╚═══════════════════════════════════════════╝" << RESET << "\n";
        choice = get_int(GREEN + "📝 Enter your choice: " + RESET);

        switch (choice)
        {
            case 1:
            {
                addEmployee(employees);
                saveToFile(employees, filename);
                break;
            }
            case 2:
            {
                displayAllEmployees(employees);
                break;
            }
            case 3:
            {
                calculateEmployeeSalary(employees);
                saveToFile(employees, filename);
                break;
            }
            case 4:
            {
                searchEmployees(employees);
                break;
            }
            case 5:
            {
                // Add delete employee functionality here
                // deleteEmployee(employees);
                saveToFile(employees, filename);
                break;
            }
            case 6:
            {
                // update functionality
                break;
            }
            case 7:
            {
                cout << GREEN << "👋 Thank you for using Puffy Fowlers™ Payroll" << RESET << "\n";
                return 0;
            }
            default:
            {
                cout << RED << "❌ Invalid choice. Please try again." << RESET << "\n";
                break;
            }
        }
    }

    return 0;
}

string Date::toString() const
{
    stringstream ss;
    ss << year << "/" << setw(2) << setfill('0') << month
       << "/" << setw(2) << setfill('0') << day;
    return ss.str();
}

void Employee::display() const
{
    cout << "ID: " << id << '\n'
         << "Name: " << name << '\n'
         << "Sex: " << sex << '\n'
         << "Marital Status: " << maritalStatus << '\n'
         << "Birthday: " << birthday.toString() << '\n'
         << "Address: " << address << '\n'
         << "Dependents: " << dependents << '\n';
}

void EmployeeHours::display() const
{
    cout << "Overtime Hours: " << overtimeHours << '\n'
         << "Holiday Days: " << holidayDays << '\n'
         << "Absent Days: " << absentDays << '\n'
         << "Late Days: " << lateDays << '\n';
}

void addEmployee(unordered_map<string, EmployeeData>& employees)
{
    system("CLS");
    cout << "\n" << CYAN;
    cout << "╔═══════════════════════════════════════════╗\n";
    cout << "║               Add Employee                ║\n";
    cout << "╚═══════════════════════════════════════════╝\n" << RESET << "\n";
    Employee employee;

    unordered_set<string> existingIDs;
    for (const auto& entry : employees)
    {
        existingIDs.insert(entry.first);
    }

    do
    {
        employee.id = "EMP" + to_string(rand() % 1000);
    }
    while (existingIDs.find(employee.id) != existingIDs.end());

    employee.name = getValidatedInput(GREEN + "Name: " + RESET,
        RED + "Name must contain only alphabetic characters and spaces." + RESET,
        [](const string& name) { return !name.empty() && regex_match(name, regex(R"(^[a-zA-Z ]+$)")); });

    employee.sex = getValidatedInput(GREEN + "Sex (M/F): " + RESET,
        RED + "Invalid input. Please enter 'M' or 'F'." + RESET,
        [](const string& sex) { return sex == "M" || sex == "F"; });

    employee.maritalStatus = getValidatedInput(GREEN + "Marital Status: " + RESET,
        RED + "Marital Status cannot be empty." + RESET,
        [](const string& status) { return !status.empty(); });

    while (true)
    {
        string input = getValidatedInput(GREEN + "Birthday (YYYY/MM/DD): " + RESET,
            RED + "Invalid date format. Please use 'YYYY/MM/DD'." + RESET,
            [](const string& dateStr) { return regex_match(dateStr, regex(R"(^\d{4}/\d{2}/\d{2}$)")); });

        if (parseDate(input, employee.birthday))
        {
            break;
        }
        cout << RED << "Invalid date value. Please try again." << RESET << "\n";
    }

    employee.address = getValidatedInput(GREEN + "Address: " + RESET,
        RED + "Address cannot be empty." + RESET,
        [](const string& address) { return !address.empty(); });

    employee.dependents = get_int(GREEN + "Dependents: " + RESET);

    employee.position = getValidatedInput(GREEN + "Position: " + RESET,
        RED + "Invalid position. Please enter 'staff', 'supervisor', or 'manager'." + RESET,
        [](const string& position)
        {
            string lowerPosition = position;
            transform(lowerPosition.begin(), lowerPosition.end(), lowerPosition.begin(), ::tolower);
            return lowerPosition == "staff" || lowerPosition == "supervisor" || lowerPosition == "manager";
        });

    employee.department = getValidatedInput(GREEN + "Department: " + RESET,
        RED + "Department cannot be empty." + RESET,
        [](const string& department) { return !department.empty(); });

    Admin& admin = getAdminInstance();
    while (true)
    {
        if (admin.confirm())
        {
            employees[employee.id] = {employee, {0, 0, 0, 0}};
            cout << GREEN << "\nEmployee with the ID: " << employee.id << " added successfully!" << RESET << "\n";
            Sleep(1500);
            break;
        }

        cout << RED << "Invalid password. Please try again." << RESET << "\n";
    }
}

void displayAllEmployees(const unordered_map<string, EmployeeData>& employees)
{
    const int ID_WIDTH = 6;
    const int NAME_WIDTH = 20;
    const int SEX_WIDTH = 4;
    const int MARITAL_WIDTH = 16;
    const int BIRTHDAY_WIDTH = 10;
    const int ADDRESS_WIDTH = 20;
    const int DEPENDENTS_WIDTH = 10;
    const int POSITION_WIDTH = 12;
    const int DEPARTMENT_WIDTH = 13;

    system("CLS");
    cout << CYAN;
    cout << "╔════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                                   Employee Records                                                     ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n";
    cout << RESET << "\n";

    if (employees.empty())
    {
        cout << YELLOW << "No employees to display!" << RESET << "\n";
        return;
    }

    auto createLine = [](char left, char middle, char right, const vector<int>& widths)
    {
        string line(1, left);
        for (size_t i = 0; i < widths.size(); ++i)
        {
            line += string(widths[i] + 2, '-');
            line += (i < widths.size() - 1) ? middle : right;
        }
        return line;
    };

    auto formatField = [](const string& text, int width)
    {
        if (text.length() > width)
        {
            return text.substr(0, width - 3) + "...";
        }
        return text;
    };

    vector<int> widths = {ID_WIDTH, NAME_WIDTH, SEX_WIDTH, MARITAL_WIDTH,
                          BIRTHDAY_WIDTH, ADDRESS_WIDTH, DEPENDENTS_WIDTH,
                          POSITION_WIDTH, DEPARTMENT_WIDTH};

    cout << CYAN << createLine('+', '+', '+', widths) << RESET << '\n';

    cout << CYAN << "| " << setw(ID_WIDTH) << left << "ID"
         << " | " << setw(NAME_WIDTH) << left << "Name"
         << " | " << setw(SEX_WIDTH) << left << "Sex"
         << " | " << setw(MARITAL_WIDTH) << left << "Marital Status"
         << " | " << setw(BIRTHDAY_WIDTH) << left << "Birthday"
         << " | " << setw(ADDRESS_WIDTH) << left << "Address"
         << " | " << setw(DEPENDENTS_WIDTH) << left << "Dependents"
         << " | " << setw(POSITION_WIDTH) << left << "Position"
         << " | " << setw(DEPARTMENT_WIDTH) << left << "Department" << " |" << RESET << "\n";

    cout << CYAN << createLine('+', '+', '+', widths) << RESET << '\n';

    for (const auto& [_, data] : employees)
    {
        const Employee& e = data.employee;

        cout << "| " << setw(ID_WIDTH) << left << formatField(e.id, ID_WIDTH)
             << " | " << setw(NAME_WIDTH) << left << formatField(e.name, NAME_WIDTH)
             << " | " << setw(SEX_WIDTH) << left << formatField(e.sex, SEX_WIDTH)
             << " | " << setw(MARITAL_WIDTH) << left << formatField(e.maritalStatus, MARITAL_WIDTH)
             << " | " << setw(BIRTHDAY_WIDTH) << left << formatField(e.birthday.toString(), BIRTHDAY_WIDTH)
             << " | " << setw(ADDRESS_WIDTH) << left << formatField(e.address, ADDRESS_WIDTH)
             << " | " << setw(DEPENDENTS_WIDTH) << left << formatField(to_string(e.dependents), DEPENDENTS_WIDTH)
             << " | " << setw(POSITION_WIDTH) << left << formatField(e.position, POSITION_WIDTH)
             << " | " << setw(DEPARTMENT_WIDTH) << left << formatField(e.department, DEPARTMENT_WIDTH) << " |\n";
    }

    cout << CYAN << createLine('+', '+', '+', widths) << RESET << '\n';

    cout << GREEN << "\nPress any key to return..." << RESET;
    cin.get();
    system("CLS");
}

void calculateEmployeeSalary(unordered_map<string, EmployeeData>& employees)
{
    system("CLS");

    cout << "\n" << BOLD_BLUE;
    cout << "╔═══════════════════════════════════════════╗\n";
    cout << "║             Calculate Salary              ║\n";
    cout << "╚═══════════════════════════════════════════╝\n" << RESET << '\n';

    cout << "\n" << CYAN;
    cout << "╔════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                         EMPLOYEES LIST                             ║\n";
    cout << "╠════════════╦═══════════════════════════════╦═══════════════════════╣\n";
    cout << "║    ID      ║            NAME               ║       POSITION        ║\n";
    cout << "╠════════════╬═══════════════════════════════╬═══════════════════════╣\n";

    for (const auto& [id, data] : employees)
    {
        cout << "║ " << setw(11) << left << id
             << "║ " << setw(30) << left << data.employee.name
             << "║ " << setw(22) << left << data.employee.position << "║\n";
    }
    cout << "╚════════════╩═══════════════════════════════╩═══════════════════════╝\n" << RESET << "\n";

    string selectedID;
    cout << GREEN << "👤 Enter Employee ID (0 to exit): " << RESET;
    getline(cin >> ws, selectedID);

    if (selectedID == "0")
    {
        cout << YELLOW << "Returning to main menu..." << RESET;
        Sleep(2000);
        return;
    }

    auto it = employees.find(selectedID);
    if (it == employees.end())
    {
        cout << RED << "\n❌ Error: Invalid Employee ID.\n" << RESET;
        return;
    }

    EmployeeData& selectedEmployee = it->second;
    EmployeeHours& hours = selectedEmployee.hours;

    cout << "\n" << CYAN;
    cout << "╔═══════════════════════════════════════════╗\n";
    cout << "║ Time Record for: " << setw(25) << left << selectedEmployee.employee.name << "║\n";
    cout << "╠═══════════════════════════════════════════╣\n" << RESET;

    hours.overtimeHours = get_int(GREEN + "║ ⏰ Overtime Hours: " + RESET);
    hours.holidayDays = get_int(GREEN + "║ 🎉 Holiday Days: " + RESET);
    hours.absentDays = get_int(GREEN + "║ ❌ Absent Days: " + RESET);
    hours.lateDays = get_int(GREEN + "║ ⏲️  Late Days: " + RESET);

    cout << CYAN << "╚═══════════════════════════════════════════╝\n" << RESET;

    Admin& admin = getAdminInstance();
    bool isCorrect = false;

    do
    {
        if (admin.confirm())
        {
            isCorrect = true;
            break;
        }
        cout << RED << "❌ Invalid password. Please try again." << RESET << '\n';
    }
    while (!isCorrect);

    employees[selectedEmployee.employee.id] = selectedEmployee;

    try
    {
        SalaryBreakdown breakdown = calculateSalary(selectedEmployee);

        cout << "\n" << CYAN;
        cout << "╔═══════════════════════════════════════════╗\n";
        cout << "║           SALARY BREAKDOWN                ║\n";
        cout << "╠═══════════════════════════════════════════╣\n";
        cout << "║ Employee: " << setw(32) << left << selectedEmployee.employee.name << "║\n";
        cout << "╠═══════════════════════════════════════════╣\n";
        cout << "║ 💰 Gross Pay:      ₱" << setw(22) << fixed << setprecision(2) << breakdown.grossPay << "║\n";
        cout << "║ 📊 Tax Deductions: ₱" << setw(22) << breakdown.taxDeductions << "║\n";
        cout << "║ ⚠️  Penalties:     ₱" << setw(22) << abs(breakdown.penalties) << "║\n";
        cout << "╠═══════════════════════════════════════════╣\n";
        cout << "║ 💵 Net Salary:     ₱" << setw(22) << breakdown.netSalary << "║\n";
        cout << "╚═══════════════════════════════════════════╝\n" << RESET;

        cout << GREEN << "\nPress any key to return..." << RESET;
        cin.get();
        system("CLS");
    }
    catch (const invalid_argument& e)
    {
        cout << RED << "\n❌ Error calculating salary: " << e.what() << RESET << '\n';
    }
}

void searchEmployees(unordered_map<string, EmployeeData>& employeesMap)
{
    // Check if the unordered_map is empty
    if (employeesMap.empty())
    {
        cout << YELLOW << "No employees found." << RESET << endl;
        Sleep(2000);
        return;
    }

    string searchTerm;
    vector<EmployeeData> filteredEmployees;

    while (true)
    {
        system("cls");
        cout << BOLD << CYAN << "╔══════════════════════════════════════════╗" << RESET << '\n';
        cout << BOLD << CYAN << "║          Search Employees                ║" << RESET << '\n';
        cout << BOLD << CYAN << "╚══════════════════════════════════════════╝" << RESET << '\n' << '\n';

        cout << YELLOW << "Enter search term (press Esc to exit): " << RESET;
        cout << searchTerm;

        if (!searchTerm.empty())
        {
            filteredEmployees.clear();
            string lowerSearchTerm = toLower(searchTerm);

            // Loop through all employees in the unordered_map
            for (const auto& entry : employeesMap)
            {
                const EmployeeData& employeeData = entry.second;
                const Employee& employee = employeeData.employee;

                // Check if the search term matches any employee field
                if (toLower(employee.name).find(lowerSearchTerm) != string::npos ||
                    toLower(employee.position).find(lowerSearchTerm) != string::npos ||
                    toLower(employee.department).find(lowerSearchTerm) != string::npos)
                {
                    filteredEmployees.push_back(employeeData);
                }
            }

            if (!filteredEmployees.empty())
            {
                cout << "\n\n";
                cout << MAGENTA << "┌────────────────────┬────────────────────┬────────────────────┐" << RESET << endl;
                cout << MAGENTA << "│ " << WHITE << setw(18) << left << "Name"
                     << MAGENTA << " │ " << WHITE << setw(18) << left << "Position"
                     << MAGENTA << " │ " << WHITE << setw(18) << left << "Department" << MAGENTA << " │" << RESET << endl;
                cout << MAGENTA << "├────────────────────┼────────────────────┼────────────────────┤" << RESET << endl;

                for (const auto& employeeData : filteredEmployees)
                {
                    const Employee& employee = employeeData.employee;
                    cout << MAGENTA << "│ " << WHITE << setw(18) << left << employee.name
                         << MAGENTA << " │ " << WHITE << setw(18) << left << employee.position
                         << MAGENTA << " │ " << WHITE << setw(18) << left << employee.department << MAGENTA << " │" << RESET << endl;
                }

                cout << MAGENTA << "└────────────────────┴────────────────────┴────────────────────┘" << RESET << endl;
            }
            else
            {
                cout << "\n\n" << YELLOW << "No matching employees found." << RESET << endl;
            }
        }

        char ch = _getch();

        if (ch == 27) // Esc key
        {
            break;
        }
        else if (ch == 8) // Backspace
        {
            if (!searchTerm.empty())
            {
                searchTerm.pop_back();
            }
        }
        else if (isprint(ch))
        {
            searchTerm += ch;
        }
    }
}

void updateEmployee(unordered_map<string, EmployeeData>& employeesMap)
{

}

void deleteEmployee(unordered_map<string, EmployeeData>& employeesMap)
{

}

// Helpers

SalaryBreakdown calculateSalary(const EmployeeData& data)
{
    const Employee& emp = data.employee;
    const EmployeeHours& hours = data.hours;

    double basePay;
    if (toLower(emp.position) == "staff")
    {
        basePay = 90.0;
    }
    else if (toLower(emp.position) == "supervisor")
    {
        basePay = 140.0;
    }
    else if (toLower(emp.position) == "manager")
    {
        basePay = 190.0;
    }
    else
    {
        throw invalid_argument("Unknown position: " + emp.position);
    }

    // Constants for calculation
    const double overtimeRate = 20.0;
    const double holidayRate = 8.0 * basePay;
    const double latePenalty = 80.0;
    const double absentPenalty = (hours.absentDays >= 3) ? -400 : 0;
    const double insurance = (emp.dependents >= 3) ? 600.0 : 300.0;
    const double unionDues = 200.0;

    // Compute salary components
    double overtimePay = hours.overtimeHours * overtimeRate;
    double holidayPay = hours.holidayDays * holidayRate;
    double penalties = (hours.lateDays * latePenalty) + absentPenalty;

    // Gross Salary
    double grossSalary = basePay + overtimePay + holidayPay + penalties;

    // Tax rates
    const double federalTaxRate = 0.02;
    const double incomeTaxRate = 0.04;
    const double securityTaxRate = 0.03;

    // Taxes
    double federalTax = grossSalary * federalTaxRate;
    double incomeTax = grossSalary * incomeTaxRate;
    double securityTax = grossSalary * securityTaxRate;

    double totalTaxes = federalTax + incomeTax + securityTax;

    // Total deductions
    double totalDeductions = totalTaxes + insurance + unionDues;

    // Net Salary
    double netSalary = grossSalary - totalDeductions;

    // Return salary breakdown
    return SalaryBreakdown{grossSalary, totalTaxes, penalties, netSalary};
}

string getValidatedInput(const string& prompt, const string& errorMsg, function<bool(const string&)> validator)
{
    string input;
    while (true)
    {
        cout << prompt;
        getline(cin, input);
        if (validator(input))
        {
            return input;
        }
        cout << errorMsg << '\n';
    }
}

int get_int(const string& prompt)
{
    regex integer_regex("^-?[0-9]+$");
    string input;

    while (true)
    {
        cout << prompt;
        getline(cin, input);

        if (regex_match(input, integer_regex))
        {
            try
            {
                return stoi(input);
            }
            catch (out_of_range&)
            {
                cout << "Error: Number out of range. Please try again.\n";
            }
        }
        else
        {
            cout << "Invalid input. Please enter a valid integer.\n";
        }
    }
}

bool parseDate(const string& input, Date& date)
{
    regex datePattern(R"(^(\d{4})/(\d{2})/(\d{2})$)");
    smatch matches;

    if (regex_match(input, matches, datePattern))
    {
        date.year = stoi(matches[1]);
        date.month = stoi(matches[2]);
        date.day = stoi(matches[3]);

        return (date.year >= 1900 && date.year <= 2006) &&
               (date.month >= 1 && date.month <= 12) &&
               (date.day >= 1 && date.day <= 31);
    }
    return false;
}

string get_password(const string prompt)
{
    string password;
    int ch;
    bool showPassword = false;

    while (true)
    {
        ch = _getch();

        if (ch == '\r' || ch == '\n')
            break;
        else if (ch == '\t')
        {
            showPassword = !showPassword;
            cout << '\r' << prompt;
            if (showPassword)
                cout << password;
            else
                cout << string(password.size(), '*');
        }
        else if (ch == 8 || ch == 127)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            password += static_cast<char>(ch);
            if (showPassword)
                cout << static_cast<char>(ch);
            else
                cout << '*';
        }
    }

    cout << '\n';
    return password;
}

size_t hashPassword(const string& password)
{
    hash<string> hasher;
    return hasher(password);
}

string toLower(const string& str)
{
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(),
        [](unsigned char c) { return tolower(c); });
    return lower;
}

// File I/O Functions
void saveToFile(const unordered_map<string, EmployeeData>& employees, const string& filename)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "Error opening file for writing: " << filename << '\n';
        return;
    }

    for (const auto& [id, data] : employees)
    {
        const Employee& emp = data.employee;
        const EmployeeHours& hours = data.hours;

        outFile << emp.id << '\n'
                << emp.name << '\n'
                << emp.sex << '\n'
                << emp.maritalStatus << '\n'
                << emp.birthday.year << ' ' << emp.birthday.month << ' ' << emp.birthday.day << '\n'
                << emp.address << '\n'
                << emp.dependents << '\n'
                << emp.position << '\n'
                << emp.department << '\n'
                << hours.overtimeHours << ' ' << hours.holidayDays << ' '
                << hours.absentDays << ' ' << hours.lateDays << '\n';
    }

}

void loadFromFile(unordered_map<string, EmployeeData>& employees, const string& filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "Error opening file for reading: " << filename << '\n';
        return;
    }

    employees.clear();
    while (!inFile.eof())
    {
        Employee emp;
        EmployeeHours hours;

        getline(inFile, emp.id);
        if (emp.id.empty()) break;

        getline(inFile, emp.name);
        getline(inFile, emp.sex);
        getline(inFile, emp.maritalStatus);

        inFile >> emp.birthday.year >> emp.birthday.month >> emp.birthday.day;
        inFile.ignore();

        getline(inFile, emp.address);

        inFile >> emp.dependents;
        inFile.ignore();

        getline(inFile, emp.position);
        getline(inFile, emp.department);

        inFile >> hours.overtimeHours >> hours.holidayDays
               >> hours.absentDays >> hours.lateDays;
        inFile.ignore();

        employees[emp.id] = {emp, hours};
    }
}
