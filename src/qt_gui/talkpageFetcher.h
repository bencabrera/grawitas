#ifndef TALKPAGEFETCHER_H
#define TALKPAGEFETCHER_H

#include <functional>
#include <stack>
#include <vector>

class TalkPageFetcher
{
public:
    struct TalkPageResult {
        bool missing;
        std::string title;
        std::string content;
    };

    struct TalkPageTitle {
        std::string title;
        bool is_archive;
        std::size_t i_archive;
    };

    static constexpr std::size_t N_PAGES_PER_REQUEST = 10;
    static constexpr std::size_t N_NEXT_ARCHIVES = 5;
    static const std::string WIKIPEDIA_API_URL;

    TalkPageFetcher(const std::vector<std::string> titles);

    std::vector<std::function<void(std::string, std::string, std::string) >> new_page_callbacks; 		// [normalized_title] [long_title] [content]
    std::vector<std::function<void(std::string) >> finished_last_archive_callbacks;	 	// [normalized_title] [title]
    std::vector<std::function<void(std::string) >> status_callbacks;	 					// [status message]

    void run();
    std::vector<TalkPageResult> request(std::vector<std::string> cur_titles);

    void generate_next_titles_to_get(const std::vector<TalkPageFetcher::TalkPageResult>& results);

private:

    static TalkPageTitle split_title(std::string title);
    const std::vector<std::string> _titles;
    std::stack<std::string> _next_titles;
};

#endif // TALKPAGEFETCHER_H
