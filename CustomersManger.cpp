#include "CustomersManager.h"
#include "Helper.h"
#include <iostream>
#include "Admin.h"
#include <unordered_set>
#include "ReadingHistoriesManager.h"
#include <iomanip>

bool CustomersManager::LoadDatabse() {
    blockedUsernames.clear();
    customers.clear();
    std::vector<std::string> data;
    Helper::ReadFromFile(fileName, data);
    Helper::ReadFromFile(blockedUsersFileNames, blockedUsernames);
    
    if (data.size() == 0) {
        return false;
    }

    if (customers.size() != 0) {
        customers.clear();
    }

    for (auto const& line : data) {
        std::shared_ptr<Customer> customer(new Customer());
        customer->InitializeWithLine(line);
        ReadingHistoriesManager::GetInstance()->FillReadingCustomerHistories(customer);
        lastID = std::max(lastID, customer->GetID());
        customers.push_back(customer);
    }
    return true;
}

CustomersManager* CustomersManager::customersManager = nullptr;

CustomersManager::CustomersManager() : lastID(0) { LoadDatabse(); };

CustomersManager* CustomersManager::GetInstance() {
    if (customersManager == nullptr)
        customersManager = new CustomersManager();

    return customersManager;
}

void CustomersManager::ListCustomers() const {
    if (customers.size() != 0) {
        //Printing the header
        std::cout << "\n_______________________________________________________________________________________________________________\n\n";
        std::cout << "| " << std::left << std::setw(10) << "ID"
            << "| " << std::left << std::setw(20) << "Name"
            << "| " << std::left << std::setw(35) << "Email"
            << "| " << std::left << std::setw(15) << "Username"
            << "| " << std::left << std::setw(20) << "Permissions Values"
            << "|\n";
        std::cout << "_______________________________________________________________________________________________________________\n";

    }
    for (auto const& customer : customers)
        customer->Print();
}

//Returns false if the list is empty, and true otherwise
bool CustomersManager::ListBlockedCustomers() const {
    if (blockedUsernames.size() == 0)
        return false;

    int index = 0;
    std::cout << "\nUsernames of blocked users:\n";
    for (const auto& customer : blockedUsernames)
        std::cout << "\t" << ++index << ": " << customer << "\n";

    std::cout << "\n";
    return true;
}

std::vector<std::string> CustomersManager::GetUnblockedCustomers() {
    int idx = 0;
    std::vector<std::string> usernames;

    if (blockedUsernames.size() == 0) {
        for (const auto& customer : customers)
            usernames.push_back(customer->GetUsername());
    
        return usernames;
    }

    std::unordered_set<std::string> blockedSet(blockedUsernames.begin(), blockedUsernames.end());
    for (const auto& customer : customers)
        if (blockedSet.find(customer->GetUsername()) == blockedSet.end())
            usernames.push_back(customer->GetUsername());

    return usernames;
}

bool CustomersManager::RemoveCustomerFromBlackList(std::string const& username) {
    for (int i = 0; i < blockedUsernames.size(); ++i) {
        if (blockedUsernames[i] == username) {
            blockedUsernames.erase(blockedUsernames.begin() + i);
            UpdateDatabse();
            return true;
        }
    }
    return false;
}

void CustomersManager::UnBlockCustomerBy(int index) {
    std::string username = customers[index]->GetUsername();
    RemoveCustomerFromBlackList(blockedUsernames[index]);
    std::cout << "\nCustomer " << "\"" << username << "\"" << " Was Unblocked successfully.\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Press go back to main menu...";
    std::cin.get();
    std::cout << "\x1b[2J\x1b[H";
}

void CustomersManager::AddCustomerToBlackList(const std::string& username) {
    blockedUsernames.push_back(username);
    Helper::WriteLinesToFile(std::vector<std::string>(1, username), blockedUsersFileNames);
}

void CustomersManager::BlockCustomerBy(int index) {
    std::string username = customers[index]->GetUsername();
    AddCustomerToBlackList(username);
    std::cout << "\nCustomer " << "\"" << username << "\"" << " Was blocked successfully.\n";
}

void CustomersManager::AddCustomerToDatabase(std::shared_ptr<Customer> customer) {
    customers.push_back(customer);
    Helper::WriteLinesToFile(std::vector<std::string>(1, customer->ToString()), fileName);
}

bool CustomersManager::isUsernameUsed(const std::string& username) const {
    for (const auto& customer : customers)
        if (customer->GetUsername() == username)
            return true;

    return false;
}

bool CustomersManager::isBlockedCustomer(std::string const& username) {
    for (const auto& user : blockedUsernames)
        if (user == username)
            return true;

    return false;
}

bool CustomersManager::UpdateDatabse() const {
    std::vector<std::string> data;
    for (auto const& customer : customers)
        data.push_back(customer->ToString());

    if (data.size() == 0) {
        std::cout << "No customers available at the moment\n";
        return false;
    }

    Helper::WriteLinesToFile(data, fileName, false);
    Helper::WriteLinesToFile(blockedUsernames, blockedUsersFileNames, false);
    return true;
}

std::shared_ptr<Customer> CustomersManager::GetCustomer(std::string const& username, std::string const& password) {
    for (auto const& customer : customers)
        if (customer->GetUsername() == username && customer->GetPassword() == password)
            return customer;
    return nullptr;
}

std::vector<std::shared_ptr<Customer>> CustomersManager::GetCustomers() const {
    return customers;
}

size_t CustomersManager::GetCustomersSize() const {
    return customers.size();
}