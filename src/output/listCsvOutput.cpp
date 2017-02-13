#include "listCsvOutput.h"

#include <algorithm>
#include "dateOutput.h"

namespace WikiTalkNet {

	void listToCsv(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage)
	{
		for (auto paragraph : parsedTalkPage) {
			for(auto it = paragraph.second.begin(); it != paragraph.second.end(); ++it)
			{
				auto comment = *it;
				auto text = comment.Text;
				std::replace( text.begin(), text.end(), ';', '.');
				ostr
					<< comment.Id << ";"
					<< comment.ParentId << ";"
					<< comment.User << ";"
					<< to_iso_8601(comment.Date) << ";"
					<< text << std::endl;
			}
		}
	}
}
