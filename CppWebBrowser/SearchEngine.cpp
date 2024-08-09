#include "SearchEngine.h"

SearchEngine::SearchEngine()
{
	_lemmatizer.LoadBinary("english.bin");
	_queryParser.SetLemmatizer(&_lemmatizer);
}

void SearchEngine::LoadRepositoryFromBinaryFile(const std::string& fileName)
{
	_webPageRepository.LoadFromBinaryFile(fileName + ".repository");
	_index.LoadFromBinaryFile(fileName + ".index");
}

std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> SearchEngine::Search(std::string& query)
{
	std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> searchResults;

	std::vector<std::string> parsedSearchQuery = _queryParser.Parse(query);

	// For each query word in the parsed query words vector
	for (auto& word : parsedSearchQuery)
	{
		// Get token frequencies for each query keyword
		std::vector<std::pair<int, int>> tokenFrequencies = _index.GetTokenFrequency(word);

		for (const auto& result : tokenFrequencies)
		{
			std::shared_ptr<WebPage> webPage = _webPageRepository.GetWebPageById(result.first);

			// Store the frequency count of the individual word
			searchResults[webPage].first[word] += result.second;

			// Store the total frequency of all keywords from the query
			searchResults[webPage].second += result.second;
		}
	}
	return searchResults;
}

