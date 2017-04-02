#include "hand.h"
#include <set>
#include <string>
#include <iostream>
using namespace std;

Hand::Hand(set<int> cards, int _size, Hand **_allHands, map<set<int>,int> *_idxs, bool (*_isRummy)(set<int>)){
	size = _size;
	isRummy = _isRummy;
	allHands = _allHands;
	idxs = _idxs;
	cardSet = cards;
	lockedIn = (isRummy(cardSet)) ? true : false;
	E = (isRummy(cardSet)) ? 0.0 : 500.0;
}

bool Hand::hasSameCards(set<int> compare){
	return compare==cardSet;
}

void Hand::assocHands(){
	for(int i=0; i<52; i++){
		set<int> newSet = cardSet;
		newSet.insert(i);
		if(newSet.size()>cardSet.size()){
			map<int, Hand*> m;
			int j=0;
			for(set<int>::iterator it = cardSet.begin(); it!=cardSet.end(); ++it){
				newSet.erase(*it);
				m[j] = allHands[idxs->at(newSet)];
				j++;
				newSet.insert(*it);
			}
			futureHands[i] = m;
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
	for(map<int, map<int,Hand*>>::iterator it = futureHands.begin(); it!=futureHands.end(); ++it){
		map<int,Hand*> n = it->second;
		for(map<int,Hand*>::iterator it2 = n.begin(); it2!=n.end(); ++it2){
			if(it2->second->getLockedIn() && (m.count(it->first)==0 || m[it->first]>it2->second->getE())){
				m[it->first]=it2->second->getE();
			}
		}
	}
	for(map<int, double>::iterator it3 = m.begin(); it3!=m.end(); ++it3){
		sum += it3->second;
	}
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
		if(*it%4==0){s="C";}
		else if(*it%4==1){s="D";}
		else if(*it%4==2){s="H";}
		else if(*it%4==3){s="S";}
		string v = "X";
		if(*it/4==0){v="A";}
		else if(*it/4==1){v="2";}
		else if(*it/4==2){v="3";}
		else if(*it/4==3){v="4";}
		else if(*it/4==4){v="5";}
		else if(*it/4==5){v="6";}
		else if(*it/4==6){v="7";}
		else if(*it/4==7){v="8";}
		else if(*it/4==8){v="9";}
		else if(*it/4==9){v="T";}
		else if(*it/4==10){v="J";}
		else if(*it/4==11){v="Q";}
		else if(*it/4==12){v="K";}
		st = st + v;
		st = st + s;
		st = st + " ";
	}
	return st;
}

bool Hand::getIsRummy(){
	return isRummy(cardSet);
}

void Hand::reset(){
	lockedIn = false;
	E = (isRummy(cardSet)) ? 0.0 : 500.0;
}

Hand::~Hand(){

}