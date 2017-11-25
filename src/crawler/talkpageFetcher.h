#ifndef TALKPAGEFETCHER_H
#define TALKPAGEFETCHER_H

#include <functional>
#include <stack>
#include <vector>

#include <QObject>

class TalkPageFetcher : public QObject
{
    Q_OBJECT

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

    TalkPageFetcher(std::vector<std::string> titles);

    void run();

protected:
    // static TalkPageTitle split_title(std::string title);
    const std::vector<std::string> _titles;
    std::stack<std::string> _next_titles;

    std::vector<TalkPageResult> request(std::vector<std::string> cur_titles);
    void generate_next_titles_to_get(const std::vector<TalkPageFetcher::TalkPageResult>& results);

signals:
	void start_new_article(std::string article_title, std::string full_page_title, std::string page_content);
	void finish_last_archive(std::string article_title);
    void write_status(std::string status_message);
};

#endif // TALKPAGEFETCHER_H
