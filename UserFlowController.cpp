#include "UserFlowController.h"
#include "Helper.h"
#include <iostream>
#include <string>

class MenuOptions {
public:
	enum { Login = 1, SignUp, Exit};
};

CustomersManager* UserFlowController::customersManager = CustomersManager::GetInstance();
CustomerFlowController* UserFlowController::customerFlowController = CustomerFlowController::GetInstance();

void UserFlowController::ShowMainMenu() {
	int choice = 0;
	while (choice != MenuOptions::Exit) {
		std::cout << "\x1b[2J\x1b[H";
		choice = Helper::ReadMenuOption({ "Login", "Sign Up", "Exist."});
		if (choice == MenuOptions::Login)
			Login();
		else if (choice == MenuOptions::SignUp)
			SignUp();
	}
}

void UserFlowController::Login() {
	std::string username, password;
	std::cout << "Enter username (No spaces): ";
	std::cin >> username;

	std::cout << "Enter password (No spaces): ";
	std::cin >> password;

	if (customerFlowController->isValidCustomer(username, password)) {
		if (customersManager->isBlockedCustomer(username)) {
			std::cout << "\nYou were banned from using the system by the Administator.\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nPress any key go back to main menu...";
			std::cin.get();
			std::cout << "\x1b[2J\x1b[H";
			return;
		}
		std::cout << "\x1b[2J\x1b[H";
		customerFlowController->TakeControl(username, password);
	}
	else if (adminFlowController.isAdmin(username, password)) {
		std::cout << "\x1b[2J\x1b[H";
		adminFlowController.TakeControl();
	}

	else {
		std::cout << "\nThis user doesn't exist, try again\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nPress any key to try again...";
		std::cin.get();
		std::cout << "\x1b[2J\x1b[H";
	}
}

void UserFlowController::SignUp() {
	customerFlowController->SignUp();
}