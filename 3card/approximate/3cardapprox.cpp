#include "hand.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <set>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
using namespace std;

bool isRummyAcesLow(set<int> _cards){
	int i = 0;
	int c[3];
	for(set<int>::iterator it = _cards.begin(); it!=_cards.end(); ++it){
		c[i++]=*it;
	}
	if(c[0]%4==c[1]%4 && c[1]%4==c[2]%4){
		if(c[1]/4-c[0]/4==1 && c[2]/4-c[1]/4==1){
			return true;
		}
	}
	if(c[0]/4==c[1]/4 && c[1]/4==c[2]/4){
		return true;
	}
	return false;
}

int main ( int argc, char *argv[] )
{
	if ( argc < 2 ) // argc should be 2 for correct execution
		cout<<"usage: "<< argv[0] <<" number of buckets per unit (int)" << endl;
	else {
		ofstream times;
		times.open("times.txt");
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
					hands[counter++] = new Hand(c, 3, hands, &idxs, isRummyAcesLow);
				}
			}
		}

		for(int i=0; i<22100; i++){
			hands[i]->assocHands();
			if(i%1000==0){
				cout << "Associated " << i << " out of 22100 hands" << endl;
			}
		}

		for(int i=1; i<argc; i++){
			char* p;
			errno = 0;
			int arg = strtol(argv[i], &p, 10);
			if (*p != '\0' || errno != 0) {
				cout << "Discarding argument " << argv[i] << " (could not convert to int)" << endl;
			} else {
				time_t start;
				time(&start);
				int counter = 0;

				double tol = 1.0/arg;
				string sarg(argv[i]);
				string sumOutStr = "3lowApprox" + sarg + ".txt";
				string fullOutStr = "3lowApprox" + sarg + "full.txt";
				
				ofstream outfull;
				outfull.open(fullOutStr);
				ofstream outsum;
				outsum.open(sumOutStr);

				int locked = 0;
				for(int i=0; i<22100; i++){
					if(hands[i]->getLockedIn()){
						outfull << hands[i]->prettyPrintHand() << 0 << endl;
						locked++;
					}
				}

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
						if(hands[i]->getE()-minE < tol && !hands[i]->getLockedIn()){
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
					counter++;
				}
				outsum.close();
				outfull.close();
				time_t end;
				time(&end);
				int duration = difftime(start, end);
				times << "For tolerance " << tol << " (presumed mean accuracy " <<
				  tol*0.3 << ") " << counter << " groups calculated in " << duration
				  << " seconds" << endl;

				for(int i=0; i<22100; i++){
					if(hands[i]->getLockedIn()){
						hands[i]->reset();
					}
				}
			}
		}
		times.close();
	}
}