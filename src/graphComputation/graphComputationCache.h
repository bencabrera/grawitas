#include "../models/graph.h"
#include "../models/parsedTalkPage.h"

namespace WikiTalkNet {

	class GraphComputationCache {
		public: 
			GraphComputationCache(const ParsedTalkPage& parsedTalkPage);
			~GraphComputationCache();

			UserGraph& GetUserGraph();
			CommentGraph& GetCommentGraph();
			TwoModeGraph& GetTwoModeGraph();

		private:
			const ParsedTalkPage& _parsedTalkPage;

			bool _hasUserGraph;
			bool _hasCommentGraph;
			bool _hasTwoModeGraph;
			
			void computeUserGraph();
			void computeCommentGraph();
			void computeTwoModeGraph();

			UserGraph* _userGraph;
			CommentGraph* _commentGraph;
			TwoModeGraph* _twoModeGraph;
	};

}
