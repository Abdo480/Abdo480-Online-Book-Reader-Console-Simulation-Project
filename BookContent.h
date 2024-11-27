#pragma once
#include <string>
#include "Admin.h"
#include <vector>
#include "Helper.h"
#include "Book.h"
#include "Customer.h"
#include <memory>

class BookContent {
private:
	int wordsPerLine;
	int linesPerPage;
	int currentPage;
	int numberOfPages;
	std::vector<std::string> text;
	std::shared_ptr<Book> book;
	int bookID;
	void Read();
	void SetText(std::vector<std::string> const& text);
	
	enum class PeginagtionControls {
		Next = 1, Previous, Quit
	};


	const std::string directoryPath = "Books/BooksContents/";
	std::string fileName;
	std::string fullPath;

	void DisplayCurrentPage() const;

public:
	friend class Admin;
	friend class Book;

	//Constructors
	BookContent();
	BookContent(int wordsPerLine, int linesPerPage);

	bool isTextEmpty() const;
	//Customer can change wordsPerLine and update the database and load it back, but only the admin can read and edit the book content
	void LoadBookContent();
	void UpdateDatabase() const;
	void ReadBook(std::shared_ptr<Customer> reader, std::shared_ptr<Book> book);
	void CalculateNumberOfPages();

	//Setters & Getters
	int GetWordsPerLine() const;
	void SetWordsPerLine(int wordsPerLine);
	void SetBookID(int bookID);
	void SetLinesPerPage(int linesPerPage);
	void SetBook(std::shared_ptr<Book> book);

	const std::vector<std::string>& GetText() const;

};