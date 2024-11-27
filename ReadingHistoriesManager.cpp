#include "ReadingHistoriesManager.h"
#include "Customer.h"
#include "Helper.h"
#include "CustomersManager.h"
#include "BooksManager.h"
#include "MyExceptions.h"

ReadingHistoriesManager::ReadingHistoriesManager() { LoadDatabase(); }

ReadingHistoriesManager* ReadingHistoriesManager::readingHistoriesManager = nullptr;

ReadingHistoriesManager* ReadingHistoriesManager::GetInstance() {
	if (readingHistoriesManager == nullptr)
		readingHistoriesManager = new ReadingHistoriesManager();
	return readingHistoriesManager;
}

//Customer has the ownership
ReadingHistoriesManager::~ReadingHistoriesManager() {
	delete readingHistoriesManager;
}

void ReadingHistoriesManager::ClearDatabase() const { 
	for (const auto& readingHistory : readingHistories) {
		if (readingHistory != nullptr)
			readingHistory->SetBook(nullptr);
	}
};

void ReadingHistoriesManager::AddBookToReadingHistory(std::shared_ptr<Book> book) {
	for (const auto& readingHistory : readingHistories) {
		if (readingHistory->GetBookID() == book->GetID())
			readingHistory->SetBook(book);
	}
}

void ReadingHistoriesManager::LoadDatabase() {	
	std::vector<std::string> data;
	Helper::ReadFromFile(directoryPath + fileName, data);

	for (const auto& line : data) {
		std::shared_ptr<ReadingHistory> readingHistory(new ReadingHistory());
		if (readingHistory->InitializeWithLine(line))
			readingHistories.push_back(readingHistory);
	}
}

void ReadingHistoriesManager::UpdateDatabase() const {
	std::vector<std::string> data;
	for (const auto& readingHistory : readingHistories)
		data.push_back(readingHistory->ToString());

	Helper::WriteLinesToFile(data, directoryPath + fileName, false);
}

//if a history is in the file with the same book ID it means already added before, should be updated not duplicated
void ReadingHistoriesManager::AddReadingHistoryToDatabase(std::shared_ptr<ReadingHistory> readingHistory, int userID) {
	fileName = std::to_string(userID) + ".txt";
	if (readingHistory == nullptr)
		throw DanglingException("NullPointerException", NULL);

	//Removing the old reading history and pushing back to new, then adding everything back to the file :)
	for (int pos = 0; pos < readingHistories.size(); ++pos) {
		if (readingHistories[pos]->GetBookID() == readingHistory->GetBookID()) {
			readingHistories.erase(readingHistories.begin() + pos);
			readingHistories.push_back(readingHistory);
			UpdateDatabase();
			return;
		}
	}

	Helper::WriteLinesToFile(std::vector<std::string>(1, readingHistory->ToString()), directoryPath + fileName);
}

void ReadingHistoriesManager::FillReadingCustomerHistories(std::shared_ptr<Customer> customer) {
	BooksManager* booksManager = BooksManager::GetInstance();
	           
	std::vector<std::string> data;
	fullPath = directoryPath + std::to_string(customer->GetID()) + ".txt";
	Helper::ReadFromFile(fullPath, data);
	for (const auto& line : data) {
		std::shared_ptr<ReadingHistory> readingHistory(new ReadingHistory());
		readingHistory->InitializeWithLine(line);
		BooksManager::GetInstance()->FillReadingHistoriesBooks(std::vector<std::shared_ptr<ReadingHistory>>(1, readingHistory));
		

		//Book* book = booksManager->GetBookBy(readingHistory->GetBookID());
		//readingHistory->SetBook(book);
		customer->AddReadingHistory(readingHistory);
	}
}