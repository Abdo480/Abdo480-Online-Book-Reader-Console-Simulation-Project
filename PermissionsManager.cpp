#include "PermissionsManager.h"
#include <cctype>
#include <cmath>
#include "CustomersManager.h"
#include "Customer.h"
#include <iostream>
#include "CustomerFlowController.h"
#include "Helper.h"
#include <unordered_map>

PermissionsManager::PermissionsManager() {
}

PermissionsManager::PermissionsManager(std::vector<std::shared_ptr<Customer>> customers) {
	for (const auto customer : customers) {
		customersPermissionsMap[customer->GetUsername()] = customer->GetPermissions();
	}
}

void PermissionsManager::RemovePermissions(std::shared_ptr<Customer> customer) {
	const std::vector<std::string>& menuOptions = CustomerFlowController::GetInstance()->GetMenuOptions();
	std::unordered_map<std::string, int> permittedFeatures;

	//All permissions are enabled
	if (customer->GetPermissions() == -1) {
		std::cout << "\nAll features are enables to be used for this customer:\n";
		//Enabling all features
		int permissions = 0;
		for (int i = 0; i < menuOptions.size(); ++i) {
			permissions |= (1 << i);
			permittedFeatures[menuOptions[i]] = (1 << i);
		}
		customer->SetPermissions(permissions);
	}
	else {
		//Populating all permitted features
		for (int i = 0; i < menuOptions.size(); ++i) {
			if (customer->GetPermissions() & (1 << i))
				permittedFeatures[menuOptions[i]] = (1 << i);
		}
	}

	if (permittedFeatures.size() == 0) {
		std::cout << "\nCustomer is  not permitted to use any feature in the system.\n";
		return;
	}

	int idx = 0;
	std::cout << "\nThe Following are the features the customer is permitted to use\n";
	for (const auto& feature : permittedFeatures) {
		std::cout << "\t" << ++idx << ": " << feature.first << "\n";
	}

	char removeMore = 'y';
	std::cout << "\nDo you want to remove permission:\n";
	int permissions = customer->GetPermissions();
	for (const auto& feature : permittedFeatures) {
		do {
			char answer;
			std::cout << feature.first << " (Y/N): ";
			std::cin >> answer;

			if (tolower(answer) == 'y') {
				permissions &= ~feature.second; // Unset the bit corresponding to the feature
				break;
			}
			else if (tolower(answer) == 'n')
				break;

			else {
				std::cout << "\nInvalid input, Please enter Y or N.\n";
			}

		} while (true);
		std::cout << "\nDo you want to continue removing permissions (Y/N): ";
		std::cin >> removeMore;

		if (removeMore != 'y')
			break;
	}

	customer->SetPermissions(permissions);
	//Updating customer permissions
	CustomersManager::GetInstance()->UpdateDatabse();
	customersPermissionsMap[customer->GetUsername()] = customer->GetPermissions();
	std::cout << "\nPermissions were removed successfully.\n";
}

void PermissionsManager::LoadCustomers(const std::vector<std::shared_ptr<Customer>>& customers) {
	customersPermissionsMap.clear();
	for (const auto customer : customers)
		customersPermissionsMap[customer->GetUsername()] = customer->GetPermissions();
}

void PermissionsManager::AddPermissions(std::shared_ptr<Customer> customer) {
	if (customer->GetPermissions() == -1) {
		std::cout << "\nAll permissions are already enables for this customer:\n";
		return;
	}

	const std::vector<std::string>& menuOptions = CustomerFlowController::GetInstance()->GetMenuOptions();
	//Maping the feature with its corresponding binary representation
	std::unordered_map<std::string, int> deniedFeatures;

	for (int i = 0; i < menuOptions.size(); ++i) {
		if (!(customer->GetPermissions() & (1 << i)))
			deniedFeatures[menuOptions[i]] = 1 << i;
	}

	int idx = 0;
	std::cout << "\nThe Following are the features the customer is not permitted to use\n";
	for (const auto& feature : deniedFeatures) {
		std::cout << "\t" << ++idx << ": " << feature.first << "\n";
	}
	std::cout << "\n_________________________________________________________________________\n";
	std::cout << "\nDo you want to give permission to:\n";
	
	int permissions = customer->GetPermissions();
	char answer = 'y';
	char addMore = 'y';
	for (const auto& feature : deniedFeatures) {

		do {
			std::cout << feature.first << " (Y/N): ";
			std::cin >> answer;

			if (tolower(answer) == 'y') {
				permissions |= feature.second;
				break;
			}
			if (tolower(answer) == 'n')
				break;
			else {
				std::cout << "\nInvalid input, Please enter Y or N.\n";
			}

		} while (true);
		
		std::cout << "\nDo you want to continue adding permissions (Y/N): ";
		std::cin >> addMore;
		
		if (tolower(addMore) != 'y')
			break;
	}

	customer->SetPermissions(permissions);
	CustomersManager::GetInstance()->UpdateDatabse();
	customersPermissionsMap[customer->GetUsername()] = customer->GetPermissions();
	std::cout << "\nPermissions were Added successfully.\n";
}

void PermissionsManager::SetPermissions(std::shared_ptr<Customer> customer, std::vector<std::string> features) {
	if (features.size() == 0) {
		std::cout << "\nNo features to set permissions to:\n";
		return;
	}

	int permissions = 0;
	char answer = 'y';
	char addMore = 'y';
	std::cout << "\nDo you want to give permissions to:\n";
	for (size_t i = 0; i < features.size(); ++i) {
		do {
			std::cout << features[i] << " (Y/N): ";
			std::cin >> answer;

			if (tolower(answer) == 'y') {
				permissions |= 1 << i;
				break;
			}
			else if (tolower(answer) == 'n')
				break;
			else
				std::cout << "Invalid input, Enter Y or N.\n";

		} while (true);
		
		std::cout << "\nDo you want to continue setting more permissions:\n";
		std::cin >> addMore;

		if (addMore != 'y')
			break;
	}

	customer->SetPermissions(permissions);
	//Updating customer permissions
	CustomersManager::GetInstance()->UpdateDatabse();
	customersPermissionsMap[customer->GetUsername()] = customer->GetPermissions();
	std::cout << "\nPermissions were set successfully.\n";
}

bool PermissionsManager::HasPermission(std::string const& username, unsigned int feature) {
	int userPermissions = customersPermissionsMap[username];
	if (userPermissions == -1)
		return true;

	//0 means the admin didn't permit the customer from using any of the provided features.
	if (userPermissions == 0)
		return false;

	return (userPermissions & (1 << (feature - 1)));

}