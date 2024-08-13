#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include "InvertedIndex.h"
#include "WebPage.h"
#include <fstream>
#include <iostream>
#include <filesystem>

void InvertedIndex::TokenizeWebPageContent(const WebPage& webPage)
{
	int webPageID = webPage.GetWebPageID();
	std::stringstream stringStream(webPage.GetWebPageContent());
	std::string token;

	// Read the content of the web page
	while (stringStream >> token)
	{
		// Increment the frequency of each token for the current web page
		_index[token][webPageID]++;
	}
}

std::vector<std::pair<int, int>> InvertedIndex::GetTokenFrequency(const std::string& query)
{
	std::vector<std::pair<int, int>> searchResults;

	if (_index.find(query) != _index.end())
	{
		for (const auto& pageIdFrequency : _index[query])
		{
			searchResults.push_back(pageIdFrequency);
		}
	}
	else
	{
		std::cout << "Key doesn't exist: Coldn't find any instance of \"" << query << "\" in provided web pages" << std::endl;
		return {};
	}
	return searchResults;
}

// Load the _index map from binary
void InvertedIndex::LoadFromBinaryFile(const std::string& fileName)
{
	if (!std::filesystem::exists(fileName)) 
	{
		std::cout << "File not found: " + fileName << std::endl;
		return;
	}

	std::ifstream inFile(fileName, std::ios::binary);
	if (!inFile.is_open())
	{
		throw std::runtime_error("Failed to open file for reading: " + fileName);
	}

	boost::archive::binary_iarchive ia(inFile);
	ia >> _index;
}

// Save the _index map to binary
void InvertedIndex::SaveToBinaryFile(const std::string& fileName)
{
	if (_index.empty()) 
	{
		std::cerr << "Index is empty, nothing to serialize" << std::endl;
		return;
	}

	std::ofstream outFile(fileName, std::ios::binary);
	if (!outFile.is_open())
	{
		std::cerr << "Failed to open file for writing" << std::endl;
	}

	try
	{
		boost::archive::binary_oarchive oa(outFile);
		oa << _index;
	}
	catch (const boost::archive::archive_exception& e)
	{
		std::cerr << "Serialization failed: " << e.what() << std::endl;
	}

	if (outFile.fail() || outFile.bad())
	{
		std::cerr << "Error writing to file" << std::endl;
	}

	if (outFile.good()) 
	{
		outFile.flush();
		outFile.close();
	}
	else 
	{
		std::cerr << "Error writing to file" << std::endl;
	}
}

