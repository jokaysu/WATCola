//truck.cc

#include "truck.h"
#include "MPRNG.h"

extern MPRNG mprng; // in main.cc

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
		prt(prt), nameServer(nameServer), plant(plant),
		numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {
	vmList = nameServer.getMachineList();
	lastVM = numVendingMachines - 1;
}

void Truck::main() {
	prt.print(Printer::Kind::Truck, 'S');
	while (true) {
		yield(mprng(1, 10));
		unsigned int sum = 0;

		try {
			plant.getShipment( stock );
			for (int i = 0; i < 4; ++i) 
				sum += stock[i];
			prt.print(Printer::Kind::Truck, 'P', sum);
		} catch (BottlingPlant::Shutdown) {
			break;
		}// try

		lastVM = (lastVM + 1) % numVendingMachines;
		for (unsigned int i = 0; i < numVendingMachines; ++i) {//check a full circle

			//check empty truck?
			bool flag = true;
			for (int j = 0; j < 4; ++j) 
				if (stock[j] > 0) 
					flag = false;
			if (flag) {
				lastVM = (lastVM + i - 1) % numVendingMachines;
				break;
			}

			//to VM
			int vmNo = (lastVM + i) % numVendingMachines;
			prt.print(Printer::Kind::Truck, 'd', vmList[vmNo]->getId(), sum);
			unsigned int remain = 0;
			unsigned int *vmInv = vmList[vmNo]->inventory();
			for (int j = 0; j < 4; ++j) { // per flavour
				if (stock[j] > maxStockPerFlavour - vmInv[j]) {
					vmInv[j] = maxStockPerFlavour;
					stock[j] -= maxStockPerFlavour - vmInv[j];
					sum -= maxStockPerFlavour - vmInv[j];
				} else {
					sum -= stock[j];
					vmInv[j] += stock[j];
					stock[j] = 0;
				}
				remain += maxStockPerFlavour - vmInv[j];
			}
			if (remain > 0) 
				prt.print(Printer::Kind::Truck, 'U', vmList[vmNo]->getId(), remain);
			prt.print(Printer::Kind::Truck, 'D', vmList[vmNo]->getId(), sum);
			vmList[vmNo]->restocked();

		}//for
	}//while
	prt.print(Printer::Kind::Truck, 'F');
}