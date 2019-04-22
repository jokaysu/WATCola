#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include "printer.h"
#include "vendingMachine.h"

_Task VendingMachine;

_Task NameServer {
  private:
  	Printer & prt;
  	unsigned int numVendingMachines;
  	unsigned int numStudents;
  	unsigned int registerOrder;
  	VendingMachine ** machineList;
  	int * studentOffset;
  protected:
    void main();
  public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id );
    VendingMachine ** getMachineList();
};

#endif //__NAMESERVER_H__