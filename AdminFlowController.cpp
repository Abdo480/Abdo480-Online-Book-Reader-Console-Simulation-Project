#include "AdminFlowController.h"
#include "Helper.h"
#include <iostream>
#include "CustomerFlowController.h"

void AdminFlowController::TakeControl() {
	std::cout << "\n -- Welcome to the system, Sir." << admin.GetName() << " --\n";	
	ShowMainMenu();
}

bool AdminFlowController::isAdmin(std::string const& username, std::string const& password) const {
	return (admin.GetUsername() == username && admin.GetPassword() == password);
}

void AdminFlowController::ShowPermissionsManagementMenu(std::vector<std::string> const& menuOptions) {
	int choice = 0;
	do {
		std::cout << "\x1b[2J\x1b[H";
		choice = Helper::ReadMenuOption({ "Rest/Set Permissions", "Remove Permission", "Add Permission", "Back" });
		if (choice == static_cast<int>(PermissionsMenuOptions::SetPermissions)) {
			admin.SetPermissions(menuOptions);
			std::cout << "\x1b[2J\x1b[H";
		}

		else if (choice == static_cast<int>(PermissionsMenuOptions::AddPermission)) {
			admin.AddPermissions(menuOptions);
			std::cout << "\x1b[2J\x1b[H";
		}

		else if (choice == static_cast<int>(PermissionsMenuOptions::RemovePermission)) {
			admin.RemovePermissions(menuOptions);
			std::cout << "\x1b[2J\x1b[H";
		}

	} while (choice != static_cast<int>(PermissionsMenuOptions::Back));
	std::cout << "\x1b[2J\x1b[H";
}


void AdminFlowController::ShowMainMenu() {
	int choice = 0;
	do {
		 choice = Helper::ReadMenuOption({ "View Profile", "Add Book", "Block Customer", "Unblock Customer", "ManagePermissions", "Logout" });
		 if (choice == static_cast<int>(AdminMenuOptions::ViewPrfile)) {
			 std::cout << "\x1b[2J\x1b[H";
			 ViewProfile();
		 } 
		 else if (choice == static_cast<int>(AdminMenuOptions::AddBook)) {
			 std::cout << "\x1b[2J\x1b[H";
			 admin.AddBook();
		 }
		 else if (choice == static_cast<int>(AdminMenuOptions::BlockCustomer)) {
			 std::cout << "\x1b[2J\x1b[H";
			 admin.BlockCustomer();
		 }
		 else if (choice == static_cast<int>(AdminMenuOptions::UnBlockCustomer)) {
			 std::cout << "\x1b[2J\x1b[H";
			 admin.UnBlockCustomer();
		 }
		 else if (choice == static_cast<int>(AdminMenuOptions::ManagePermissions)){
			 const std::vector<std::string>& menuOptions = CustomerFlowController::GetInstance()->GetMenuOptions();
			 std::cout << "\x1b[2J\x1b[H";
			 ShowPermissionsManagementMenu(menuOptions);
		 }

	} while (choice != static_cast<int>(AdminMenuOptions::Logout));
}

void AdminFlowController::ViewProfile() const {
	std::cout << "\n";
	std::cout << "\nProfile Preview:\n";
	std::cout << "\n_______________________________________\n";
	std::cout << "Name: " << admin.GetName();
	std::cout << "\nEmail: " << admin.GetEmail();
	std::cout << "\nUser name: " << admin.GetUsername();
	std::cout << "\n_______________________________________\n";
	std::cout << "Press any key to go back to main menu...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	std::cout << "\x1b[2J\x1b[H";
}