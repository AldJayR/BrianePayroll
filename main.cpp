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
#include <windows.h>
#include <algorithm>
#include <fstream>

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

void displayEmployee(const Employee& emp);
void displayEmployeeHours(const EmployeeHours& hours);
void addEmployee(unordered_map<string, EmployeeData>& employees);
void displayAllEmployees(const unordered_map<string, EmployeeData>& employees);
void calculateEmployeeSalary(unordered_map<string, EmployeeData>& employees);


// Helper functions
SalaryBreakdown calculateSalary(const EmployeeData& data);
string getValidatedInput(const string& prompt, const string& errorMsg, function<bool(const string&)> validator);
int get_int(const string& prompt);
bool parseDate(const string& input, Date& date);
string get_password(const string prompt);
size_t hashPassword(const string& password);
void comparePasswords(const unordered_map<string, EmployeeData>& employees, size_t password);


// File I/o
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

// Main function
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    unordered_map<string, EmployeeData> employees;

    string filename = "employees.txt";

    loadFromFile(employees, filename);

    cout << "enter password: ";
    string pass = get_password("enter password: ");

    int choice;
    do
    {
        cout << "\nMenu:\n"
             << "1. Add Employee\n"
             << "2. Display Employee\n"
             << "3. Save Data\n"
             << "4. Exit\n"
             << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline from buffer

        if (choice == 1)
        {
            addEmployee(employees);
            saveToFile(employees, filename);
        }
        else if (choice == 2)
        {
            displayAllEmployees(employees);
            /*
            cout << "Enter Employee ID to display: ";
            string id;
            getline(cin, id);
            if (employees.find(id) != employees.end())
            {
                employees[id].employee.display();
                employees[id].hours.display();
            }
            else
            {
                cout << "Employee not found.\n";
            }
            */
        }
        else if (choice == 3)
        {
            // TODO
            calculateEmployeeSalary(employees);

        }
        else if (choice != 4)
        {
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}


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
        string inputPassword = get_password("\nEnter admin password: ");

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
    Employee employee;

    unordered_set<string> existingIDs;
    for (const auto& entry : employees)
    {
        existingIDs.insert(entry.first);
    }

    do
    {
        employee.id = "EMP" + to_string(rand() % 1000000);
    }
    while (existingIDs.find(employee.id) != existingIDs.end());


    employee.name = getValidatedInput("Name: ",
        "Name must contain only alphabetic characters and spaces.",
        [](const string& name) { return !name.empty() && regex_match(name, regex(R"(^[a-zA-Z ]+$)")); });

    employee.sex = getValidatedInput("Sex (M/F): ",
        "Invalid input. Please enter 'M' or 'F'.",
        [](const string& sex) { return sex == "M" || sex == "F"; });

    employee.maritalStatus = getValidatedInput("Marital Status: ",
        "Marital Status cannot be empty.",
        [](const string& status) { return !status.empty(); });

    while (true)
    {
        string input = getValidatedInput("Birthday (YYYY/MM/DD): ",
            "Invalid date format. Please use 'YYYY/MM/DD'.",
            [](const string& dateStr) { return regex_match(dateStr, regex(R"(^\d{4}/\d{2}/\d{2}$)")); });

        if (parseDate(input, employee.birthday))
        {
            break;
        }
        cout << "Invalid date value. Please try again.\n";
    }

    employee.address = getValidatedInput("Address: ",
        "Address cannot be empty.",
        [](const string& address) { return !address.empty(); });

    employee.dependents = get_int("Dependents: ");

    employee.position = getValidatedInput("Position: ",
        "Invalid position. Please enter 'staff', 'supervisor', or 'manager'.",
        [](const string& position)
        {
            string lowerPosition = position;
            transform(lowerPosition.begin(), lowerPosition.end(), lowerPosition.begin(), ::tolower);
            return lowerPosition == "staff" || lowerPosition == "supervisor" || lowerPosition == "manager";
        });

    employee.department = getValidatedInput("Department: ",
        "Department cannot be empty.",
        [](const string& department) { return !department.empty(); });

    Admin& admin = getAdminInstance();
    while (true)
    {
        if (admin.confirm())
        {
            employees[employee.id] = {employee, {0, 0, 0, 0}};
            cout << "\nEmployee with the ID: " << employee.id << " added successfully!\n";
            break;
        }

        cout << "Invalid password. Please try again.\n";
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
    cout << "╔════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                                   Employee Records                                                     ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n";

    if (employees.empty())
    {
        cout << "No employees to display!\n";
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

    cout << createLine('+', '+', '+', widths) << '\n';

    cout << "| " << setw(ID_WIDTH) << left << "ID"
         << " | " << setw(NAME_WIDTH) << left << "Name"
         << " | " << setw(SEX_WIDTH) << left << "Sex"
         << " | " << setw(MARITAL_WIDTH) << left << "Marital Status"
         << " | " << setw(BIRTHDAY_WIDTH) << left << "Birthday"
         << " | " << setw(ADDRESS_WIDTH) << left << "Address"
         << " | " << setw(DEPENDENTS_WIDTH) << left << "Dependents"
         << " | " << setw(POSITION_WIDTH) << left << "Position"
         << " | " << setw(DEPARTMENT_WIDTH) << left << "Department" << " |\n";

    cout << createLine('+', '+', '+', widths) << '\n';

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

    cout << createLine('+', '+', '+', widths) << '\n';
}

void calculateEmployeeSalary(unordered_map<string, EmployeeData>& employees)
{
    // Display employees list
    cout << "Employees List:\n";
    for (const auto& [id, data] : employees)
    {
        cout << "ID: " << id << "| Name: " << data.employee.name << " | Position: " << data.employee.position << '\n';
    }

    string selectedID;
    cout << "Enter Employee ID: ";
    getline(cin >> ws, selectedID); // Skips any leading whitespace before reading the ID

    // Find selected employee by ID
    auto it = employees.find(selectedID);
    if (it == employees.end())
    {
        cerr << "Error: Invalid Employee ID.\n";
        return;
    }

    EmployeeData& selectedEmployee = it->second; // Get reference to selected employee's data
    EmployeeHours& hours = selectedEmployee.hours;

    // Get employee details
    cout << "\nEnter details for " << selectedEmployee.employee.name << ":\n";
    hours.overtimeHours = get_int("Overtime Hours: ");
    hours.holidayDays = get_int("Holiday Days: ");
    hours.absentDays = get_int("Absent Days: ");
    hours.lateDays = get_int("Late Days: ");

    Admin& admin = getAdminInstance();

        // Admin confirmation
        if (admin.confirm())
        {
            // Here you should likely update or save the employee data,
            // but ensure 'employee.id' and the object being assigned are correct
            employees[selectedEmployee.employee.id] = selectedEmployee;
            return;
        }

        cout << "Invalid password. Please try again.\n";


    // Calculate salary breakdown
    try
    {
        SalaryBreakdown breakdown = calculateSalary(selectedEmployee);

        // Display salary breakdown
        cout << "\nSalary Breakdown for " << selectedEmployee.employee.name << ":\n";
        cout << "Gross Pay: P" << fixed << setprecision(2) << breakdown.grossPay << '\n';
        cout << "Tax Deductions: P" << breakdown.taxDeductions << '\n';
        cout << "Penalties: P" << breakdown.penalties << '\n';
        cout << "Net Salary: P" << breakdown.netSalary << '\n';
    }
    catch (const invalid_argument& e)
    {
        cerr << "Error calculating salary: " << e.what() << '\n';
    }
}
// Helpers

SalaryBreakdown calculateSalary(const EmployeeData& data)
{
    const Employee& emp = data.employee;
    const EmployeeHours& hours = data.hours;

    double basePay;
    if (emp.position == "Staff")
    {
        basePay = 90.0;
    }
    else if (emp.position == "Supervisor")
    {
        basePay = 140.0;
    }
    else if (emp.position == "Manager")
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
