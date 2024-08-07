#include "SearchEngine.h"
#include <algorithm>
#include <set>

void SearchEngine::IndexWebPage(WebPage webPage)
{
	_crawler.Crawl(webPage.GetWebPageUrl(), webPage);
	_webPageRepository.AddWebPage(webPage.GetWebPageID(), std::make_shared<WebPage>(webPage));
	_index.TokenizeWebPageContent(webPage);
}

std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> SearchEngine::Search(std::string& query)
{
	ParseQueryKeywords(query);

	std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> searchResults;

	// For each query word in the query words vector
	for (auto& word : _queryKeyWords)
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

std::vector<std::string>& SearchEngine::GetQueryKeyWords()
{
	return _queryKeyWords;
}

void SearchEngine::ParseQueryKeywords(std::string& query)
{
	// Define a list of stopwords
	std::set<std::string> stopwords = { "the", "and", "a", "an", "is", "in", "it", "of", "to" };

	// Split the query into individual words
	std::istringstream iss(query);
	std::string word;

	while (iss >> word)
	{
		// Brute force each query word to lowercase so we can query case agnostic
		std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });

		// Strip punctuation as well for now
		word.erase(std::remove_if(word.begin(), word.end(), [](char c) { return !isalnum(c); }), word.end());

		// Make sure the word isn't a stop word - not super important for querying
		if (stopwords.find(word) == stopwords.end())
		{
			// Not a stop word - add it
			_queryKeyWords.push_back(word);
		}
	}
}
