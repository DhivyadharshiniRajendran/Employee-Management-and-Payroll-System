#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
using namespace std;

// ================= ABSTRACT BASE CLASSES =================

// Root abstract class for all entities
class Entity {
public:
    virtual string getEntityType() = 0;
    virtual void display() = 0;
    virtual ~Entity() {}
};

// Abstract class for anything with an ID
class Identifiable {
protected:
    string id;
public:
    Identifiable(string i) : id(i) {}
    virtual string getID() { return id; }
};

// Abstract class for payable entities
class Payable {
public:
    virtual float calculatePay() = 0;
    virtual void generatePaySlip() = 0;
    virtual ~Payable() {}
};

// Abstract class for taxable entities
class Taxable {
protected:
    const float TAX_RATE = 0.10; // 10% tax
public:
    virtual float calculateTax(float amount) {
        return amount * TAX_RATE;
    }
    virtual ~Taxable() {}
};

// ================= PERFORMANCE TRACKING =================
class PerformanceRecord {
private:
    int rating; // 1-5
    string review;
    string date;
    string reviewedBy;

public:
    PerformanceRecord(int r, string rev, string d, string by)
        : rating(r), review(rev), date(d), reviewedBy(by) {}

    void display() const {
        cout << "   Date: " << date << " | Rating: " << rating << "/5 | By: " << reviewedBy << "\n";
        cout << "   Review: " << review << "\n";
    }

    int getRating() const { return rating; }
};

// ================= PERSON HIERARCHY =================

class Person : public Entity, public Identifiable {
protected:
    string name;
    int age;
    string address;

public:
    Person(string id, string n, int a, string addr)
        : Identifiable(id), name(n), age(a), address(addr) {}

    string getEntityType() override {
        return "Person";
    }

    void display() override {
        cout << "ID: " << id << " | Name: " << name
             << " | Age: " << age << endl;
    }

    string getName() { return name; }
};

// ================= EMPLOYEE HIERARCHY =================

// Abstract Employee class
class Employee : public Person, public Payable, public Taxable {
protected:
    string department;
    string joinDate;
    int experienceYears;
    vector<PerformanceRecord> performanceHistory;
    int leaveBalance;
    int leavesTaken;

public:
    Employee(string id, string n, int a, string addr,
             string dept, string date, int exp)
        : Person(id, n, a, addr), department(dept),
          joinDate(date), experienceYears(exp),
          leaveBalance(20), leavesTaken(0) {}

    string getEntityType() override {
        return "Employee";
    }

    virtual string getEmployeeType() = 0;
    virtual float getBaseSalary() = 0;

    void display() override {
        cout << "\n--- " << getEmployeeType() << " ---\n";
        cout << "ID         : " << id << endl;
        cout << "Name       : " << name << endl;
        cout << "Age        : " << age << endl;
        cout << "Department : " << department << endl;
        cout << "Experience : " << experienceYears << " years\n";
        cout << "Join Date  : " << joinDate << endl;
        cout << "Leaves     : " << leavesTaken << " taken, "
             << leaveBalance << " remaining\n";
    }

    void generatePaySlip() override {
        float gross = calculatePay();
        float tax = calculateTax(gross);
        float net = gross - tax;

        cout << "\n========================================\n";
        cout << "          PAYSLIP - " << name << endl;
        cout << "========================================\n";
        cout << "Employee ID    : " << id << endl;
        cout << "Type           : " << getEmployeeType() << endl;
        cout << "Department     : " << department << endl;
        cout << "----------------------------------------\n";
        cout << fixed << setprecision(2);
        cout << "Gross Salary   : $" << gross << endl;
        cout << "Tax (10%)      : $" << tax << endl;
        cout << "----------------------------------------\n";
        cout << "Net Salary     : $" << net << endl;
        cout << "========================================\n";
    }

    void addPerformanceReview(int rating, string review, string date, string by) {
        performanceHistory.emplace_back(rating, review, date, by);
    }

    void showPerformanceHistory() {
        cout << "\n--- Performance History for " << name << " ---\n";
        if (performanceHistory.empty()) {
            cout << "No performance reviews yet.\n";
        } else {
            for (const auto& record : performanceHistory) {
                record.display();
                cout << "---\n";
            }
        }
    }

