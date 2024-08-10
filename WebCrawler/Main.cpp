#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <string>
#include <WebCrawler.h>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main()
{
    WebCrawler webCrawler;
/*
_____________________________________________________________________________________________________________________________________________________________
TEST LOGIC FOR ADDING WEB PAGES FROM A TXT FILE MANUALLY BEGIN - Comment this entire block out once binaries are created - simulates web crawling
_____________________________________________________________________________________________________________________________________________________________
*/

std::vector<std::string> webPagesFromConfig;
std::string line;
std::ifstream inFile("WebPages.txt");
if (!inFile.is_open())
{
    std::cerr << "Failed to open config file." << std::endl;
    return -1;
}

// Add each URL from config file to a collection
while (getline(inFile, line))
{
    webPagesFromConfig.push_back(line);
}

// Create an empty vector of threads for each web page
std::vector<std::thread> threads;
int webPageID = 1;
auto timeOne = high_resolution_clock::now();

// Basically a C# foreach loop going over each line in the txt file containing urls
for (auto& line : webPagesFromConfig)
{
    // Create a thread for each web page so we can index every page concurrently, adds the thread to threads vector
    // This is a combination of AI/Stack overflow 
    // Might have to look at refactoring this as the project scales because we'll run out of threads
    threads.emplace_back([&webCrawler, &line, webPageID]() {
        WebPage webPage(line, webPageID);
        webCrawler.IndexWebPage(webPage);
        });
    webPageID++;
}

for (auto& thread : threads) 
{
    thread.join();
}

auto timeTwo = high_resolution_clock::now();

// Getting number of milliseconds as a double. 
duration<double, std::milli> msToIndex = timeTwo - timeOne;
std::cout << "It took " << msToIndex.count() << " ms to get and index all web pages" << std::endl;
std::cout << std::endl;

/*
_____________________________________________________________________________________________________________________________________________________________
TEST LOGIC FOR ADDING WEB PAGES FROM A TXT FILE MANUALLY END
_____________________________________________________________________________________________________________________________________________________________
*/


webCrawler.SaveRepositoryToBinaryFile("WebPages");

}
