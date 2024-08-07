#include "WebPageRepository.h"
#include <stdexcept>

void WebPageRepository::AddWebPage(int webPageID, std::shared_ptr<WebPage> webPage)
{
	_webPages[webPageID] = webPage;
}

std::shared_ptr<WebPage> WebPageRepository::GetWebPageById(int webPageID)
{
    auto iterator = _webPages.find(webPageID);
    if (iterator != _webPages.end()) 
    {
        return iterator->second;
    }
    else
    {
        throw std::runtime_error("Web page not found");
    }
}