    float getAverageRating() {
        if (performanceHistory.empty()) return 0;
        float sum = 0;
        for (const auto& record : performanceHistory) {
            sum += record.getRating();
        }
        return sum / performanceHistory.size();
    }

    bool applyLeave(int days) {
        if (days <= leaveBalance) {
            leaveBalance -= days;
            leavesTaken += days;
            return true;
        }
        return false;
    }

    string getDepartment() { return department; }
    int getExperience() { return experienceYears; }
    int getLeavesTaken() { return leavesTaken; }
};

// ================= FULL-TIME EMPLOYEES =================

class FullTimeEmployee : public Employee {
protected:
    float monthlySalary;
    float bonus;

public:
    FullTimeEmployee(string id, string n, int a, string addr,
                     string dept, string date, int exp, float salary)
        : Employee(id, n, a, addr, dept, date, exp),
          monthlySalary(salary), bonus(0) {}

    float getBaseSalary() override {
        return monthlySalary;
    }

    void setBonus(float b) {
        bonus = b;
    }

    float calculatePay() override {
        return monthlySalary + bonus;
    }

    string getEmployeeType() override {
        return "Full-Time Employee";
    }

    void giveRaise(float percentage) {
        monthlySalary += monthlySalary * (percentage / 100);
        cout << name << " received a " << percentage << "% raise!\n";
        cout << "New salary: $" << monthlySalary << endl;
    }
};

// Manager - inherits from FullTimeEmployee
class Manager : public FullTimeEmployee {
private:
    int teamSize;
    float teamBonus;

public:
    Manager(string id, string n, int a, string addr,
            string dept, string date, int exp, float salary, int team)
        : FullTimeEmployee(id, n, a, addr, dept, date, exp, salary),
          teamSize(team), teamBonus(500 * team) {}

    float calculatePay() override {
        return monthlySalary + bonus + teamBonus;
    }

    string getEmployeeType() override {
        return "Manager";
    }

    void display() override {
        FullTimeEmployee::display();
        cout << "Team Size  : " << teamSize << " members\n";
        cout << "Team Bonus : $" << teamBonus << endl;
    }

    int getTeamSize() { return teamSize; }
};

// Developer - inherits from FullTimeEmployee
class Developer : public FullTimeEmployee {
private:
    string programmingLanguage;
    int projectsCompleted;

public:
    Developer(string id, string n, int a, string addr,
              string dept, string date, int exp, float salary, string lang, int projects)
        : FullTimeEmployee(id, n, a, addr, dept, date, exp, salary),
          programmingLanguage(lang), projectsCompleted(projects) {
        bonus = projects * 200; // Bonus per project
    }

    void completeProject() {
        projectsCompleted++;
        bonus += 200;
        cout << name << " completed a project! Total: " << projectsCompleted << "\n";
    }

    string getEmployeeType() override {
        return "Developer";
    }

    void display() override {
        FullTimeEmployee::display();
        cout << "Language   : " << programmingLanguage << endl;
        cout << "Projects   : " << projectsCompleted << endl;
        cout << "Project Bonus: $" << (projectsCompleted * 200) << endl;
    }

    string getLanguage() { return programmingLanguage; }
    int getProjects() { return projectsCompleted; }
};

// ================= PART-TIME EMPLOYEES =================

class PartTimeEmployee : public Employee {
protected:
    float hourlyRate;
    int hoursWorked;

public:
    PartTimeEmployee(string id, string n, int a, string addr,
                     string dept, string date, int exp, float rate, int hours)
        : Employee(id, n, a, addr, dept, date, exp),
          hourlyRate(rate), hoursWorked(hours) {}

    void logHours(int hours) {
        hoursWorked += hours;
        cout << name << " logged " << hours << " hours. Total: " << hoursWorked << "\n";
    }

    float getBaseSalary() override {
        return hourlyRate * 160;
    }

    float calculatePay() override {
        return hourlyRate * hoursWorked;
    }

    string getEmployeeType() override {
        return "Part-Time Employee";
    }

