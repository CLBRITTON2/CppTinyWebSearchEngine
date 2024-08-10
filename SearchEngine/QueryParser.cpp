#include "QueryParser.h"
#include <algorithm>

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

void QueryParser::SetLemmatizer(RdrLemmatizer* lemmatizer)
{
    _lemmatizer = lemmatizer;
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

        // Make sure lemmatizer isn't nullptr and queryword isn't empty before lemmatizing
        if (_lemmatizer && !queryWord.empty())
        {
            // Lemmatize each word
            char* lemmatizedWord = _lemmatizer->Lemmatize(queryWord.c_str());
            queryWord = lemmatizedWord;
            delete[] lemmatizedWord;
        }

        // Check if the word is not a stop word
        if (std::find(_stopWords.begin(), _stopWords.end(), queryWord) == _stopWords.end())
        {
            // Not a stop word - add it to the query keywords
            parsedQuery.push_back(queryWord);
        }
    }
	return parsedQuery;
}

QueryParser::QueryParser()
{
    LoadStopWords("StopWords.txt");
}
