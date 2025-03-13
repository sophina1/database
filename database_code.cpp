/*
C++ program that performs three operations:
1) Add a new employee
2) Get employee information
3) List all employees
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

fstream file;
string filename = "data.txt";

struct Person {
    string id, firstName, lastName;
    int income;
};

// Function that takes an employee id and searches for that employee in the file
struct Person *search(string id) {
	struct Person *p2 = new Person;
	char empId[5], firstName[15], lastName[15], incomeStr[10];
	file.clear();
	file.seekg(0, ios::beg);
	while (file.read(empId, 5)) {
		empId[5] = '\0';
		p2->id = empId;
        if (p2->id == id) {
        	file.read(firstName, 15); 
        	file.read(lastName, 15); 
        	file.read(incomeStr, 10);
        	p2->firstName = firstName;
        	p2->lastName = lastName;
        	p2->income = stof(incomeStr); // convert incomeStr to float
        	return p2; // return pointer to Person struct
		}
		file.seekg(40, ios::cur);
        // Why it is 40? 
		/*
		The file.seekg uses 40-bytes from current file position because since the function intially
		reads only the 5-byte employee ID, when the ID entered is not found, it needs to skip over
		the remaning 40-bytes of data (firstname(15), lastname(15), and income(10) = 40) in the record
		before reading another ID.
		*/
    	}
    	return 0;
} 

// Function that retrieves employee information from employee id entered by user
void get_employee() {
	string empId;
    cout << "Enter Employee ID: ";
    cin >> empId;
    Person* employee = search(empId);
    if (employee == nullptr) {
        cout << "Employee not found." << endl;
    }
    else {
        cout << "Employee Information:\n";
        cout << "ID: " << employee->id << "\n";
        cout << "Name: " << employee->firstName << " " << employee->lastName << "\n";
        cout << "Income: " << employee->income << "\n";
    }
}

// Function that lists all employees’ information
void list() {
	char id[6], firstName[15], lastName[15], incomeStr[10];
	file.clear();
	file.seekg(0, ios::beg); // rewind file to begining
	while (file.read(id, 5)) {
		file.read(firstName, 15);
		file.read(lastName, 15);
		file.read(incomeStr, 10);
		float income = atof(incomeStr); // convert string to float
		cout << id << " " << firstName << " " << lastName << " " << income << endl;
    }
}

// Function that stores data into the file
void write_data(Person p) {
	file.clear();
	file.write(p.id.c_str(), 5); // write 5 chars for id
	file.write(p.firstName.c_str(), 15); // write 15 chars for first name
	file.write(p.lastName.c_str(), 15); // write 15 chars for last name
	file.write(to_string(p.income).c_str(), 10); // write 10 chars after converting to string
	file.flush();
}

// Function that asks user about a new employee data and call write_data to save the data into the file
void new_employee() {
	string empId;
	while (1) {
		cout << "\n*** New Employee ***\n";
		cout << "Enter Employee ID (-1 to end): ";
		cin >> empId;
		if (empId == "-1") break; // stop loop when user enters -1
        
        // Search if the employee exists
        Person* existingEmployee = search(empId);
        if (existingEmployee != nullptr) {
            cout << "Employee ID " << empId << " already exists.\n";
            return;
        }
		
		// Save data to Person struct
		Person p;
		p.id = empId;
		cout << "Enter first name: ";
		cin >> p.firstName;
		cout << "Enter last name: ";
		cin >> p.lastName;
		cout << "Enter income: ";
		cin >> p.income;
		cout << "Saving data: ";
		write_data(p);
	}
}

int main() {
    file.open(filename, ios::in | ios::out); // open file for read and write
	if (!file) {
		file.open(filename, ios::out); // create the file
		file.close();
		file.open(filename, ios::in | ios::out);
	}
	// Menu:
    int option = -1;
    while (1) {
    	cout << "*** Menu ***" << endl;
    	cout << "1) New employee\n";
    	cout << "2) Search employee information\n";
    	cout << "3) List employee\n";
    	cout << "option: ";
    	cin >> option;
    	switch (option) {
    		case 1: new_employee(); break;
    		case 2: get_employee(); break;
    		case 3: list(); break;
    		default: // any other number will exit the menu loop
    			return 0;
		}
	}
	// Close files
	file.close();
    return 0;
}