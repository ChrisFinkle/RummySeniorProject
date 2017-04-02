#ifndef HAND_H
#define HAND_H
#include <set>
#include <map>
using namespace std;
class Hand
{
	private:
		bool lockedIn;
		int size;
		double E;
		set<int> cardSet;
		Hand **allHands;
		map<set<int>, int> *idxs;
		map<int, map<int, Hand*>> futureHands;

		bool (*isRummy)(set<int>);
		
	public:
		Hand(set<int> cards, int _size, Hand **_allHands, map<set<int>, int> *_idxs, bool (*_isRummy)(set<int>));
		~Hand();
		bool getLockedIn();
		bool hasSameCards(set<int> compare);
		double evalE();
		double getE();
		void lockIn();
		void assocHands();
		string prettyPrintHand();
		bool getIsRummy();
		void reset();
};

#endif
