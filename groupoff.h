#pragma once
#include "watcard.h"
#include <vector>

class Printer;
_Task Groupoff {
    Printer &prt;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;
    std::vector<WATCard::FWATCard> cards;
    void main();
  public:
    Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};
