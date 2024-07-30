#include "WebPageCrawler.h"
#include <curl/curl.h>
#include <iostream>
#include <rapidxml.hpp>

void WebPageCrawler::Crawl(const std::string& webPageUrl, WebPage& webPage)
{
	CURL* curlHandle;
	CURLcode httpResponseCode;
	std::string webPageHtmlContent;

	// Create the cURL handle and validate it
	curlHandle = curl_easy_init();
	if (curlHandle)
	{
		// Set the URL to fetch
		curl_easy_setopt(curlHandle, CURLOPT_URL, webPageUrl.c_str());

		// Handle the recieved data - append the recieved data to the webPageHtmlContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, [](void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData)
			{
				((std::string*)userData)->append((char*)buffer, bufferSize * numberOfBlocks);
				return bufferSize * numberOfBlocks;
			});

		// Set the write data pointer to the webPageHtmlContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &webPageHtmlContent);

		// Execute the cURL request
		httpResponseCode = curl_easy_perform(curlHandle);

		// Validate the response
		if (httpResponseCode != CURLE_OK)
		{
			std::cerr << "cURL error: " << curl_easy_strerror(httpResponseCode);
		}
		else
		{
			// Good response extract text from the HTML content to populate the web page content
			std::string extractedText = ExtractTextFromHtml(webPageHtmlContent);
			webPage.SetWebPageContent(extractedText);
		}

		curl_easy_cleanup(curlHandle);
	}
}

std::string WebPageCrawler::ExtractTextFromHtml(const std::string& webPageHtmlContent)
{
	std::string textContent;

	// Points to a new instance of RapidXML and deletes the instance when it's out of scope
	std::unique_ptr<rapidxml::xml_document<>> doc(new rapidxml::xml_document<>);

	// Get a pointer to the beginning of the HTML content string
	// Rapid XML requires a non-const pointer so it must be casted
	char* start = const_cast<char*>(webPageHtmlContent.c_str());
	doc->parse<0>(start);

	// Iterate over the nodes in the parsed document
	rapidxml::xml_node<>* node = doc->first_node();
	while (node)
	{	
		// Only extract the text value of the node if it is a text node
		if (node->type() == rapidxml::node_data) 
		{
			textContent += node->value();
		}
		node = node->next_sibling();
	}

	return textContent;
}
