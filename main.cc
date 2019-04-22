//main.cc

#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
#include "student.h"
#include "config.h"
#include "MPRNG.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

MPRNG mprng; //globally accessed

int main (int argc, char *argv[]) {
	//init
	struct ConfigParms cp;
	const char *configFile = "soda.config";
	int seed = getpid();
	try {
		if (argc >= 2) {
			processConfigFile(argv[1], cp);
		} else {
			processConfigFile(configFile, cp);
		}//endif
		if (argc == 3) {
			seed = atoi(argv[2]);
			if (seed < 0) {
				cout << "Seed must be positive" << endl;
				return -2;
			}
		}//endif
		if (argc > 3) {
			cout << "Usage: soda [ config-file [ Seed ] ]" << endl;
			return -1;
		}//endif
	} catch (...) {
		cout << "Usage: soda [ config-file [ Seed ] ]" << endl;
		return -1;
	}

	//set seed
	mprng.set_seed(seed);

	//create
	Printer prt(cp.numStudents, cp.numVendingMachines, cp.numCouriers);
	Bank *bank = new Bank(cp.numStudents);
	Parent *parent = new Parent(prt, *bank, cp.numStudents, cp.parentalDelay);
	WATCardOffice office(prt, *bank, cp.numCouriers);
	Groupoff groupoff(prt, cp.numStudents, cp.sodaCost, cp.groupoffDelay);
	NameServer server(prt, cp.numVendingMachines, cp.numStudents);

	//vm
	VendingMachine *vm[cp.numVendingMachines];
	for (unsigned int i = 0; i < cp.numVendingMachines; ++i) {
		vm[i] = new VendingMachine(prt, server, i, cp.sodaCost, cp.maxStockPerFlavour);
	}

	//bp
	BottlingPlant *plant = new BottlingPlant(prt, server, cp.numVendingMachines,
							cp.maxShippedPerFlavour, cp.maxStockPerFlavour, cp.timeBetweenShipments);

	//stu
	Student *stu[cp.numStudents];
	for (unsigned int i = 0; i < cp.numStudents; ++i) {
		stu[i] = new Student(prt, server, office, groupoff, i, cp.maxPurchases);
	}

	//try delete stu first
	for (unsigned int i = 0; i < cp.numStudents; ++i) {
		delete stu[i];
	}

	//plant then vm
	delete plant;
	for (unsigned int i = 0; i < cp.numVendingMachines; ++i) {
		delete vm[i];
	}

	//delete others as reverse order
	delete parent;
	delete bank;
}