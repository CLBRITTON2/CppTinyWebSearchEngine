#pragma once
#include <string>

class WebPage
{
private: 
	std::string _url;
	std::string _webPageContent;
	const int _webPageID;

public:
	WebPage(const std::string url, const std::string webPageContent, int webPageID);
	const std::string GetWebPageUrl();
	const std::string GetWebPageContent() const;
	void SetWebPageContent(const std::string webPageContent);
	const int GetWebPageID() const;
};