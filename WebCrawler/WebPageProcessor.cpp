#include "WebPageProcessor.h"
#include <curl/curl.h>
#include <regex>
#include <stack>

size_t WebPageProcessor::WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData)
{
	((std::string*)userData)->append((char*)buffer, bufferSize * numberOfBlocks);
	return bufferSize * numberOfBlocks;
}

WebPageProcessor::WebPageProcessor()
{
	_lemmatizer.LoadBinary("english.bin");
}

// Sets a web pages content to an extracted HTML string via http request
void WebPageProcessor::ProcessWebPage(WebPage& webPage, std::queue<string>& urlQueue)
{
	// Get the web page URL
	std::string webPageUrl = webPage.GetWebPageUrl();

	// Get the contents of a web page and place it in raw html string
	std::string rawHtmlString = GetContentFromHttpRequest(webPageUrl, webPage);

	// Pass the raw html string to html extractor which Extracts URLS and serializes all words on a page
	// It's ugly and violates SRP - but the memory management with LXB library would be atrociouis without this - maybe refactor later
	std::string extractedText = ExtractTextFromHtml(rawHtmlString, urlQueue);

	// Set the current web page content to the stripped text representation of the web page
	webPage.SetWebPageContent(extractedText);
}

// HTTP request returns a massive string of unserialized webpage HTML data
std::string WebPageProcessor::GetContentFromHttpRequest(const std::string& webPageUrl, WebPage& webPage)
{
	CURL* curlHandle;
	CURLcode httpResponseCode;
	std::string webPageContent = "";

	// Create the cURL handle and validate it
	curlHandle = curl_easy_init();
	if (curlHandle)
	{
		// Set the URL to fetch
		curl_easy_setopt(curlHandle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
		CURLcode setOptResult = curl_easy_setopt(curlHandle, CURLOPT_URL, webPageUrl.c_str());
		// Only allow redirects to HTTP and HTTPS URLs
		curl_easy_setopt(curlHandle, CURLOPT_REDIR_PROTOCOLS_STR, "http,https");
		curl_easy_setopt(curlHandle, CURLOPT_AUTOREFERER, 1L);
		curl_easy_setopt(curlHandle, CURLOPT_MAXREDIRS, 10L);

		// skip files larger than a gigabyte
		curl_easy_setopt(curlHandle, CURLOPT_MAXFILESIZE_LARGE, (curl_off_t)1024 * 1024 * 1024);

		// Each transfer needs to be done within 20 seconds
		curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT_MS, 20000L);

		// connect fast or fail 
		curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT_MS, 2000L);

		if (setOptResult != CURLE_OK)
		{
			std::cerr << "curl_easy_setopt failed: " << curl_easy_strerror(setOptResult) << std::endl;
			curl_easy_cleanup(curlHandle);
			return "";
		}

		// Handle the recieved data - append the recieved data to the webPageContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);

		// Set the write data pointer to the webPageContent string
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &webPageContent);

		// Execute the cURL request
		httpResponseCode = curl_easy_perform(curlHandle);

		// Validate the response
		if (httpResponseCode != CURLE_OK)
		{
			std::cout << "cURL error: " << curl_easy_strerror(httpResponseCode) << std::endl;
			curl_easy_cleanup(curlHandle);
			return "";
		}
		else
		{
			// Good response return the content
			return webPageContent;
		}

		curl_easy_cleanup(curlHandle);
	}
	else
	{
		std::cout << "cURL initialization failed" << std::endl;
		return "";
	}
}

std::string WebPageProcessor::ExtractTextFromHtml(const std::string& webPageContent, std::queue<string>& urlQueue)
{
	lxb_status_t status;
	lxb_html_parser_t* htmlParser;
	lxb_html_document_t* document;

	// Make lemmatizer threadsafe - multiple threads trying to use the lemmatizer = lemmatizer destructor error
	std::lock_guard<std::mutex> lock(_lemmatizerMutex);

	htmlParser = lxb_html_parser_create();
	status = lxb_html_parser_init(htmlParser);
	if (status != LXB_STATUS_OK)
	{
		lxb_html_parser_destroy(htmlParser);
	}

	// Parse HTML 
	document = lxb_html_parse_chunk_begin(htmlParser);
	if (document == NULL)
	{
		std::cout << "Failed to parse HTML\n" << std::endl;;
		CleanupLXB(htmlParser, document);
	}

	status = lxb_html_parse_chunk_process(htmlParser, (const lxb_char_t*)webPageContent.c_str(), webPageContent.length());
	if (status != LXB_STATUS_OK)
	{
		std::cout << "Failed to parse HTML chunk\n" << std::endl;
		CleanupLXB(htmlParser, document);
	}

	status = lxb_html_parse_chunk_end(htmlParser);
	if (status != LXB_STATUS_OK)
	{
		std::cout << "Failed to parse HTML\n" << std::endl;
		CleanupLXB(htmlParser, document);
	}

	lxb_dom_node_t* root = lxb_dom_interface_node(document);

	// Extract and write all discovered URLs to a queue
	ExtractUrlsFromWebpage(root, urlQueue);

	std::string extractedText{ "" };
	try
	{
		SerializeTextContent(root, extractedText);
	}
	catch (const std::exception& e)
	{
		std::cout << "Error serializing text content: " << e.what() << std::endl;
	}

	// Clean up 
	CleanupLXB(htmlParser, document);

	return extractedText;
}

