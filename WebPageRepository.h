#pragma once
#include <map>
#include <memory>
#include "WebPage.h"

class WebPageRepository
{
private:
	std::map<int, std::shared_ptr<WebPage>> _webPages;

public:
	void AddWebPage(int webPageID, std::shared_ptr<WebPage> webPage);
	std::shared_ptr<WebPage> GetWebPageById(int webPageID);
};