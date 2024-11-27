#pragma once
#include "Admin.h"

class AdminFlowController {
private:
	Admin admin;
	enum class AdminMenuOptions { ViewPrfile = 1, AddBook, BlockCustomer, UnBlockCustomer, ManagePermissions, Logout };
	enum class PermissionsMenuOptions { SetPermissions = 1, RemovePermission, AddPermission, Back };

public:
	void ShowMainMenu();
	void ShowPermissionsManagementMenu(std::vector<std::string> const& menuOptions);
	void TakeControl();
	bool isAdmin(std::string const& username, std::string const& password) const;
	void ViewProfile() const;
};