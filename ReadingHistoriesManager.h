#pragma once
#include "ReadingHistory.h"
#include <vector>
#include <memory>

class ReadingHistoriesManager {
private:
	std::vector<std::shared_ptr<ReadingHistory>> readingHistories;
	const std::string directoryPath = "Users/ReadingHistories/";
	std::string fileName;
	std::string fullPath;

	static ReadingHistoriesManager* readingHistoriesManager;
	ReadingHistoriesManager();

public:
	void LoadDatabase();
	void UpdateDatabase() const;
	void FillReadingCustomerHistories(std::shared_ptr<Customer> customer);
	void AddReadingHistoryToDatabase(std::shared_ptr<ReadingHistory> readingHistory, int userID);
	static ReadingHistoriesManager* GetInstance();
	void AddBookToReadingHistory(std::shared_ptr<Book> book);
	void ClearDatabase() const;
	~ReadingHistoriesManager();
};