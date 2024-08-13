#pragma once
#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class WebPage
{
public:
	WebPage();
	WebPage(const std::string& url, const std::string& webPageContent = "");
	const std::string& GetWebPageUrl() const;
	const std::string& GetWebPageContent() const;
	void SetWebPageContent(const std::string webPageContent);

private:
	std::string _url;
	std::string _webPageContent;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& _url;
		ar& _webPageContent;
	}
};
