#include "BookContent.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <ctime>
#include <chrono>
#include "ReadingHistory.h"
#include "ReadingHistoriesManager.h"
#include "MyExceptions.h"

BookContent::BookContent() :  wordsPerLine(30), linesPerPage(30), currentPage(0) {
	CalculateNumberOfPages();
}

BookContent::BookContent(int wordsPerLine, int linesPerPage) : wordsPerLine(wordsPerLine), linesPerPage(linesPerPage), currentPage(0) {
	CalculateNumberOfPages();
}


void BookContent::CalculateNumberOfPages() { numberOfPages = ((int)text.size() + linesPerPage - 1) / linesPerPage; }

void BookContent::SetLinesPerPage(int linesPerPage) { this->linesPerPage = linesPerPage; }

void BookContent::SetBookID(int bookID) {
	this->bookID = bookID;
	fileName = std::to_string(bookID) + ".txt";
	fullPath = directoryPath + fileName;
	LoadBookContent();
}

void BookContent::LoadBookContent() {
	std::vector<std::string> data;
	Helper::ReadFromFile(directoryPath + fileName, data);

	if (data.empty()) {
		return;
	}

	std::istringstream settingsStream(data[0]);
	settingsStream >> wordsPerLine >> linesPerPage;

	text.assign(data.begin() + 1, data.end());
}

void BookContent::UpdateDatabase() const {
	std::vector<std::string> outputData;
	outputData.push_back(std::to_string(wordsPerLine) + " " + std::to_string(linesPerPage));

	outputData.insert(outputData.end(), text.begin(), text.end());

	Helper::WriteLinesToFile(outputData, directoryPath + fileName, false);

}


void BookContent::DisplayCurrentPage() const {
	if (currentPage + 1 > numberOfPages) {
		std::cout << "\nNo more pages :).\n";
		return;
	}

	int startLine = currentPage * linesPerPage;
	int endLine = std::min(startLine + linesPerPage, (int)text.size());

	std::cout << "\n\t\t-- Page " << (currentPage + 1 > numberOfPages ? currentPage : currentPage + 1) << " of " << numberOfPages << " --\n\n";
	for (int line = startLine; line < endLine; ++line) {
		std::cout << "\t\t" << text[line] << "\n";
	}
}

void BookContent::ReadBook(std::shared_ptr<Customer> reader, std::shared_ptr<Book> book) {
	LoadBookContent();

	for (const auto& readingHistory : reader->GetReadingHistories()) {
		if (readingHistory->GetBookID() == book->GetID())
			currentPage = readingHistory->GetCurrentPage();
	}

	if (text.empty()) {
		std::cerr << "\nThe Book Content is empty.\n";
		return;
	}

	CalculateNumberOfPages();

	if (currentPage >= numberOfPages)
		currentPage = numberOfPages - 1;

	if (currentPage < 0)
		currentPage = 0;

	int choice;
	do {
		std::cout << "\x1b[2J\x1b[H";
		DisplayCurrentPage();
		choice = Helper::ReadMenuOption({"Next", "Previous", "Quit"});
		if (choice == static_cast<int>(PeginagtionControls::Next)) {
			if (currentPage < numberOfPages)
				++currentPage;
		}
		else if (choice == static_cast<int>(PeginagtionControls::Previous)) {
			if (currentPage > 0)
				--currentPage;
		}
		else {
			std::shared_ptr<ReadingHistory> readingHistory(new ReadingHistory());
			readingHistory->SetBook(book);
			readingHistory->SetCurrentPage(currentPage);
			readingHistory->SetCustomerID(reader->GetID());
			readingHistory->SetLastSession(Helper::GetCurrentDataTime());
			readingHistory->SetNumberOfPages(numberOfPages);
			readingHistory->SetBookName(book->GetName());
			ReadingHistoriesManager::GetInstance()->AddReadingHistoryToDatabase(readingHistory, reader->GetID());
			reader->AddReadingHistory(readingHistory);
		}

	} while (choice != static_cast<int>(PeginagtionControls::Quit) );
	
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);

	//Reading History ID is the same as its book ID, for loading the book later when loading the database
}

void BookContent::Read() {
	std::cout << "\nEnter The Settings:\n";
	std::cout << "\tHow many words per line (1 - 100): ";

	while (!(std::cin >> wordsPerLine) || wordsPerLine < 1 || wordsPerLine > 100) {
		if (!std::cin)
			std::cout << "\nInvalid input, number must be a numeric value, try again.\n";
		else
			std::cout << "\ninput out of range, try again\n";

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << "\tHow many words per line (1 - 100): ";
	}
	std::cout << "\tHow many lines per page (1 - 100): ";
	while (!(std::cin >> linesPerPage) || linesPerPage < 1 || linesPerPage > 100) {
		if (!std::cin)
			std::cout << "\nInvalid input, number must be a numeric value, try again.\n";
		else
			std::cout << "\ninput out of range, try again\n";

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << "\tHow many lines per page (1 - 100): ";
	}


	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	char answer = 'y';
	std::string fullText, moreText;
	while (tolower(answer) == 'y') {
		std::cout << "\tEnter Book content text: ";
		std::getline(std::cin, moreText, '\n');
		fullText += moreText;

		std::cout << "\nDo you want to add more (Y/N): ";
		std::cin >> answer;
		if (std::tolower(answer) == 'y') {
			std::string MoreText;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nKeep writing :) ";
			/*std::getline(std::cin, MoreText);
			fullText += MoreText;*/
			continue;
		}
		else
			break;
	}

	//Proccess words
	std::istringstream inputStream(fullText);
	int wordsCounter = 0;
	std::string line, word;

	while (inputStream >> word) {
		++wordsCounter;
		if (!line.empty()) line += " ";
		line += word;

		if (wordsCounter >= wordsPerLine) {
			wordsCounter = 0;
			text.push_back(line);
			line.clear();
		}
	}

	if (!line.empty())
		text.push_back(line);

	CalculateNumberOfPages();
	UpdateDatabase();
}

bool BookContent::isTextEmpty() const	{
	return text.size() == 0;
}

int BookContent::GetWordsPerLine() const { return wordsPerLine; }
void BookContent::SetWordsPerLine(int wordsPerLine) { this->wordsPerLine = wordsPerLine; }

void BookContent::SetBook(std::shared_ptr<Book> book_) {
	if (book_ == nullptr)
		throw DanglingException("NullPointerException", NULL);

	if (this->book != nullptr) {
		if (bookID == -1)
			this->bookID = book->GetID();
		return;
	}

	this->book = book_;
	this->bookID = book_->GetID();
		
}

const std::vector<std::string>& BookContent::GetText() const { return text; }
void BookContent::SetText(std::vector<std::string> const& text) { this->text = text; }