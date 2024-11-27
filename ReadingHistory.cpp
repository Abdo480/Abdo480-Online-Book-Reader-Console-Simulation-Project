#include "ReadingHistory.h"
#include "Customer.h"
#include "Helper.h"
#include "BookContent.h"
#include <iomanip>
#include <iostream>

ReadingHistory::ReadingHistory() : currentPage(0), numberOfPages(-1), book(nullptr){}


int ReadingHistory::GetBookID() const { return  bookID; }

std::shared_ptr<Book> ReadingHistory::GetBook() const {
	if (book != nullptr)
		return book;
	return nullptr;
}

int ReadingHistory::GetCustomerID() { return customerID; }

int ReadingHistory::GetID() const { return customerID; }

void ReadingHistory::SetCustomerID(int ID) { this->customerID = ID; }

void ReadingHistory::DisplayProgress() const {
	std::cout << "Current Page: " << GetCurrentPage() << "/" << GetNumberOfPages() << "\n";
}

void ReadingHistory::SetBook(std::shared_ptr<Book> book) { 
	if (book != nullptr) {
		this->book = book;
		this->bookID = this->book->GetID();
	}
}

bool ReadingHistory::InitializeWithLine(std::string const& line) {

	std::vector<std::string> data = Helper::SplitString(line);
	if (data.size() == 0) {
		std::cout << "reading histy data is empty\n";
		return false;
	}

	if (data.size() != 5) {
		std::cout << "Something wrong with the reading history line format, try again\n";
		return false;
	}

	currentPage = Helper::ToInt(data[0]);
	numberOfPages = Helper::ToInt(data[1]);
	//ID same as customer ID
	customerID = Helper::ToInt(data[2]);
	bookID = Helper::ToInt(data[3]);
	lastSession = data[4];
	return true;
}

void ReadingHistory::SetBookName(std::string const& bookName) {
	this->bookName = bookName;
}

void ReadingHistory::PrintReadingHistory() const {
	std::ostringstream progressStream;
	progressStream << (GetCurrentPage() + 1) << "/" << GetNumberOfPages();
	std::string progress = progressStream.str();

	std::cout << "| " << std::left
		      << std::setw(20) << book->GetName()
		      << "| " << std::left
		      << std::setw(27) << progress
		      << "| " << std::left
		      << std::setw(22) << GetLastSession()
		      << "|\n";
}

//Sample
//currentPage, numberOfPages,  customerID,   bookID,              lastSession
//        3            6            1         1        2024 - 11 - 20 16:11 : 36
std::string ReadingHistory::ToString() const {
	std::ostringstream oss;
	oss << currentPage + 1 << "," << numberOfPages << "," << customerID << "," << bookID << "," << GetLastSession();
	return oss.str();
}

void ReadingHistory::DisplayLastReadingHistory() const {
	std::cout << book->GetName() << " Page: " << GetCurrentPage() << "/" << GetNumberOfPages() << " - "<< lastSession;
}

//Getters & Setters
int ReadingHistory::GetNumberOfPages() const { return numberOfPages; }

void ReadingHistory::SetNumberOfPages(int numberOfPages) { this->numberOfPages = numberOfPages; }

int ReadingHistory::GetCurrentPage() const { return currentPage; }

void ReadingHistory::SetCurrentPage(int currentPage) { this->currentPage = currentPage; }

const std::string& ReadingHistory::GetLastSession() const { return lastSession; }

void ReadingHistory::SetLastSession(std::string const& lastSession) { this->lastSession = lastSession; }
