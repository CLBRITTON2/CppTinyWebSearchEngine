#pragma once
#include "WebPage.h"
#include <vector>
#include <map>

class InvertedIndex
{
public:
	// Needs a method for adding web pages to a collection - takes a web page object and returns nothing
	// Breaks the file down into individual words, updates inverted index with frequency of each word in the page
	void IndexWebPageContent(const WebPage& webPage);

	// Needs a method to get web pages from said collection using a search term that returns a collection of applicable web pages
	// Initial implementation will store the page Id and number of occurences of each queried word
	// TODO: Switch to a proximity index to store words and position to allow query phrases instead of just words 

	// Returns a vector containing the page ID paired with the number of times the queried word appeared in the page
	std::vector<std::pair<int, int>> Search(const std::string& query);

private:
	std::map<std::string, std::map<int, int>> _index;
};