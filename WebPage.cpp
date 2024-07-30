#include "WebPage.h"

WebPage::WebPage(const std::string url, const int webPageID, const std::string webPageContent = "")
	:_url{ url }, _webPageID{ webPageID }, _webPageContent{ webPageContent }
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
