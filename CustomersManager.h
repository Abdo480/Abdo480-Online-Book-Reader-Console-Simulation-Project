#pragma once
#include "Customer.h"
#include <memory>

class Admin;

class CustomersManager {
private:
	std::vector<std::shared_ptr<Customer>> customers;

	CustomersManager();
	std::vector<std::string> blockedUsernames;
	static CustomersManager* customersManager;
	int lastID = 0;
	const std::string blockedUsersFileNames = "Users/BlockedUsernames.txt";
	const std::string fileName = "Users/Customers.txt";

	//Only Admin can access these functions
	void BlockCustomerBy(int index);
	void AddCustomerToBlackList(std::string const& username);
	bool ListBlockedCustomers() const;
	bool RemoveCustomerFromBlackList(std::string const& username);
	void UnBlockCustomerBy(int index);
	std::vector<std::string> GetUnblockedCustomers();

public:
	friend class Admin;
	friend class CustomerFlowController;

	static CustomersManager* GetInstance();
	bool LoadDatabse(
);
	bool UpdateDatabse() const;
	void ListCustomers() const;
	bool isUsernameUsed(std::string const& username) const;
	bool isBlockedCustomer(std::string const& username);
	void AddCustomerToDatabase(std::shared_ptr<Customer> customer);
	std::vector<std::shared_ptr<Customer>> GetCustomers() const;

	std::shared_ptr<Customer> GetCustomer(std::string const& username, std::string const& password);
	size_t GetCustomersSize() const;
};