    void display() override {
        Employee::display();
        cout << "Hourly Rate: $" << hourlyRate << endl;
        cout << "Hours Work : " << hoursWorked << endl;
    }

    int getHours() { return hoursWorked; }
};

// Intern - inherits from PartTimeEmployee
class Intern : public PartTimeEmployee {
private:
    string university;
    string mentor;

public:
    Intern(string id, string n, int a, string addr,
           string dept, string date, string univ, string ment, int hours)
        : PartTimeEmployee(id, n, a, addr, dept, date, 0, 15.0, hours),
          university(univ), mentor(ment) {}

    string getEmployeeType() override {
        return "Intern";
    }

    void display() override {
        PartTimeEmployee::display();
        cout << "University : " << university << endl;
        cout << "Mentor     : " << mentor << endl;
    }

    string getUniversity() { return university; }
};

// ================= CONTRACT EMPLOYEES =================

class ContractEmployee : public Employee {
private:
    float contractAmount;
    string contractEndDate;
    bool isCompleted;

public:
    ContractEmployee(string id, string n, int a, string addr,
                     string dept, string date, int exp,
                     float amount, string endDate, bool completed)
        : Employee(id, n, a, addr, dept, date, exp),
          contractAmount(amount), contractEndDate(endDate),
          isCompleted(completed) {}

    float getBaseSalary() override {
        return contractAmount;
    }

    float calculatePay() override {
        return isCompleted ? contractAmount : contractAmount * 0.5;
    }

    void completeContract() {
        isCompleted = true;
        cout << name << "'s contract has been marked as completed!\n";
    }

    string getEmployeeType() override {
        return "Contract Employee";
    }

    void display() override {
        Employee::display();
        cout << "Contract   : $" << contractAmount << endl;
        cout << "End Date   : " << contractEndDate << endl;
        cout << "Status     : " << (isCompleted ? "Completed" : "In Progress") << endl;
    }

    bool isContractCompleted() { return isCompleted; }
};

// ================= COMPANY CLASS =================

class Company {
private:
    string name;
    vector<unique_ptr<Employee>> employees;

public:
    Company(string n) : name(n) {}

