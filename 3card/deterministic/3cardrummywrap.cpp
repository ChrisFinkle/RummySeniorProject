#include "hand.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <map>
#include <fstream>
using namespace std;

bool isRummyAcesWrap(set<int> _cards){
	int i = 0;
	int c[3];
	for(set<int>::iterator it = _cards.begin(); it!=_cards.end(); ++it){
		c[i++]=*it;
	}
	if(c[0]%4==c[1]%4 && c[1]%4==c[2]%4){
		if((c[1]/4-c[0]/4==1 && c[2]/4-c[1]/4==1) || (c[0]/4==0 && c[1]/4==11) || (c[1]/4==1 && c[2]/4==12)){
			return true;
		}
	}
	if(c[0]/4==c[1]/4 && c[1]/4==c[2]/4){
		return true;
	}
	return false;
}

int main(){
	Hand **hands = new Hand*[22100];
	map<set<int>, int> idxs;
	int counter = 0;

	for(int i=0; i<50; i++){
		for(int j=i+1; j<51; j++){
			for(int k=j+1; k<52; k++){
				set<int> c;
				c.insert(i);
				c.insert(j);
				c.insert(k);
				idxs[c] = counter;
				hands[counter++] = new Hand(c, 3, hands, &idxs, isRummyAcesWrap);
			}
		}
	}

	for(int i=0; i<22100; i++){
		hands[i]->assocHands();
		if(i%1000==0){
			cout << "Associated " << i << " out of 22100 hands" << endl;
		}
	}

	ofstream outfull;
	outfull.open("3aceWrapResultsFull.txt");
	int locked = 0;
	for(int i=0; i<22100; i++){
		if(hands[i]->getLockedIn()){
			outfull << hands[i]->prettyPrintHand() << 0 << endl;
			locked++;
		}
	}
	ofstream outsum; 
	outsum.open("3aceWrapResults.txt");
	while(locked<22100){
		bool xf = false;
		string xhand = "";
		int oldlocked = locked;
		double minE = 50.0;
		for(int i=0; i<22100; i++){
			if(!hands[i]->getLockedIn()){
				if(hands[i]->evalE()<minE){
					minE = hands[i]->getE();
				}
			}
		}

		for(int i=0; i<22100; i++){
			if(hands[i]->getE()-minE < 0.0000001 && !hands[i]->getLockedIn()){
				hands[i]->lockIn();
				locked++;
				if(!xf){
					outfull << hands[i]->prettyPrintHand() << fixed << setprecision(9) << minE << endl;
					xhand = hands[i]->prettyPrintHand();
					xf = true;
				} else {
					outfull << hands[i]->prettyPrintHand() << fixed << setprecision(4) << minE << endl;
				}
			}
		}
		cout << locked - oldlocked << " hands like " << xhand << "with E = " << minE << endl;
		outsum << locked - oldlocked << " hands like " << xhand << "with E = " << fixed << setprecision(10) << minE << endl;
	}
	outsum.close();
	outfull.close();
}