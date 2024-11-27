#include "Customer.h"
#include "Helper.h"
#include <sstream>
#include "ReadingHistory.h"
#include <iostream>
#include <cassert>
#include "MyExceptions.h"
#include "ReadingHistoriesManager.h"
#include "BooksManager.h"
#include <iomanip>
#include "CustomerFlowController.h"
#include <cmath>

Customer::Customer() : ID(-1), permissions(-1){}

void Customer::Print() const {

	const std::vector<std::string>& customerMenuOptions = CustomerFlowController::GetInstance()->GetMenuOptions();
	std::vector<std::string> permittedFeatures;
	for (int i = 0; i < customerMenuOptions.size(); ++i) {
		if (permissions & (1 << i))
			permittedFeatures.push_back(customerMenuOptions[i]);
	}

	std::cout << "_______________________________________________________________________________________________________________\n\n";
	std::cout << "| " << std::left << std::setw(10) << ID;
	std::cout << "| " << std::left << std::setw(20) << name;
	std::cout << "| " << std::left << std::setw(35) << email;
	std::cout << "| " << std::left << std::setw(15) << username;
	std::cout << "| " << std::left << std::setw(20)  << permissions;
	std::cout << "|\n_______________________________________________________________________________________________________________\n";
				    
	int idx = 0;
	if (permissions > 0) {
		std::cout << "- Permission is enabled for the following features:\n";
		for (const auto& feature : permittedFeatures)
			std::cout << "\t" << ++idx << ": " << feature << "\n";
	}
	else if (permissions == -1) {
		std::cout << "\nMeaning All Permissions are enabled for this customer.";
	};
	std::cout << "\n";
}

void Customer::DisplayReadingHistories() {
	int idx = 0;
	
	std::cout << "\n___________________________________________________________________________________\n\n";
	std::cout << "| " << std::left << std::setw(5) << "No."
		      << "| " << std::left << std::setw(20) << "Book Name" 
		      << "| " << std::left << std::setw(27) << "Current Progress " 
		      << "| " << std::left << std::setw(22) << "Last Session" 
		      << "|";
	std::cout << "\n___________________________________________________________________________________\n";
	for (const auto& readingHistory : readingHistories) {
		BooksManager::GetInstance()->SetReadingHistoryBookBy(readingHistory);
		std::cout << "| " << std::left << std::setw(5) << ++idx;
		readingHistory->PrintReadingHistory();
	}
	std::cout << "___________________________________________________________________________________\n";
}

std::vector<std::shared_ptr<ReadingHistory>> Customer::GetReadingHistories() { return readingHistories; }


//If there is an old reading history for the same book overwrite it
void Customer::AddReadingHistory(std::shared_ptr<ReadingHistory> readingHistory) {
	if (readingHistory == nullptr)
		throw DanglingException("NullPointerException", NULL);

	for (int pos = 0; pos < readingHistories.size(); ++pos) {
		if (readingHistories[pos]->GetBookID() == readingHistory->GetBookID()) {
			readingHistories.erase(readingHistories.begin() + pos);
			readingHistories.push_back(readingHistory);
			return;
		}
	}
	

	readingHistories.push_back(readingHistory);
}

void Customer::ReadCustomer() {
	std::cout << "\nEnter The Following info:\n";
	std::cout << "\tName: ";
	do {
		std::getline(std::cin, name);
	} while (name.empty());

	std::cout << "username (NO SPACES): ";
	std::cin >> username;

	std::cout << "password: ";
	std::cin >> password;
	
	bool emailNotValid = true;
	while (emailNotValid) {
		std::cout << "Email (NO SPACES): ";
		std::cin >> email;
		if (Helper::isValidEmail(email))
			emailNotValid = false;
		else
			std::cout << "Invalid email address, try again\n";
	}
}

bool Customer::isAuthData(std::string const& username, std::string const& password) const {
	if (username == GetUsername() && password == GetPassword())
		return true;
	return false;
}

/*
* 	std::string name;
	std::string username;
	std::string password;
	std::string email;
	int ID;
	std::vector<ReadingHistory*> readingsHistory;
*/

void Customer::ClearReadingHistoriesBooks() {
	for (const auto& readingHistory : readingHistories) {
		readingHistory->SetBook(nullptr);
	}
}

bool Customer::InitializeWithLine(std::string const& line) {
	std::vector<std::string> substrs = Helper::SplitString(line);
	if (substrs.size() != 6) {
		std::cout << "\nFile format is wrong\n";
		return false;
	}

	name = substrs[0];
	username = substrs[1];
	password = substrs[2];
	email = substrs[3];
	ID = Helper::ToInt(substrs[4]);
	permissions = Helper::ToInt(substrs[5]);
	return true;
}

/*
	std::string lastReadingSession;
	int lastOpenedPage;
	int numberOfPages;
	Book* book;
	Customer* customer;
	DateTime lastSession;
*/

std::string Customer::ToString() const{
	std::ostringstream oss;
	oss << name << "," << username << "," << password << "," << email << "," << ID  << "," << permissions;
	return oss.str();
}

void Customer::SetReadingHistoryBook(std::shared_ptr<Book> book) {
	for (const auto& readingHistory : readingHistories)
		if (readingHistory->GetBookID() == book->GetID())
			readingHistory->SetBook(book);
}

const std::string& Customer::GetName() const{ return name; };
void Customer::SetName(std::string const& name) { this->name = name; }

const std::string& Customer::GetUsername() const { return username; };
void Customer::SetUsername(std::string const& username) { this->username = username; }

const std::string& Customer::GetPassword() const { return password; }
void Customer::SetPassword(std::string const& password) { this->password = password; }

const std::string& Customer::GetEmail() const { return email; }

void Customer::SetEmail(std::string const& email) { this->email = email; }

int Customer::GetID() const { return ID; }
void Customer::SetID(int ID) { this->ID = ID; }

int Customer::GetPermissions() const { return permissions; }

void Customer::SetPermissions(int permissions) { this->permissions = permissions; }