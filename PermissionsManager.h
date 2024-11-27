#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

class Customer;

class PermissionsManager {
private:
	//Mapping the username with the user permissions
	std::unordered_map<std::string, int> customersPermissionsMap;

public:
	PermissionsManager(std::vector<std::shared_ptr<Customer>> customers);
	PermissionsManager();

	void LoadCustomers(const std::vector<std::shared_ptr<Customer>>& customers);
	void RemovePermissions(std::shared_ptr<Customer> customer);
	void AddPermissions(std::shared_ptr<Customer> customer);
	bool HasPermission(std::string const& username, unsigned int feature);
	void SetPermissions(std::shared_ptr<Customer> customer, std::vector<std::string> features);
};
