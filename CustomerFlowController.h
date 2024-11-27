#pragma once
#include <string>
#include "Customer.h"
#include "CustomersManager.h"
#include "BooksManager.h"
#include <memory>
#include "PermissionsManager.h"

class CustomerFlowController {
private:
	std::shared_ptr<Customer> currentCustomer;
	static CustomersManager* customersManager;
	static BooksManager* booksManager;
	PermissionsManager permissionsManager;

	CustomerFlowController();
	static CustomerFlowController* customerFlowController;
	std::vector<std::string> menuOptions = { "View Profile", "List & Select From My Reading History", "List & Select From Available Books", "Logout" };

	/*
	* enum class to define menu options for customer flow
	* each option is assigned a unique binary value to facilitate
	* betwise operations in permissions management
	*/
	enum class CustomerMenuOptions{
		ViewProfile = 1,
		ListSelectFromReadingHistory = 2,
		ListSelectFromAvailableBooks = 3,
		Logout = 4 
	};

public:
	bool isValidCustomer(std::string const& username, std::string const& password) const;
	void TakeControl(std::string const& username, std::string const& password);
	void ShowMainMenu();
	bool SignUp();
	void ViewProfile();
	void ListSelectFromReadingHistory();
	void ListSelectFromAvailableBooks();
	std::shared_ptr<Customer> GetCurrentCustomer() const;
	static CustomerFlowController* GetInstance();
	const std::vector<std::string>& GetMenuOptions() const;
	~CustomerFlowController();
};