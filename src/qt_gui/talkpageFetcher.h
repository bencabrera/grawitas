#ifndef TALKPAGEFETCHER_H
#define TALKPAGEFETCHER_H

#include <functional>
#include <stack>

class TalkPageFetcher
{
public:
    TalkPageFetcher(const std::vector<std::string> titles);

    std::vector<std::function<void(std::string, std::string)>> new_page_callback; 		// [title] [content]
    std::vector<std::function<void(std::string)>> finished_last_archive_callback;	 	// [title]

    void run();

    void generate_next_titles_to_get(std::vector<std::string> last_titles);

private:
    const std::vector<std::string> _titles;
    std::stack<std::string> _next_titles;
};

#endif // TALKPAGEFETCHER_H
