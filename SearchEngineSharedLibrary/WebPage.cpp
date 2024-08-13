#include "WebPage.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

WebPage::WebPage()
	:_url{""}, _webPageContent{""}
{

}

WebPage::WebPage(const std::string& url, const std::string& webPageContent)
	:_url{ url }, _webPageContent{ webPageContent }
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

// Should be called after crawler reads a webpage to set the content
void WebPage::SetWebPageContent(const std::string webPageContent)
{
	_webPageContent = webPageContent;
}
