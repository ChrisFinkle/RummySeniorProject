#include "hand.h"
#include <set>
#include <string>
#include <iostream>
using namespace std;

/*  Rummy hand class with methods designed for successive calculation of 
	expected times to Rummy. This class is multi-purpose; it can hold any
	number of cards (though in practice only 3 and 4 are useful). It can also
	have any definition of what is Rummy as desired; the isRummy function is
	defined in another program that uses Hand and passed in via the constructor,
	along with a point to an array containing all other possible hands, a map
	that serves as a 'guide' to said array, allowing fast lookup, knowledge
	of what size the hand is, and its cards.
*/
Hand::Hand(set<int> cards, int _size, Hand **_allHands, map<set<int>,int> *_idxs, bool (*_isRummy)(set<int>)){
	size = _size;
	isRummy = _isRummy;
	allHands = _allHands;
	idxs = _idxs;
	cardSet = cards;
	lockedIn = isRummy(cardSet);
	E = (isRummy(cardSet)) ? 0.0 : 500.0; //500 is a stand-in for a high, unknown value for hands which are not rummy
}

bool Hand::hasSameCards(set<int> compare){
	return compare==cardSet;
}

void Hand::assocHands(){
	for(int i=0; i<52; i++){
		set<int> newSet = cardSet;
		newSet.insert(i); //draw a random card
		if(newSet.size()>cardSet.size()){ //if card is not in your hand
			map<int, Hand*> m; //create map of outcomes by index of card discarded
			int j=0;
			for(set<int>::iterator it = cardSet.begin(); it!=cardSet.end(); ++it){
				newSet.erase(*it); //discard card
				m[j] = allHands[idxs->at(newSet)]; //search all hands for new hand, store ptr
				j++;
				newSet.insert(*it); //put the card back
			}
			futureHands[i] = m; //store map of outcomes
		}
	}
}


bool Hand::getLockedIn(){
	return lockedIn;
}

void Hand::lockIn(){
	lockedIn=true;
}

double Hand::evalE(){
	map<int, double> m;
	double sum = 0.0;
	//iterate over potential hands to draw
	for(map<int, map<int,Hand*>>::iterator it = futureHands.begin(); it!=futureHands.end(); ++it){
		map<int,Hand*> n = it->second; //possible post-discard outcomes
		//among locked-in future hands, if hand not yet stored in m or if hand is lower than value
		//currently stored in m under the drawn card, stores E in m.
		for(map<int,Hand*>::iterator it2 = n.begin(); it2!=n.end(); ++it2){
			if(it2->second->getLockedIn() && (m.count(it->first)==0 || m[it->first]>it2->second->getE())){
				m[it->first]=it2->second->getE();
			}
		}
	}
	//adds up all Es in accordance with Bellman sum
	for(map<int, double>::iterator it3 = m.begin(); it3!=m.end(); ++it3){
		sum += it3->second;
	}
	//if any future hands locked in, calculate final value in accordance w/ Bellman sum
	if(m.size()>0){
		E = (sum+52.0-(float)size)/(float)m.size();
	}
	return E;
}

double Hand::getE(){
	return E;
}

string Hand::prettyPrintHand(){
	string st = "";
	for(set<int>::iterator it = cardSet.begin(); it!=cardSet.end(); ++it){
		string s = "X";
		if(*it%4==0){s="C";} //Clubs
		else if(*it%4==1){s="D";} //Diamonds
		else if(*it%4==2){s="H";} //Hearts
		else if(*it%4==3){s="S";} //Spades
		string v = "X";
		if(*it/4==0){v="A";} //Ace
		else if(*it/4==1){v="2";}
		else if(*it/4==2){v="3";}
		else if(*it/4==3){v="4";}
		else if(*it/4==4){v="5";}
		else if(*it/4==5){v="6";}
		else if(*it/4==6){v="7";}
		else if(*it/4==7){v="8";}
		else if(*it/4==8){v="9";}
		else if(*it/4==9){v="T";} //Ten
		else if(*it/4==10){v="J";} //Jack
		else if(*it/4==11){v="Q";} //Queen
		else if(*it/4==12){v="K";} //King
		st = st + v;
		st = st + s;
		st = st + " ";
	}
	return st;
}

bool Hand::getIsRummy(){
	return isRummy(cardSet); //function pointer
}

//resets E without resetting hand associations; used in batch approximations
void Hand::reset(){
	lockedIn = false;
	E = (isRummy(cardSet)) ? 0.0 : 500.0;
}

Hand::~Hand(){

}
