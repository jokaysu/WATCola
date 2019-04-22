#pragma once
#include "watcard.h"
#include <queue>

class Printer;
class Bank;

_Task WATCardOffice {
    struct Job {                              // marshalled arguments and return future
        struct Args {
            unsigned int sid;
            unsigned int amount;
            WATCard *card;
            Args(unsigned int sid, unsigned int amount, WATCard *card=nullptr):
            sid(sid), amount(amount), card(card) {}
        };
        Args args;                            // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;             // return future
        Job( Args args ) : args( args ) {}
    };
    _Task Courier {
        WATCardOffice &office;
        Bank &bank;
        //jokay
        Printer &prt;
        //end jokay
	unsigned int id;
        void main();
        public:
        Courier(WATCardOffice &office, Bank &bank, Printer &prt, unsigned int id);
        ~Courier();
    };                    // communicates with bank

    void main();
    Printer &prt;
    uCondition cond;
    unsigned int numCouriers;
    Courier **couriers;
    std::queue<Job*> jobs;
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
    Job * requestWork();
};

