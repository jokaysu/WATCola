#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include "printer.h"
#include "nameServer.h"
#include "watcard.h"

_Task NameServer;

_Task VendingMachine {
  private:
    Printer & prt;
    NameServer & nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;
    unsigned int ivtr[4];
  protected:
    void main();
  public:
    enum Flavours { blues = 0, classic = 1, rock = 2, 
        jazz = 3 };                    // flavours of soda *** Music fans can only see genres.
    _Event Free {};                           // free, advertisement
    _Event Funds {};                          // insufficient funds
    _Event Stock {};                          // out of stock for particular flavour
    VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard & card );
    unsigned int * inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};


#endif //__VENDINGMACHINE_H__