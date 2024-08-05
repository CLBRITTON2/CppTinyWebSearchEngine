#include <sstream>
#include <iostream>
#include "WebPageCrawler.h"
#include <curl/curl.h>
#include <lexbor/html/html.h>

void WebPageCrawler::Crawl(const std::string& webPageUrl, WebPage& webPage)
{
	CURL* curlHandle;
	CURLcode httpResponseCode;
	std::string webPageContent;

	// Create the cURL handle and validate it
	curlHandle = curl_easy_init();
	if (curlHandle)
	{
		//std::cout << "curlHandle successfully initialized" << std::endl;
		
		// Optional verbose logging for cURL issues
		//curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L);

		// Set the URL to fetch
		CURLcode setOptResult = curl_easy_setopt(curlHandle, CURLOPT_URL, webPageUrl.c_str());
		if (setOptResult != CURLE_OK) 
		{
			std::cerr << "curl_easy_setopt failed: " << curl_easy_strerror(setOptResult) << std::endl;
			return;
		}

		// Handle the recieved data - append the recieved data to the webPageContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);

		// Set the write data pointer to the webPageHtmlContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &webPageContent);

		// Execute the cURL request
		httpResponseCode = curl_easy_perform(curlHandle);

		//std::cout << webPageHtmlContent << std::endl;

		// Validate the response
		if (httpResponseCode != CURLE_OK)
		{
			std::cout << "cURL error: " << curl_easy_strerror(httpResponseCode);
		}
		else
		{
			// Good response extract text from the HTML content to populate the web page content
			std::string extractedText = ExtractTextFromHtml(webPageContent);
			webPage.SetWebPageContent(extractedText);
		}

		curl_easy_cleanup(curlHandle);
	}
	else
	{
		std::cout << "cURL initialization failed" << std::endl;
	}
}

std::string WebPageCrawler::ExtractTextFromHtml(const std::string& webPageContent) 
{
	lxb_status_t status;
	lxb_html_parser_t* htmlParser;
	lxb_html_document_t* document;

	htmlParser = lxb_html_parser_create();
	status = lxb_html_parser_init(htmlParser);
	if (status != LXB_STATUS_OK) 
	{
		return "";
	}

	 // Parse HTML 
	document = lxb_html_parse_chunk_begin(htmlParser);
	if (document == NULL)
	{
		std::cout << "Failed to parse HTML\n" << std::endl;;
		return "";
	}

	status = lxb_html_parse_chunk_process(htmlParser, (const lxb_char_t*)webPageContent.c_str(), webPageContent.length());
	if (status != LXB_STATUS_OK)
	{
		std::cout << "Failed to parse HTML chunk\n" << std::endl;
		return "";
	}

	status = lxb_html_parse_chunk_end(htmlParser);
	if (status != LXB_STATUS_OK)
	{
		std::cout << "Failed to parse HTML\n" << std::endl;
		return "";
	}

	lxb_dom_node_t* root = lxb_dom_interface_node(document);

	std::string extractedText;

	try
	{
		SerializeTextContent(root, extractedText);
	}
	catch (const std::exception& e)
	{
		std::cout << "Error serializing text content: " << e.what() << std::endl;
		lxb_html_document_destroy(document);
		lxb_html_parser_destroy(htmlParser);
		return "";
	}

	// Clean up
	lxb_html_document_destroy(document);
	lxb_html_parser_destroy(htmlParser);

	return extractedText;
}

size_t WebPageCrawler::WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData)
{
	((std::string*)userData)->append((char*)buffer, bufferSize * numberOfBlocks);
	return bufferSize * numberOfBlocks;
}

void WebPageCrawler::SerializeTextContent(lxb_dom_node_t* node, std::string& extractedText)
{
	if (node->type == LXB_DOM_NODE_TYPE_TEXT)
	{
		size_t len;
		lxb_char_t* text = lxb_dom_node_text_content(node, &len);
		std::string filteredText;

		for (size_t i = 0; i < len; ++i)
		{
			char character = static_cast<char>(text[i]);
			if (isalnum(static_cast<unsigned char>(character)) || isspace(static_cast<unsigned char>(character)))
			{
				extractedText.push_back(character);
			}
		}

	}
	else
	{
		lxb_dom_node_t* child = lxb_dom_node_first_child(node);
		while (child != NULL)
		{
			SerializeTextContent(child, extractedText);
			child = child->next;
		}
	}
}
