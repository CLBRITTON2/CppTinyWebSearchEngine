#pragma once
#include "WebPage.h"
#include <vector>
#include <map>

class InvertedIndex
{
public:
	// Breaks a file down into individual words and maps them according to file ID
	void IndexWebPageContent(const WebPage& webPage);
	void AddTokenToIndex(const std::string& token, int webPageID);
	int CurrentWebPageID;

	// Initial implementation will store the page Id and number of occurences of each queried word
	// TODO: Switch to a proximity index to store words and position to allow query phrases instead of just words 

	// Returns a vector containing the page ID paired with the number of times the queried word appeared in the page
	std::vector<std::pair<int, int>> GetTokenFrequency(const std::string& query);

private:
	std::map<std::string, std::map<int, int>> _index;
};