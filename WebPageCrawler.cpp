#include <sstream>
#include <iostream>
#include "WebPageCrawler.h"
#include <curl/curl.h>
#include <lexbor/html/html.h>


void WebPageCrawler::Crawl(const std::string& webPageUrl, WebPage& webPage)
{
	CURL* curlHandle;
	CURLcode httpResponseCode;
	std::string webPageHtmlContent;

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

		// Handle the recieved data - append the recieved data to the webPageHtmlContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);

		// Set the write data pointer to the webPageHtmlContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &webPageHtmlContent);

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
			std::string extractedText = ExtractTextFromHtml(webPageHtmlContent);
			webPage.SetWebPageContent(extractedText);
		}

		curl_easy_cleanup(curlHandle);
	}
	else
	{
		std::cout << "cURL initialization failed" << std::endl;
	}
}

std::string WebPageCrawler::ExtractTextFromHtml(const std::string& webPageHtmlContent) 
{
	lxb_status_t status;
	lxb_html_parser_t* parser;
	lxb_html_document_t* doc;

	parser = lxb_html_parser_create();
	status = lxb_html_parser_init(parser);
	if (status != LXB_STATUS_OK) 
	{
		return "";
	}

	// Parse the HTML content
	doc = lxb_html_parse(parser, (const lxb_char_t*)webPageHtmlContent.c_str(), webPageHtmlContent.size());
	if (doc == NULL) 
	{
		lxb_html_parser_destroy(parser);
		return "";
	}

	std::string extractedText;

	// Serialize the HTML tree and extract text - skip comments - I believe comments will skew search results but unsure
	status = lxb_html_serialize_pretty_tree_cb(lxb_dom_interface_node(doc), LXB_HTML_SERIALIZE_OPT_SKIP_COMMENT, 0, SerializerCallback, &extractedText);

	if (status != LXB_STATUS_OK) 
	{
		lxb_html_document_destroy(doc);
		lxb_html_parser_destroy(parser);
		return "";
	}

	// Clean up
	lxb_html_document_destroy(doc);
	lxb_html_parser_destroy(parser);

	return extractedText;
}

unsigned int WebPageCrawler::SerializerCallback(const lxb_char_t* data, size_t len, void* ctx) 
{
	std::string* extractedText = static_cast<std::string*>(ctx);
	std::string text(data, data + len);


	// Preprocess the text (can't find a way through lexbor to remove formatting chars etc)
	std::string cleanText;
	for (char character : text) {
		if (isalnum(static_cast<unsigned char>(character)) || isspace(static_cast<unsigned char>(character))) {
			cleanText += character;
		}
	}

	extractedText->append(cleanText);
	return 0;
}

size_t WebPageCrawler::WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData)
{
	((std::string*)userData)->append((char*)buffer, bufferSize * numberOfBlocks);
	return bufferSize * numberOfBlocks;
}
