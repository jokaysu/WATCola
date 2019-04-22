#include "groupoff.h"
#include "printer.h"
#include <vector>
#include "MPRNG.h"
extern MPRNG mprng;
using std::vector;

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay):
    prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay)
{}

Groupoff::~Groupoff() {
    prt.print(Printer::Groupoff, 'F');
}

void Groupoff::main() {
    prt.print(Printer::Groupoff, 'S');
    for (unsigned int i=0; i<numStudents; ++i) {
	_Accept(~Groupoff, giftCard);
    }//for
    for (unsigned int i=0; i<numStudents; ++i) {
	_Accept(~Groupoff) {
		break;
	}//Accepte
	_Else {
	    yield(groupoffDelay);
	    unsigned int idx = mprng(0, cards.size()-1);
	    WATCard *newcard = new WATCard(); // student needs to delete it.
	    newcard->deposit(sodaCost);
	    prt.print(Printer::Groupoff, 'D', sodaCost);
	    cards[idx].delivery(newcard);
	    cards.erase(cards.begin()+idx);
	}//Else
    }//for
}//main

WATCard::FWATCard Groupoff::giftCard() {
	WATCard::FWATCard card;
	cards.push_back(card);
	return card;
}