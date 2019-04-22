#include "parent.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

Parent::Parent(Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay):
    prt(prt), bank(bank), 
    numStudents(numStudents), parentalDelay(parentalDelay)
{}

void Parent::main() {
    prt.print(Printer::Parent, 'S');
    for (;;) {
		_Accept (~Parent) {
			break;
		} _Else {
		    yield(parentalDelay);
		    unsigned int id = mprng(0, numStudents-1);
		    unsigned int amount = mprng(1, 3);
		    bank.deposit(id, amount);
		    prt.print(Printer::Parent, 'D', id, amount);
		}
    }// for ;;
}

Parent::~Parent() {
    prt.print(Printer::Parent, 'F');
}

//jokay:
//The original parent will keep giving money to UW. #LOL