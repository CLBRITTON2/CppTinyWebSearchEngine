#include "WebPageCrawler.h"
#include <curl/curl.h>
#include <iostream>

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
		curl_easy_cleanup(curlHandle);
	}
}
