#include "WebPage.h"

WebPage::WebPage(const std::string url, const std::string webPageContent = "", const int webPageID)
	:_url{ url }, _webPageContent{ webPageContent }, _webPageID {webPageID}
{
}

const std::string WebPage::GetWebPageUrl()
{
	return _url;
}

const std::string WebPage::GetWebPageContent() const
{
	return _webPageContent;
}

// Should be called after crawler reads a webpage to set the content
void WebPage::SetWebPageContent(const std::string webPageContent)
{
	_webPageContent = webPageContent;
}

const int WebPage::GetWebPageID() const
{
	return _webPageID;
}