void WebPageProcessor::SerializeTextContent(lxb_dom_node_t* node, std::string& extractedText)
{
	// Using a node stack instead of recursion. Recursion caused a stack overflow on larger web pages
	std::stack<lxb_dom_node_t*> nodeStack;
	nodeStack.push(node);

	while (!nodeStack.empty())
	{
		lxb_dom_node_t* currentNode = nodeStack.top();
		nodeStack.pop();

		if (currentNode->type == LXB_DOM_NODE_TYPE_TEXT)
		{
			size_t len;
			lxb_char_t* text = lxb_dom_node_text_content(currentNode, &len);
			std::string filteredText{ "" };

			for (size_t i = 0; i < len; ++i)
			{
				char character = static_cast<char>(text[i]);
				if (isalnum(static_cast<unsigned char>(character)) || isspace(static_cast<unsigned char>(character)))
				{
					// Serialize lowercase for case agnostic querying
					filteredText.push_back(tolower(character));
				}
			}

			// Split the filteredText into words
			std::istringstream iss(filteredText);
			std::string word;

			extractedText.reserve(extractedText.size() + filteredText.size()); // Reserve space

			while (iss >> word)
			{
				// Lemmatize each word
				char* lemmatizedWord = _lemmatizer.Lemmatize(word.c_str());
				extractedText += lemmatizedWord;
				extractedText += " ";
				delete[] lemmatizedWord;
			}
		}
		else
		{
			// Small filter to avoid serializing a ton of values that don't relate to desired data
			if (currentNode->local_name != LXB_TAG_STYLE && currentNode->local_name != LXB_TAG_SCRIPT)
			{
				lxb_dom_node_t* child = lxb_dom_node_first_child(currentNode);
				while (child != NULL)
				{
					nodeStack.push(child);
					child = child->next;
				}
			}
		}
	}
}

void WebPageProcessor::ExtractUrlsFromWebpage(lxb_dom_node_t* node, std::queue<string>& urlQueue)
{
	if (node->local_name == LXB_TAG_A) // Check for anchor tags (links)
	{
		lxb_dom_element_t* element = lxb_dom_interface_element(node);
		const lxb_char_t* href = lxb_dom_element_get_attribute(element, (const lxb_char_t*)"href", 4, NULL);
		if (href)
		{
			std::string href_str{ (const char*)href, strlen((const char*)href) };
			std::regex urlPattern("^(https?|ftp)://[^\\s/$.?#].[^\\s]*$");

			// Remove fragment identifiers (e.g., #anchor)
			size_t fragment_pos = href_str.find('#');
			if (fragment_pos != std::string::npos) 
			{
				href_str.erase(fragment_pos);
			}

			// Check if the link starts with a valid scheme
			if (href_str.find("http://") == 0 || href_str.find("https://") == 0)
			{
				// Check if it's a Wikipedia page
				if (href_str.find("wikipedia.org") != std::string::npos)
				{
					// Check if it's an English Wikipedia page
					if (href_str.find("https://en.wikipedia.org/") == 0)
					{
						if (!href_str.empty() && std::regex_match(href_str, urlPattern))
						{
							//std::cout << "English Wikipedia page: " << href_str << std::endl;
							urlQueue.push(href_str);
						}
					}
				}
				else
				{
					if (!href_str.empty() && std::regex_match(href_str, urlPattern))
					{
						//std::cout << "Non-Wikipedia page: " << href_str << std::endl;
						urlQueue.push(href_str);
					}
				}
			}
		}
	}
	else
	{
		// Recursively search all child nodes
		lxb_dom_node_t* child = lxb_dom_node_first_child(node);
		while (child != NULL)
		{
			ExtractUrlsFromWebpage(child, urlQueue);
			child = child->next;
		}
	}
}

void WebPageProcessor::CleanupLXB(lxb_html_parser_t* htmlParser, lxb_html_document_t* document)
{
	if (htmlParser != NULL)
	{
		lxb_html_parser_destroy(htmlParser);
		htmlParser = nullptr;
	}
	if (document != NULL) 
	{
		lxb_html_document_destroy(document);
		document = nullptr;
	}
}
