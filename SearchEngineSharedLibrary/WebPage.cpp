#include "WebPage.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

WebPage::WebPage()
	:_url{""}, _webPageContent{""}, _webPageTitle{""}
{

}

WebPage::WebPage(const std::string& url, const std::string& webPageContent, const std::string& webPageTitle)
	:_url{url}, _webPageContent{webPageContent}, _webPageTitle{webPageTitle}
{
}

const std::string& WebPage::GetWebPageUrl() const
{
	return _url;
}

const std::string& WebPage::GetWebPageContent() const
{
	return _webPageContent;
}

const std::string& WebPage::GetWebPageTitle() const
{
	return _webPageTitle;
}

// Should be called after crawler reads a webpage to set the content
void WebPage::SetWebPageContent(const std::string webPageContent)
{
	_webPageContent = webPageContent;
}

void WebPage::SetWebPageTitle(const std::string webPageTitle)
{
	_webPageTitle = webPageTitle;
}
