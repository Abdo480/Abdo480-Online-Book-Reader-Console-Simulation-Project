#include "Book.h"
#include "BookContent.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include "Helper.h"
#include <sstream>
#include "booksManager.h"
#include "MyExceptions.h"


Book::Book() : ID(-1), bookContent(nullptr) {}

bool Book::InitializeWithLine(std::string const& line) {
	
	std::vector<std::string> data = Helper::SplitString(line);
	if (data.size() != 3)
		return false;

	name = data[0];
	authorName = data[1];
	ID = Helper::ToInt(data[2]);

	bookContent->SetBookID(ID);
	bookContent->SetBook(std::shared_ptr<Book>(this));
	bookContent->LoadBookContent();
	return true;
}

std::string Book::ToString() {
	std::ostringstream oss;
	oss << name << "," << authorName << "," << ID;
	return oss.str();
}

void Book::Read() {
	BooksManager::GetInstance()->SetBookID(*this);

	std::cout << "\nEnter The Following info:\n";
	std::cout << "\tName: ";
	do {
		std::getline(std::cin, name);
	} while (name.empty());

	std::cout << "\tAuthor name: ";
	std::getline(std::cin, authorName);

	std::shared_ptr<BookContent> bookContent(new BookContent);
	SetBookContent(bookContent);
	bookContent->SetBookID(ID);
	bookContent->Read();
}

//Setters & getters
const std::string& Book::GetName() const { return name; }
void Book::SetName(std::string const& name) { this->name = name; }

const std::string& Book::GetAuthorName() const { return authorName; };
void Book::SetAuthorName(std::string const& authorName) { this->authorName = authorName; }

int Book::GetID() const { return ID; };
void Book::SetID(int ID) { this->ID = ID; }

std::shared_ptr<BookContent> Book::GetBookContent() {
	if (bookContent == nullptr)
		throw DanglingException("NullPointerException", NULL);
	return bookContent;
}

void Book::SetBookContent(std::shared_ptr<BookContent> bookContent) {
	this->bookContent = bookContent;
}