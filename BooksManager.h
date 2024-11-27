#pragma once
#include "Book.h"
#include <vector>
#include "ReadingHistory.h"
#include <memory>

class BooksManager {
private:
	std::vector<std::shared_ptr<Book>> books;
	const std::string fileName = "Books/books.txt";
	static BooksManager* booksManager;
	int lastID;
	BooksManager();
public:
	bool LoadDatabse();
	bool UpdateDatabase() const;
	void AddBookID(Book& book);
	void AddBookToDatabse(std::shared_ptr<Book> book);
	std::shared_ptr<Book> GetBookBy(int bookID);
	void FillReadingHistoriesBooks(std::vector<std::shared_ptr<ReadingHistory>> readingHistories);

	static BooksManager* GetInstance();
	void SetBookID(Book& book);
	void SetReadingHistoryBookBy(std::shared_ptr<ReadingHistory> readingHistory);
	void ListSelectFromAvailableBooks(std::shared_ptr<Customer> customer);

	~BooksManager();
};