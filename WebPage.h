#pragma once
#include <string>

class WebPage
{
private: 
	std::string _url;
	std::string _webPageContent;
	int _id;

public:
	WebPage(const std::string url, const std::string webPageContent);
	const std::string GetWebPageUrl();
	const std::string GetWebPageContent();
	void SetWebPageContent(const std::string webPageContent);
	int GetWebPageID();
};