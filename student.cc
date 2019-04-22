//student.cc

#include "student.h"
#include "MPRNG.h"

extern MPRNG mprng; //in main.cc

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
             unsigned int id, unsigned int maxPurchases ) : 
			prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff),
			id(id), maxPurchases(maxPurchases) {
	giftUsed = false;
}

void Student::main() {
	//init
	numBottles = mprng(1, maxPurchases);
	int randomNum = mprng(0, 3);
	if (randomNum == 0) {
		favFlavor = VendingMachine::Flavours::blues;
	} else if (randomNum == 1) {
		favFlavor = VendingMachine::Flavours::classic;
	} else if (randomNum == 2) {
		favFlavor = VendingMachine::Flavours::rock;
	} else if (randomNum == 3) {
		favFlavor = VendingMachine::Flavours::jazz;
	}
	prt.print(Printer::Kind::Student, id, 'S', favFlavor, numBottles);

	watcard = cardOffice.create(id, 5);
	giftcard = groupoff.giftCard();
	vm = nameServer.getMachine(id);
	prt.print(Printer::Kind::Student, id, 'V', vm->getId());

	//attempt to buy
	while (numBottles > 0) {
		yield(mprng(1, 10));

		while (true) {
			_Select ( watcard ) {
				try {
					vm->buy(favFlavor, *(watcard()));
					prt.print(Printer::Kind::Student, id, 'B', favFlavor, watcard()->getBalance());
					break;
				} catch (WATCardOffice::Lost) {
					prt.print(Printer::Kind::Student, id, 'L');
					watcard = cardOffice.create(id, 5);
					continue; // not yielding
				} catch (VendingMachine::Free) {
					prt.print(Printer::Kind::Student, id, 'A', favFlavor, watcard()->getBalance());
					yield(4); // watch ads
					break;
				} catch (VendingMachine::Funds) {
					watcard = cardOffice.transfer(id, vm->cost() + 5, watcard());
					continue;
				} catch (VendingMachine::Stock) {
					vm = nameServer.getMachine(id);
					prt.print(Printer::Kind::Student, id, 'V', vm->getId());
					continue;
				}
			} or _When (!giftUsed) _Select ( giftcard ) {
				try {
					vm->buy(favFlavor, *(giftcard()));
					prt.print(Printer::Kind::Student, id, 'G', favFlavor, giftcard()->getBalance());
					giftUsed = true;
					break;
				} catch (VendingMachine::Free) {
					prt.print(Printer::Kind::Student, id, 'a', favFlavor, giftcard()->getBalance());
					yield(4);
					break;
				} catch (VendingMachine::Funds) {
					giftUsed = true;
					continue;
				} catch (VendingMachine::Stock) {
					vm = nameServer.getMachine(id);
					prt.print(Printer::Kind::Student, id, 'V', vm->getId());
					continue;
				}
			}//_Selete
		}//while true

		numBottles--;
	}// while numBottle

	//finish
	delete giftcard();
	try {
		delete watcard();
	} catch (WATCardOffice::Lost) {}
	prt.print(Printer::Kind::Student, id, 'F');
}