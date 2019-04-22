#ifndef __TRUCK_H__
#define __TRUCK_H__

#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"

_Task Truck {
  private:
  	Printer & prt;
  	NameServer & nameServer;
  	BottlingPlant & plant;
  	unsigned int numVendingMachines;
  	unsigned int maxStockPerFlavour;
  	unsigned int stock[4];
  	VendingMachine ** vmList;
  	int lastVM;
  protected:
    void main();
  public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif //__TRUCK_H__