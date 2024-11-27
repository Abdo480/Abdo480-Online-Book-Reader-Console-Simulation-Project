#include "Admin.h"
#include "BookContent.h"
#include "CustomersManager.h"
#include "BooksManager.h"
#include <memory>
#include <iostream>
#include "CustomerFlowController.h"
#include <unordered_map>
#include <cmath>

Admin::Admin() :
	name("Abdulrahman Sayed"),
	username("AbdoSayed"),
	password("123"),
	email("abdo@gmail.com"),
	ID(-1), permissionsManager(customersManager->customers){

}

CustomersManager* Admin::customersManager = CustomersManager::GetInstance();
BooksManager* Admin::booksManager = BooksManager::GetInstance();

bool Admin::isAuthData(std::string const& username, std::string const& password) const {
	if (username == GetUsername() && password == GetPassword())
		return true;
	return false;
}

void Admin::AddBook() {
	std::shared_ptr<Book> book(new Book);
	book->Read();
	booksManager->AddBookToDatabse(book);
	std::cout << "\nBook was added successfully.\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Press any key to go back to main menu...";
	std::cin.get();
	std::cout << "\x1b[2J\x1b[H";
}

bool Admin::BlockCustomer() const {
	if (customersManager->GetCustomersSize() == 0) {
		std::cout << "\nNo customers in the system.\n";
		return false;
	}

	//Would list all customers, but we want to only list the unblocked customers, thus we need a different function
	//customersManager->ListCustomers();
	
	//Listing only unblocked customers
	int idx = 0;
	std::vector<std::string> unblockedCustomers = customersManager->GetUnblockedCustomers();
	if (unblockedCustomers.size() == 0) {
		std::cout << "\nNo customers to block :)\n";
		return false;
	}

	std::cout << "\nThe Following is the list of unblocked customers (NOTE: this list EXCLUDES blocked customers)\n";
	for (const auto& username : unblockedCustomers)
		std::cout << "\t" << ++idx << ": " << username << "\n";

	int customerIndex = Helper::ReadNumberInRange(1, (int)unblockedCustomers.size());
	std::string username = unblockedCustomers[--customerIndex];
	customersManager->AddCustomerToBlackList(username);
	std::cout << "\nCustomer was blocked successfully\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Press any key to go back to main menu...";
	std::cin.get();
	std::cout << "\x1b[2J\x1b[H";
	return true;
}

bool Admin::UnBlockCustomer() {
	if (customersManager->GetCustomersSize() == 0) {
		std::cout << "\nNo customers in the system.\n";
		return false;
	}

	//Returns false if the list is empty, and true otherwise
	if (!customersManager->ListBlockedCustomers()) {
		std::cout << "\nNo users are currently blocked :)\n";
		return false;
	}

	int customerIndex = Helper::ReadNumberInRange(1, (int)customersManager->GetCustomersSize());
	customersManager->UnBlockCustomerBy(--customerIndex);
	return true;
}

void Admin::SetPermissions(std::vector<std::string> const& menuOptions) {
	if ((int)customersManager->GetCustomersSize() == 0) {
		std::cout << "\nNo customers to manage their permissions.\n";
		return;
	}

	std::cout << "\x1b[2J\x1b[H";
	std::cout << "\nChoose a customer to set their permissions:\n"; 
	customersManager->ListCustomers();
	int customerIdx = Helper::ReadNumberInRange(1, (int)customersManager->GetCustomersSize());
	std::shared_ptr<Customer> customer = customersManager->customers[--customerIdx];

	permissionsManager.SetPermissions(customer, menuOptions);

	std::cout << "\nPermissions were set successfuly.\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Press any key to go back to main menu...";
	std::cin.get();
}

void Admin::AddPermissions(std::vector<std::string> const& menuOptions) {
	if ((int)customersManager->GetCustomersSize() == 0) {
		std::cout << "\nNo customers to manage their permissions.\n";
		return;
	}

	std::cout << "\x1b[2J\x1b[H";
	std::cout << "\nWhich of the following customers to add permissions to:\n";
	customersManager->ListCustomers();
	int customerIdx = Helper::ReadNumberInRange(1, (int)customersManager->GetCustomersSize());
	std::shared_ptr<Customer> customer = customersManager->customers[--customerIdx];
	permissionsManager.AddPermissions(customer);
	
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Press any key to go back to main menu...";
	std::cin.get();
}

void Admin::RemovePermissions(std::vector<std::string> const& menuOptions) {
	int maxCustomers = (int)customersManager->GetCustomersSize();
	if (maxCustomers == 0) {
		std::cout << "\nNo customers to manage their permissions.\n";
		return;
	}

	std::cout << "\x1b[2J\x1b[H";
	std::cout << "\nWhich of the following customers to remove permissions from:\n";
	customersManager->ListCustomers();

	int customerIdx;
	do {
		std::cout << "\nEnter customer ID 1 - " << maxCustomers << " : ";
		std::cin >> customerIdx;
		if (std::cin.fail()) {
			std::cout << "\nInvalid input, Enter a numeric value in range:\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		else if (customerIdx < 1 || customerIdx > maxCustomers) {
			std::cout << "\nInvalid Input, number out of range, try again\n";
			continue;
		}

		break;

	} while (true);
	std::shared_ptr<Customer> customer = customersManager->customers[--customerIdx];
	permissionsManager.RemovePermissions(customer);

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Press any key to go back to main menu...";
	std::cin.get();
}

const std::string& Admin::GetName() const { return name; };
void Admin::SetName(std::string const& name) { this->name = name; }

const std::string& Admin::GetUsername() const { return username; };
void Admin::SetUsername(std::string const& username) { this->username = username; }

const std::string& Admin::GetPassword() const { return password; }
void Admin::SetPassword(std::string const& password) { this->password = password; }

const std::string& Admin::GetEmail() const { return email; }
void Admin::SetEmail(std::string const& email) { this->email = email; }

int Admin::GetID() const { return ID; }
void Admin::SetID(int ID) { this->ID = ID; }