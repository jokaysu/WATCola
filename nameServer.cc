//nameServer.cc

#include "nameServer.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) : 
	prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
	registerOrder = 0;
	machineList = new VendingMachine*[numVendingMachines];
	studentOffset = new int[numStudents];
	for (unsigned int i = 0; i < numStudents; ++i) 
		studentOffset[i] = 0;
	registerOrder = 0;
}

NameServer::~NameServer() {
	delete [] machineList;
	delete [] studentOffset;
}

void NameServer::main() {
	prt.print(Printer::Kind::NameServer, 'S');
	while (registerOrder < numVendingMachines) 
		_Accept ( VMregister );
	while (true) {
		_Accept ( ~NameServer ) {
			break;
		} or _Accept ( getMachine ) {
		} or _Accept ( getMachineList );
	}//while true
	prt.print(Printer::Kind::NameServer, 'F');
}

void NameServer::VMregister( VendingMachine * vendingmachine) {
	machineList[registerOrder] = vendingmachine;
	registerOrder++;
	prt.print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
	unsigned int vmid;
	vmid = (id + studentOffset[id]) % numVendingMachines;
	studentOffset[id]++;
	prt.print(Printer::Kind::NameServer, 'N', id, machineList[vmid]->getId());
	return machineList[vmid];
}

VendingMachine ** NameServer::getMachineList() {
	return machineList;
}
