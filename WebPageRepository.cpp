#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "WebPageRepository.h"
#include <fstream>
#include <iostream>
#include <filesystem>

void WebPageRepository::AddWebPage(WebPage& webPage)
{
	int webPageID = webPage.GetWebPageID();
	_webPages[webPageID] = std::make_shared<WebPage>(webPage);
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

bool WebPageRepository::IsWebPagedIndexed(int webPageID)
{
	return _webPages.find(webPageID) != _webPages.end();
}

//Save the _webPages map to binary
void WebPageRepository::SaveToBinaryFile(const std::string& fileName)
{
	if (_webPages.empty())
	{
		std::cerr << "Web Pages map is empty, nothing to serialize" << std::endl;
		return;
	}

	std::ofstream outFile(fileName, std::ios::binary);
	if (!outFile.is_open())
	{
		std::cerr << "Failed to open file for writing" << std::endl;
	}

	try
	{
		boost::archive::binary_oarchive oa(outFile);
		oa << _webPages;
	}
	catch (const boost::archive::archive_exception& e)
	{
		std::cerr << "Serialization failed: " << e.what() << std::endl;
	}

	if (outFile.fail() || outFile.bad())
	{
		std::cerr << "Error writing to file" << std::endl;
	}

	if (outFile.good())
	{
		outFile.flush();
		outFile.close();
	}
	else
	{
		std::cerr << "Error writing to file" << std::endl;
	}

	std::cout << "Save to binary: repository - end" << std::endl;
}

// Populate the _webPages map from binary
void WebPageRepository::LoadFromBinaryFile(const std::string& fileName)
{
    if (!std::filesystem::exists(fileName)) 
    {
        std::cout << "File not found: " + fileName << std::endl;
        return;
    }

    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile.is_open())
    {
        throw std::runtime_error("Failed to open file for reading: " + fileName);
    }

    boost::archive::binary_iarchive ia(inFile);
    ia >> _webPages;
}
