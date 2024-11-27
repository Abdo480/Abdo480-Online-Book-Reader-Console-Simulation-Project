#pragma once
#include <string>
#include "PermissionsManager.h"
#include <vector>

class BooksManager;
class CustomersManager;
class Admin {
private:
	std::string name;
	std::string username;
	std::string password;
	std::string email;
	int ID;
	static BooksManager* booksManager;
	static CustomersManager* customersManager;
	PermissionsManager permissionsManager;

public:
	Admin();
	bool isAuthData(std::string const& username, std::string const& password) const;
	void AddBook();
	bool BlockCustomer() const;
	bool UnBlockCustomer();
	void SetPermissions(std::vector<std::string> const& menuOptions);
	void AddPermissions(std::vector<std::string> const& menuOptions);
	void RemovePermissions(std::vector<std::string> const& menuOptions);

	//Getters & setters
	const std::string& GetName() const;
	void SetName(std::string const& name);

	const std::string& GetUsername() const;
	void SetUsername(std::string const& username);

	const std::string& GetPassword() const;
	void SetPassword(std::string const& password);

	const std::string& GetEmail() const;
	void SetEmail(std::string const& email);

	int GetID() const;
	void SetID(int ID);
};