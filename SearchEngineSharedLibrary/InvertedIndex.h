#pragma once
#include "WebPage.h"
#include <vector>
#include <map>

class InvertedIndex
{
public:
	// Breaks a file down into individual words and maps them according to file ID
	void TokenizeWebPageContent(const WebPage& webPage);

	// Returns a vector containing the page URL paired with the number of times the queried word appeared in the page
	std::vector<std::pair<std::string, int>> GetTokenFrequency(const std::string& query);

	// Load the inverted index from a binary file
	void LoadFromBinaryFile(const std::string& filename);

	// Save the inverted index to a binary file
	void SaveToBinaryFile(const std::string& filename);

private:
	std::map<std::string, std::map<std::string, int>> _index;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& _index;
	}
};