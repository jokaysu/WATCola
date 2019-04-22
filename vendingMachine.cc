//vendingMachine.cc

#include "vendingMachine.h"
#include "MPRNG.h"

extern MPRNG mprng; //in main.cc

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, 
					unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
						prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost),
						maxStockPerFlavour(maxStockPerFlavour) {
	ivtr[0] = ivtr[1] = ivtr[2] = ivtr[3] = 0; // stock to 0
	nameServer.VMregister(this); // register
}

void VendingMachine::main() {
	prt.print(Printer::Kind::Vending, id, 'S', sodaCost);
	while (true) {
		try{
			_Enable{
				_Accept ( ~VendingMachine ) {
					break;
				} or _Accept ( inventory ) {
					_Accept ( restocked );
				} or _Accept ( buy );
			}
		} catch(...) {continue;}
	}
	prt.print(Printer::Kind::Vending, id, 'F');
}

void VendingMachine::buy( Flavours flavour, WATCard & card ) {
	//check
	if (card.getBalance() < sodaCost) _Throw Funds(); 
	if (ivtr[flavour] == 0) _Throw Stock();
	if (mprng(0, 4) == 0) _Throw Free(); // 1 in 5 free

	//deal
	ivtr[flavour]--;
	card.withdraw(sodaCost);
	prt.print(Printer::Kind::Vending, id, 'B', flavour, ivtr[flavour]);
}

unsigned int * VendingMachine::inventory() {
	prt.print(Printer::Kind::Vending, id , 'r');
	return ivtr;
}
    
void VendingMachine::restocked() {
	prt.print(Printer::Kind::Vending, id, 'R');
}

_Nomutex unsigned int VendingMachine::cost() {
	return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
	return id;
}