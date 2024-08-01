#include <iostream>
#include <sstream>
#include "InvertedIndex.h"
#include "WebPage.h"
#include <lexbor/html/token.h>
#include <lexbor/html/tokenizer.h>

static lxb_html_token_t* TokenCallback(lxb_html_tokenizer_t* tkz, lxb_html_token_t* token, void* ctx)
{
	InvertedIndex* index = static_cast<InvertedIndex*>(ctx);
	if (token->tag_id == LXB_TAG__TEXT)
	{
		std::string tokenText(reinterpret_cast<const char*>(token->text_start), token->text_end - token->text_start);
		std::istringstream wordStream(tokenText);
		std::string word;
		while (wordStream >> word)
		{
			index->AddTokenToIndex(word, index->CurrentWebPageID);
		}
	}
	return token;
}

void InvertedIndex::IndexWebPageContent(const WebPage& webPage)
{
	CurrentWebPageID = webPage.GetWebPageID();
	lxb_status_t status;
	lxb_html_tokenizer_t* tokenizer;

	tokenizer = lxb_html_tokenizer_create();
	status = lxb_html_tokenizer_init(tokenizer);
	if (status != LXB_STATUS_OK) 
	{
		std::cerr << "Failed to create tokenizer object" << std::endl;
	}

	// Set callback for token
	lxb_html_tokenizer_callback_token_done_set(tokenizer, TokenCallback, this);

	status = lxb_html_tokenizer_begin(tokenizer);
	if (status != LXB_STATUS_OK) 
	{
		std::cerr << "Failed to prepare tokenizer object for parsing" << std::endl;
	}

	status = lxb_html_tokenizer_chunk(tokenizer, (const lxb_char_t*)webPage.GetWebPageContent().c_str(), webPage.GetWebPageContent().size() - 1);
	if (status != LXB_STATUS_OK) 
	{
		std::cerr << "Failed to parse the html data" << std::endl;
	}

	status = lxb_html_tokenizer_end(tokenizer);
	if (status != LXB_STATUS_OK) 
	{
		std::cout << "Failed to ending of parsing the html data" << std::endl;
	}

	lxb_html_tokenizer_destroy(tokenizer);
}

void InvertedIndex::AddTokenToIndex(const std::string& token, int webPageID)
{
	_index[token][webPageID]++;
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