    void addEmployee(Employee* emp) {
        employees.push_back(unique_ptr<Employee>(emp));
    }

    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            cout << "Creating sample file...\n";
            createSampleFile(filename);
            file.open(filename);
            if (!file.is_open()) {
                return false;
            }
        }

        string line;
        int lineNum = 0;

        while (getline(file, line)) {
            lineNum++;
            if (line.empty() || line[0] == '#') continue;

            stringstream ss(line);
            string type;
            ss >> type;

            try {
                if (type == "MANAGER") {
                    string id, name, addr, dept, date, temp;
                    int age, exp, teamSize;
                    float salary;

                    ss >> id >> age >> exp;
                    getline(ss, temp, '|');
                    getline(ss, name, '|');
                    getline(ss, addr, '|');
                    getline(ss, dept, '|');
                    getline(ss, date, '|');
                    ss >> salary >> teamSize;

                    addEmployee(new Manager(id, name, age, addr, dept, date, exp, salary, teamSize));
                }
                else if (type == "DEVELOPER") {
                    string id, name, addr, dept, date, lang, temp;
                    int age, exp, projects;
                    float salary;

                    ss >> id >> age >> exp;
                    getline(ss, temp, '|');
                    getline(ss, name, '|');
                    getline(ss, addr, '|');
                    getline(ss, dept, '|');
                    getline(ss, date, '|');
                    getline(ss, lang, '|');
                    ss >> salary >> projects;

                    addEmployee(new Developer(id, name, age, addr, dept, date, exp, salary, lang, projects));
                }
                else if (type == "PARTTIME") {
                    string id, name, addr, dept, date, temp;
                    int age, exp, hours;
                    float rate;

                    ss >> id >> age >> exp;
                    getline(ss, temp, '|');
                    getline(ss, name, '|');
                    getline(ss, addr, '|');
                    getline(ss, dept, '|');
                    getline(ss, date, '|');
                    ss >> rate >> hours;

                    addEmployee(new PartTimeEmployee(id, name, age, addr, dept, date, exp, rate, hours));
                }
                else if (type == "INTERN") {
                    string id, name, addr, dept, date, univ, mentor, temp;
                    int age, hours;

                    ss >> id >> age;
                    getline(ss, temp, '|');
                    getline(ss, name, '|');
                    getline(ss, addr, '|');
                    getline(ss, dept, '|');
                    getline(ss, date, '|');
                    getline(ss, univ, '|');
                    getline(ss, mentor, '|');
                    ss >> hours;

                    addEmployee(new Intern(id, name, age, addr, dept, date, univ, mentor, hours));
                }
                else if (type == "CONTRACT") {
                    string id, name, addr, dept, date, endDate, temp;
                    int age, exp, completed;
                    float amount;

                    ss >> id >> age >> exp;
                    getline(ss, temp, '|');
                    getline(ss, name, '|');
                    getline(ss, addr, '|');
                    getline(ss, dept, '|');
                    getline(ss, date, '|');
                    getline(ss, endDate, '|');
                    ss >> amount >> completed;

                    addEmployee(new ContractEmployee(id, name, age, addr, dept, date, exp, amount, endDate, completed == 1));
                }
            }
            catch (const exception& e) {
                cout << "Error parsing line " << lineNum << ": " << e.what() << endl;
            }
        }

        file.close();
        return true;
    }

    void createSampleFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create sample file\n";
            return;
        }

        file << "# Employee Data File\n";
        file << "MANAGER M001 35 10 |Alice Johnson|123 Main St|Engineering|2020-01-15| 8000 5\n";
        file << "DEVELOPER D001 28 5 |Bob Smith|456 Oak Ave|Engineering|2021-06-01|C++| 6000 2\n";
        file << "PARTTIME P001 45 15 |David Brown|321 Elm St|Support|2022-01-01| 25.0 80\n";
        file << "INTERN I001 22 |Emma Davis|654 Maple Dr|Engineering|2024-09-01|Tech University|Bob Smith| 120\n";
        file << "CONTRACT C001 40 8 |Frank Miller|987 Cedar Ln|Marketing|2024-01-01|2024-12-31| 15000 1\n";

        file.close();
        cout << "Sample file created!\n";
    }

    void displayAllEmployees() {
        cout << "\n========================================\n";
        cout << "      " << name << " - ALL EMPLOYEES\n";
        cout << "========================================\n";
        for (auto& emp : employees) {
            emp->display();
        }
    }

    void generateAllPayslips() {
        cout << "\n========================================\n";
        cout << "      GENERATING ALL PAYSLIPS\n";
        cout << "========================================\n";
        for (auto& emp : employees) {
            emp->generatePaySlip();
        }
    }

    void showTotalPayroll() {
        float total = 0;
        for (auto& emp : employees) {
            total += emp->calculatePay();
        }
        cout << "\n========================================\n";
        cout << "Total Payroll: $" << fixed << setprecision(2) << total << endl;
        cout << "========================================\n";
    }

    void searchEmployeeByID() {
        string id;
        cout << "Enter Employee ID: ";
        cin >> id;

        for (auto& emp : employees) {
            if (emp->getID() == id) {
                emp->display();
                return;
            }
        }
        cout << "Employee not found!\n";
    }

    void searchEmployeeByName() {
        string name;
        cout << "Enter Employee Name: ";
        cin.ignore();
        getline(cin, name);

        bool found = false;
        for (auto& emp : employees) {
            if (emp->getName().find(name) != string::npos) {
                emp->display();
                found = true;
            }
        }
        if (!found) cout << "No employees found!\n";
    }

    void filterByDepartment() {
        string dept;
        cout << "Enter Department: ";
        cin.ignore();
        getline(cin, dept);

        cout << "\n--- Employees in " << dept << " ---\n";
        bool found = false;
        for (auto& emp : employees) {
            if (emp->getDepartment() == dept) {
                emp->display();
                found = true;
            }
        }
        if (!found) cout << "No employees in this department!\n";
    }

    void showDepartmentStats() {
        map<string, int> deptCount;
        map<string, float> deptPayroll;

        for (auto& emp : employees) {
            string dept = emp->getDepartment();
            deptCount[dept]++;
            deptPayroll[dept] += emp->calculatePay();
        }

        cout << "\n========================================\n";
        cout << "      DEPARTMENT STATISTICS\n";
        cout << "========================================\n";
        cout << left << setw(20) << "Department"
             << right << setw(10) << "Count"
             << setw(15) << "Payroll" << endl;
        cout << "----------------------------------------\n";

        for (auto& pair : deptCount) {
            cout << left << setw(20) << pair.first
                 << right << setw(10) << pair.second
                 << setw(15) << fixed << setprecision(2)
                 << "$" << deptPayroll[pair.first] << endl;
        }
    }

    void topEarners() {
        vector<pair<string, float>> earnings;

        for (auto& emp : employees) {
            earnings.push_back({emp->getName(), emp->calculatePay()});
        }

        sort(earnings.begin(), earnings.end(),
             [](const pair<string, float>& a, const pair<string, float>& b) {
                 return a.second > b.second;
             });

        cout << "\n========================================\n";
        cout << "      TOP 10 EARNERS\n";
        cout << "========================================\n";

        int count = min(10, (int)earnings.size());
        for (int i = 0; i < count; i++) {
            cout << (i+1) << ". " << left << setw(25) << earnings[i].first
                 << " $" << fixed << setprecision(2) << earnings[i].second << endl;
        }
    }

    void addPerformanceReview() {
        string id;
        cout << "Enter Employee ID: ";
        cin >> id;

        for (auto& emp : employees) {
            if (emp->getID() == id) {
                int rating;
                string review, date, reviewer;

                cout << "Enter Rating (1-5): ";
                cin >> rating;
                if (rating < 1 || rating > 5) {
                    cout << "Invalid rating!\n";
                    return;
                }

                cin.ignore();
                cout << "Enter Review: ";
                getline(cin, review);

                cout << "Enter Date (YYYY-MM-DD): ";
                getline(cin, date);

                cout << "Reviewed By: ";
                getline(cin, reviewer);

                emp->addPerformanceReview(rating, review, date, reviewer);
                cout << "Performance review added successfully!\n";
                return;
            }
        }
        cout << "Employee not found!\n";
    }

    void viewPerformanceHistory() {
        string id;
        cout << "Enter Employee ID: ";
        cin >> id;

        for (auto& emp : employees) {
            if (emp->getID() == id) {
                emp->showPerformanceHistory();
                cout << "Average Rating: " << fixed << setprecision(2)
                     << emp->getAverageRating() << "/5\n";
                return;
            }
        }
        cout << "Employee not found!\n";
    }

    void applyForLeave() {
        string id;
        int days;

        cout << "Enter Employee ID: ";
        cin >> id;
        cout << "Enter number of days: ";
        cin >> days;

        for (auto& emp : employees) {
            if (emp->getID() == id) {
                if (emp->applyLeave(days)) {
                    cout << "Leave approved for " << emp->getName() << "!\n";
                } else {
                    cout << "Insufficient leave balance!\n";
                }
                return;
            }
        }
        cout << "Employee not found!\n";
    }

    void showEmployeesByExperience() {
        vector<Employee*> sortedEmps;
        for (auto& emp : employees) {
            sortedEmps.push_back(emp.get());
        }

        sort(sortedEmps.begin(), sortedEmps.end(),
             [](Employee* a, Employee* b) {
                 return a->getExperience() > b->getExperience();
             });

        cout << "\n========================================\n";
        cout << "   EMPLOYEES BY EXPERIENCE\n";
        cout << "========================================\n";

        for (auto emp : sortedEmps) {
            cout << emp->getName() << " - " << emp->getExperience() << " years\n";
        }
    }

    void giveRaiseToEmployee() {
        string id;
        float percentage;

        cout << "Enter Employee ID: ";
        cin >> id;
        cout << "Enter raise percentage: ";
        cin >> percentage;

        for (auto& emp : employees) {
            if (emp->getID() == id) {
                FullTimeEmployee* fte = dynamic_cast<FullTimeEmployee*>(emp.get());
                if (fte) {
                    fte->giveRaise(percentage);
                } else {
                    cout << "Raises only applicable to full-time employees!\n";
                }
                return;
            }
        }
        cout << "Employee not found!\n";
    }

    void completeProjectForDeveloper() {
        string id;
        cout << "Enter Developer ID: ";
        cin >> id;

        for (auto& emp : employees) {
            if (emp->getID() == id) {
                Developer* dev = dynamic_cast<Developer*>(emp.get());
                if (dev) {
                    dev->completeProject();
                } else {
                    cout << "This employee is not a developer!\n";
                }
                return;
            }
        }
        cout << "Developer not found!\n";
    }

    void logHoursForPartTime() {
        string id;
        int hours;

        cout << "Enter Employee ID: ";
        cin >> id;
        cout << "Enter hours worked: ";
        cin >> hours;

        for (auto& emp : employees) {
            if (emp->getID() == id) {
                PartTimeEmployee* pte = dynamic_cast<PartTimeEmployee*>(emp.get());
                if (pte) {
                    pte->logHours(hours);
                } else {
                    cout << "This employee is not part-time!\n";
                }
                return;
            }
        }
        cout << "Employee not found!\n";
    }

    void exportToFile() {
        string filename;
        cout << "Enter filename to export: ";
        cin >> filename;

        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error creating file!\n";
            return;
        }

        file << "EMPLOYEE REPORT - " << name << "\n";
        file << "===========================================\n\n";

        for (auto& emp : employees) {
            file << "ID: " << emp->getID() << "\n";
            file << "Name: " << emp->getName() << "\n";
            file << "Type: " << emp->getEmployeeType() << "\n";
            file << "Department: " << emp->getDepartment() << "\n";
            file << "Salary: $" << fixed << setprecision(2)
                 << emp->calculatePay() << "\n";
            file << "-------------------------------------------\n";
        }

        file.close();
        cout << "Report exported successfully to " << filename << "!\n";
    }

    int getEmployeeCount() {
        return employees.size();
    }
};

