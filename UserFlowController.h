#pragma once
#include "CustomerFlowController.h"
#include "CustomersManager.h"
#include "AdminFlowController.h"

class UserFlowController {
private:
	static CustomerFlowController* customerFlowController;
	static CustomersManager* customersManager;
	AdminFlowController adminFlowController;
	void Login();
	void SignUp();
public:
	void ShowMainMenu();
	UserFlowController() {

	}
};