#include "CustomerFlowController.h"
#include <iostream>
#include "Helper.h"
#include <string>
#include "BookContent.h"
#include "MyExceptions.h"
#include <cmath>

bool CustomerFlowController::isValidCustomer(std::string const& username, std::string const& password) const {
	return customersManager->GetCustomer(username, password) != nullptr;
}

BooksManager* CustomerFlowController::booksManager = BooksManager::GetInstance();
CustomersManager* CustomerFlowController::customersManager = CustomersManager::GetInstance();

CustomerFlowController::~CustomerFlowController() {
	delete customerFlowController;
}

CustomerFlowController::CustomerFlowController() : currentCustomer(nullptr), permissionsManager(customersManager->GetCustomers()){}

CustomerFlowController* CustomerFlowController::customerFlowController = nullptr;

std::shared_ptr<Customer> CustomerFlowController::GetCurrentCustomer() const {
	return currentCustomer;
}

CustomerFlowController* CustomerFlowController::GetInstance() {
	if (customerFlowController == nullptr)
		customerFlowController = new CustomerFlowController();
	return customerFlowController;
}

bool CustomerFlowController::SignUp() {
	int tries = 0;

	currentCustomer = std::shared_ptr<Customer>(new Customer());
	std::string username, password, data;
	std::cout << "\nEnter The Following Info:\n";

	do {
		++tries;
		if (tries > 3) {
			std::cout << "\nYou exceeded the maximum number of tries.\n";
			return false;
		}

		
		std::cout << "\tUsername (No Spaces): ";
		std::cin >> username;
		while (customersManager->isUsernameUsed(username)) {
			std::cout << "\nUsername is used, try again.\n";
			std::cout << "\tEnter Username (No Spaces): ";
			std::cin >> username;
		}

		currentCustomer->SetUsername(username);

		std::cout << "\tPassword (No Spaces): ";
		std::cin >> password;
		currentCustomer->SetPassword(password);

		if (isValidCustomer(username, password)) {
			std::cout << "\nInvalid username or password, try again\n";
		}

	} while (isValidCustomer(username, password));

	std::cout << "\tName: ";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(std::cin, data);
	currentCustomer->SetName(data);
	
	std::cout << "\tEmail: ";
	std::cin >> data;
	currentCustomer->SetEmail(data);

	while (!Helper::isValidEmail(data)) {
		std::cout << "\nInvalid Email, try again.\n";
		std::cout << "\tEnter Email: ";
		std::cin >> data;
	}

	currentCustomer->SetID(++customersManager->lastID);
	customersManager->AddCustomerToDatabase(currentCustomer);

	std::cout << "\x1b[2J\x1b[H";
	ShowMainMenu();
	return true;
}

void CustomerFlowController::TakeControl(std::string const& username, std::string const& password) {
	customersManager->LoadDatabse();
	permissionsManager.LoadCustomers(customersManager->customers);
	currentCustomer = customersManager->GetCustomer(username, password);
	std::cout << "******* Welcome to the system " << currentCustomer->GetName() << " *******\n\n";

	ShowMainMenu();
}

void CustomerFlowController::ShowMainMenu() {
	int choice = 0;
	do {
		//Doesn't Clear the screen the first time only
		if (choice != 0)
		std::cout << "\x1b[2J\x1b[H";
		choice = Helper::ReadMenuOption(menuOptions);

		if (choice == static_cast<int>(CustomerMenuOptions::ViewProfile)) {
			std::cout << "\x1b[2J\x1b[H";
			ViewProfile();
		}
		else if (choice == static_cast<int>(CustomerMenuOptions::ListSelectFromReadingHistory)) {
			std::cout << "\x1b[2J\x1b[H";
			ListSelectFromReadingHistory();
		}
		else if (choice == static_cast<int>(CustomerMenuOptions::ListSelectFromAvailableBooks)) {
			std::cout << "\x1b[2J\x1b[H";
			ListSelectFromAvailableBooks();
		}
	} while(choice != static_cast<int>(CustomerMenuOptions::Logout));
}

const std::vector<std::string>& CustomerFlowController::GetMenuOptions() const {
	return menuOptions;
}

void CustomerFlowController::ViewProfile() {
	if (!permissionsManager.HasPermission(currentCustomer->GetUsername(), static_cast<unsigned int>(CustomerMenuOptions::ViewProfile))) {
		std::cout << "\nAccess Denied\n";
		std::cout << "\nYou were banned from using this feature in the system by the administrator\n";

		std::cout << "\nPress any key to go back to main menu...";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
		return;
	}


	const std::vector<std::string>& customerMenuOptions = CustomerFlowController::GetInstance()->GetMenuOptions();

	std::vector<std::string> permittedFeatures;
	for (int i = 0; i < customerMenuOptions.size(); ++i) {
		if (currentCustomer->GetPermissions() & (1 << i))
			permittedFeatures.push_back(customerMenuOptions[i]);
	}


	std::cout << "Profile Preview:\n";
	std::cout << "\n_______________________________________\n";
	std::cout << "Name: " << currentCustomer->GetName();
	std::cout << "\nEmail: " << currentCustomer->GetEmail();
	std::cout << "\nUser name: " << currentCustomer->GetUsername();
	std::cout << "\nPermissions: " << currentCustomer->GetPermissions();

	//Printing permissions names
	int idx = 0;
	if (currentCustomer->GetPermissions() > 0) {
		std::cout << " - Which are the following features:\n\n";
		for (const auto& feature : permittedFeatures)
			std::cout << "\t" << ++idx << ": " << feature << "\n";
	}

	std::cout << "\n_______________________________________\n";
	std::cout << "Press any key to go back to main menu...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}

void CustomerFlowController::ListSelectFromReadingHistory()  {
	if (!permissionsManager.HasPermission(currentCustomer->GetUsername(), static_cast<unsigned int>(CustomerMenuOptions::ListSelectFromReadingHistory))) {
		std::cout << "\nAccess Denied\n";
		std::cout << "\nYou were banned from using this feature in the system by the administrator\n";

		std::cout << "\nPress any key to go back to main menu...";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
		return;
	}

	std::vector<std::shared_ptr<ReadingHistory>> readingHistories = currentCustomer->GetReadingHistories();

	if (readingHistories.size() == 0) {
		std::cout << "\nNo reading histories yet.\n";
		std::cout << "\nPress any key to go back to main menu...";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
		return;
	}

	currentCustomer->DisplayReadingHistories();
	std::cout << "\nWhich Session to open?:\n";
	int readingHistoryIndex = Helper::ReadNumberInRange(1, (int)readingHistories.size());
	std::shared_ptr<Book> book = readingHistories[--readingHistoryIndex]->GetBook();

	std::shared_ptr<BookContent> bookContent;
	try {
		bookContent = book->GetBookContent();
	}
	catch (DanglingException& e) {
		std::cout << "\nNot a valid object: " << e.GetMessage() << "\n";
		std::cout << "Value = " << e.what();
		std::cout << "\nMake sure to assign a content to the book object\n";
		return;
	}
	
	bookContent->ReadBook(currentCustomer, book);
}

void CustomerFlowController::ListSelectFromAvailableBooks() {
	if (!permissionsManager.HasPermission(currentCustomer->GetUsername(), static_cast<unsigned int>(CustomerMenuOptions::ListSelectFromAvailableBooks))) {
		std::cout << "\nAccess Denied\n";
		std::cout << "\nYou were banned from using this feature in the system by the administrator\n";

		std::cout << "\nPress any key to go back to main menu...";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
		return;
	}

	booksManager->ListSelectFromAvailableBooks(currentCustomer);
}