// ================= MAIN =================

int main() {
    Company company("TechCorp Solutions");

    cout << "========================================\n";
    cout << "   EMPLOYEE MANAGEMENT SYSTEM\n";
    cout << "========================================\n\n";

    string filename = "employees.txt";
    cout << "Loading employees from '" << filename << "'...\n\n";

    if (!company.loadFromFile(filename)) {
        cout << "Failed to load employees!\n";
        return 1;
    }

    cout << company.getEmployeeCount() << " employees loaded!\n";

    int choice;
    do {
        cout << "\n========================================\n";
        cout << "   EMPLOYEE MANAGEMENT SYSTEM\n";
        cout << "========================================\n";
        cout << "1.  Display All Employees\n";
        cout << "2.  Search Employee by ID\n";
        cout << "3.  Search Employee by Name\n";
        cout << "4.  Filter by Department\n";
        cout << "5.  Generate All Payslips\n";
        cout << "6.  Show Total Payroll\n";
        cout << "7.  Department Statistics\n";
        cout << "8.  Top 10 Earners\n";
        cout << "9.  Add Performance Review\n";
        cout << "10. View Performance History\n";
        cout << "11. Apply for Leave\n";
        cout << "12. Employees by Experience\n";
        cout << "13. Give Raise to Employee\n";
        cout << "14. Complete Project (Developer)\n";
        cout << "15. Log Hours (Part-Time)\n";
        cout << "16. Export Report to File\n";
        cout << "17. Exit\n";
        cout << "========================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: company.displayAllEmployees(); break;
            case 2: company.searchEmployeeByID(); break;
            case 3: company.searchEmployeeByName(); break;
            case 4: company.filterByDepartment(); break;
            case 5: company.generateAllPayslips(); break;
            case 6: company.showTotalPayroll(); break;
            case 7: company.showDepartmentStats(); break;
            case 8: company.topEarners(); break;
            case 9: company.addPerformanceReview(); break;
            case 10: company.viewPerformanceHistory(); break;
            case 11: company.applyForLeave(); break;
            case 12: company.showEmployeesByExperience(); break;
            case 13: company.giveRaiseToEmployee(); break;
            case 14: company.completeProjectForDeveloper(); break;
            case 15: company.logHoursForPartTime(); break;
            case 16: company.exportToFile(); break;
            case 17: cout << "Exiting system...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 17);

    return 0;
}
