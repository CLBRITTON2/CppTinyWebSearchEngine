#include "QueryParser.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

void QueryParser::LoadStopWords(std::string_view fileName)
{
	std::ifstream stopWordsFile("StopWords.txt");
    std::string line;
    if (!stopWordsFile.is_open())
    {
        std::cerr << "Failed to open stop words file." << std::endl;
        return;
    }

    while (getline(stopWordsFile, line))
    {
        _stopWords.push_back(line);
    }
}

std::vector<std::string> QueryParser::Parse(std::string& query)
{
    std::istringstream queryStream(query);
    std::string queryWord;
    std::vector<std::string> parsedQuery;

    while (queryStream >> queryWord)
    {
        // Brute force each query word to lowercase so we can query case agnostic
        std::transform(queryWord.begin(), queryWord.end(), queryWord.begin(), [](unsigned char c) { return std::tolower(c); });

        // Strip punctuation as well for now
        queryWord.erase(std::remove_if(queryWord.begin(), queryWord.end(), [](char c) { return !isalnum(c); }), queryWord.end());

        // Check if the word is not a stop word
        if (std::find(_stopWords.begin(), _stopWords.end(), queryWord) == _stopWords.end())
        {
            // Not a stop word - add it to the query keywords
            parsedQuery.push_back(queryWord);
        }
    }
	return parsedQuery;
}
