#include "getPagesFromWikipedia.h"

#include <sstream>
#include <array>
#include <curl/curl.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

std::string uri_encode(const std::string &str)
{
	std::stringstream ss;
	for(unsigned char c : str)
		if((48 <= c && c <= 57) || (65 <= c && c <= 90) || (97 <= c && c <= 122) || c == '-' || c=='_' || c == '.' || c== '~')	
			ss << c;
		else
			ss << '%' << std::hex << static_cast<unsigned int>(c);

	return ss.str();
}

size_t curl_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	std::string* p = static_cast<std::string*>(userdata);
	p->append(ptr,size*nmemb);	

	return size*nmemb;
};

std::vector<TalkPageResult> get_pages_from_wikipedia(std::vector<std::string> page_titles)
{
	namespace pt = boost::property_tree;

	// build up url to use for request
	static const std::string WIKIPEDIA_API_URL = "https://en.wikipedia.org/w/api.php";
	static const std::string FIXED_PARAMETERS = "?action=query&&prop=revisions&&rvprop=content&&format=json";
	static const std::string URL_START = WIKIPEDIA_API_URL + FIXED_PARAMETERS;

	std::stringstream parameters;
	bool first = true;
	for(auto title : page_titles)
	{
		if(first)
			first = false;
		else
			parameters << "|";

		parameters << uri_encode(title);
	}
	const std::string url = URL_START + "&&titles=" + parameters.str();

	// curl request
	std::string request_result;
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &request_result);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);

		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			throw std::runtime_error(std::string("Could not perform HTTP GET request:") + curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	request_result.erase(std::remove_if(request_result.begin(), request_result.end(), [](const char c) { 
		return c == '\r' || c == '\n' || c == '\0';
	}), request_result.end()); // remove carriage return, newlines and \0 char

	// Create a root
	pt::ptree root;

	// Load the json file in this ptree
	std::stringstream ss(request_result);
	pt::read_json(ss, root);

	//TODO: better error handling
	auto pages = root.get_child("query.pages");
	std::vector<TalkPageResult> results;
	for(auto page : pages) {
		TalkPageResult result;
		auto t = page.second;			
		result.missing = t.count("missing");
		result.title = t.get<std::string>("title");

		if(!result.missing)
			result.content = t.get_child("revisions").front().second.get<std::string>("*");

		results.emplace_back(result);
	}

	return results;
}