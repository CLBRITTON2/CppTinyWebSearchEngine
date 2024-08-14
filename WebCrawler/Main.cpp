#include <iostream>
#include <WebCrawler.h>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main()
{
	WebCrawler webCrawler;

	int numberOfPagesToScrape{ 0 };
	std::cout << "Note: Killing the program before scraping is complete will likely corrupt binary files" << std::endl;
	std::cout << "Enter number of web pages to scrape: ";

	while (true) 
	{
		try 
		{
			std::cin >> numberOfPagesToScrape;
			if (std::cin.fail()) 
			{
				throw std::invalid_argument("Invalid input");
			}
			if (numberOfPagesToScrape <= 0) 
			{
				std::cout << "Please enter a positive number: ";
			}
			else 
			{
				break;
			}
		}
		catch (const std::exception& e) 
		{
			std::cerr << "Error: " << e.what() << std::endl;
			std::cin.clear(); // Clear the error flag
			std::cin.ignore(10000, '\n'); // Ignore the rest of the input
			std::cout << "Please enter a valid number: ";
		}
	}

	webCrawler.SetNumberOfPagesToScrape(numberOfPagesToScrape);

	// Broken at the moment - url queue isn't populated if a page has indexed - easy fix for another time maybe
	try
	{
		// See if we already started building binaries that we can use to pick up where we left off
		std::cout << "Attempting to load binaries..." << std::endl;
		std::cout << std::endl;
		webCrawler.LoadRepositoryFromBinaryFile("WebPages");
	}
	catch (const boost::archive::archive_exception& e)
	{
		std::cerr << "Deserialization failed: " << e.what() << std::endl;
	}

	webCrawler.Crawl();

	return 0;
}
