#include "WebPage.h"

WebPage::WebPage(const std::string url, const std::string webPageContent)
	:_url{ url }, _webPageContent{ webPageContent }
{
}

const std::string WebPage::GetWebPageUrl()
{
	return _url;
}

const std::string WebPage::GetWebPageContent()
{
	return _webPageContent;
}
