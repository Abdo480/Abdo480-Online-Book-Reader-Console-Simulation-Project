#pragma once
#include <string>
#include <sstream>
#include <ctime>
#include <memory>

class BookContent;

class Book {
private:
	std::string name;
	std::string authorName;
	int ID;
	std::shared_ptr<BookContent> bookContent;
	void Read();

public:
	friend class Admin;
	friend class BooksManager;
	bool InitializeWithLine(std::string const& line);
	std::string ToString();
	//Constructors
	Book();

	//Setters & getters
	const std::string& GetName() const;
	void SetName(std::string const& name);

	const std::string& GetAuthorName() const;
	void SetAuthorName(std::string const& authorName);

	int GetID() const;
	void SetID(int ID);

	std::shared_ptr<BookContent> GetBookContent();
	void SetBookContent(std::shared_ptr<BookContent> bookContent);
};
