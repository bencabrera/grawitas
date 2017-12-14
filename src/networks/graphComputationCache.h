#include "graph.h"
#include "../talkPageParser/models.h"

namespace Grawitas {

	class GraphComputationCache {
		public: 
			GraphComputationCache(const std::vector<Comment>& parsedTalkPage);
			~GraphComputationCache();

			UserGraph& GetUserGraph();
			CommentGraph& GetCommentGraph();
			TwoModeGraph& GetTwoModeGraph();

		private:
			const std::vector<Comment>& _parsedTalkPage;

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
