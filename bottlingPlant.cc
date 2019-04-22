//bottlingPlant.cc

#include "bottlingPlant.h"
#include "MPRNG.h"
#include "truck.h"

extern MPRNG mprng; //in main.cc

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ) :
				prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines), 
				maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
				timeBetweenShipments(timeBetweenShipments) {
	flag = false;
	product[0] = product[1] = product[2] = product[3] = 0;
}

void BottlingPlant::main() {
	prt.print(Printer::Kind::BottlingPlant, 'S');
	Truck trk(prt, nameServer, (*this), numVendingMachines, maxStockPerFlavour);
	while (true) {
		produce();

		yield(timeBetweenShipments);

		try {
			_Enable {
				_Accept ( ~BottlingPlant ) {
					flag = true;
					_Accept ( getShipment ); // make sure truck dies
					break;
				} or _Accept ( getShipment );
			}//_Enable
		} catch (...) {break;}
	}//while
	prt.print(Printer::Kind::BottlingPlant, 'F');
}

void BottlingPlant::produce() {
	unsigned int b = 0;
	for (int i = 0; i < 4; ++i) {
		product[i] = mprng(0, maxShippedPerFlavour);
		b += product[i];
	}
	prt.print(Printer::Kind::BottlingPlant, 'G', b);
}

void BottlingPlant::getShipment ( unsigned int cargo [] ) {
	if (flag) {
		_Throw Shutdown();
	} else {
		prt.print(Printer::Kind::BottlingPlant, 'P');
		for (int i = 0; i < 4; ++i) 
			cargo[i] = product[i];
	}
}