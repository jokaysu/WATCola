#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "printer.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "watcard.h"

_Task Student {
  private:
  	Printer & prt;
  	NameServer & nameServer;
  	WATCardOffice & cardOffice;
  	Groupoff & groupoff;
  	unsigned int id;
  	unsigned int maxPurchases;
  	int numBottles;
  	VendingMachine::Flavours favFlavor;
  	WATCard::FWATCard watcard;
  	WATCard::FWATCard giftcard;
  	VendingMachine * vm;

  	bool giftUsed;

  protected:
    void main();

  public:
    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
             unsigned int id, unsigned int maxPurchases );
};

#endif //__STUDENT_H__