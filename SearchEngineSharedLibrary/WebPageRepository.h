#pragma once
#include "WebPage.h"

class WebPageRepository
{
public:
	// Add a web page to the web page repoistory
	void AddWebPage(WebPage& webPage);

	std::shared_ptr<WebPage> GetWebPageById(int webPageID);

	// Load the web page repoistory from a binary file
	void LoadFromBinaryFile(const std::string& filename);

	// Save the web page repository to a binary file
	void SaveToBinaryFile(const std::string& filename);

	// Check to see whether or not a web page has been indexed
	bool IsWebPagedIndexed(const std::string& webPageUrl);

private:
	std::map<int, std::shared_ptr<WebPage>> _webPages;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& _webPages;
	}
};