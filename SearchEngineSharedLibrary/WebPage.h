#pragma once
#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class WebPage
{
public:
	WebPage();
	WebPage(const std::string& url, const std::string& webPageContent = "", const std::string& webPageTitle = "");
	const std::string& GetWebPageUrl() const;
	const std::string& GetWebPageContent() const;
	const std::string& GetWebPageTitle() const;
	void SetWebPageContent(const std::string webPageContent);
	void SetWebPageTitle(const std::string webPageTitle);

private:
	std::string _url;
	std::string _webPageContent;
	std::string _webPageTitle;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& _url;
		ar& _webPageContent;
		ar& _webPageTitle;
	}
};
