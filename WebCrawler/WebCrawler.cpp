#include "WebCrawler.h"

void WebCrawler::SaveRepositoryToBinaryFile(const std::string& fileName)
{
	_webPageRepository.SaveToBinaryFile(fileName + ".repository");
	_index.SaveToBinaryFile(fileName + ".index");
}

void WebCrawler::LoadRepositoryFromBinaryFile(const std::string& fileName)
{
	_webPageRepository.LoadFromBinaryFile(fileName + ".repository");
	_index.LoadFromBinaryFile(fileName + ".index");
}

void WebCrawler::IndexWebPage(WebPage& webPage)
{
	// Check to see if we've already indexed this page - no need for duplicates
	if (_webPageRepository.IsWebPagedIndexed(webPage.GetWebPageID()))
	{
		// std::cout << "Web Page ID: " << webPage.GetWebPageID() << ", has already been indexed - moving on..." << std::endl;
		return;
	}

	// Page hasn't been indexed - process it
	_webPageProcessor.SetLemmatizer(&_lemmatizer);
	_webPageProcessor.ProcessWebPage(webPage);
	_index.TokenizeWebPageContent(webPage);
	_webPageRepository.AddWebPage(webPage);
}
