//printer.cc

#include "printer.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
		numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
	
	//first 6
	waitList[Parent] = new printInfo;
	waitList[Groupoff] = new printInfo;
	waitList[WATCardOffice] = new printInfo;
	waitList[NameServer] = new printInfo;
	waitList[Truck] = new printInfo;
	waitList[BottlingPlant] = new printInfo;

	waitList[Student] = new printInfo[numStudents];
	waitList[Vending] = new printInfo[numVendingMachines];
	waitList[Courier] = new printInfo[numCouriers];

	//first lines
	cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant\t";
	for (unsigned int i = 0; i < numStudents; ++i) 
		cout << "Stud" << i << "\t";
	for (unsigned int i = 0; i < numVendingMachines; ++i)
		cout << "Mach" << i << "\t";
	for (unsigned int i = 0; i < numCouriers; ++i) {
		cout << "Cour" << i;
		if (i<numCouriers-1) cout << "\t";
	}
	cout << endl;
	for (unsigned int i = 0; i < 5 + numStudents + numVendingMachines + numCouriers; ++i)
		cout << "*******\t";
	cout << "*******" << endl;
}

Printer::~Printer() {
	flush(); // remember
	cout << "***********************" << endl;
	delete waitList[Parent];
	delete waitList[Groupoff];
	delete waitList[WATCardOffice];
	delete waitList[NameServer];
	delete waitList[Truck];
	delete waitList[BottlingPlant];

	delete [] waitList[Student];
	delete [] waitList[Vending];
	delete [] waitList[Courier];
}

static unsigned int tab_count;

void Printer::flush() {
	tab_count = 0;
	print8char(waitList[Parent]);
	print8char(waitList[Groupoff]);
	print8char(waitList[WATCardOffice]);
	print8char(waitList[NameServer]);
	print8char(waitList[Truck]);
	print8char(waitList[BottlingPlant]);
	for (unsigned int i = 0; i < numStudents; ++i) 
		print8char(&(waitList[Student][i]));
	for (unsigned int i = 0; i < numVendingMachines; ++i)
		print8char(&(waitList[Vending][i]));
	for (unsigned int i = 0; i < numCouriers; ++i) 
		print8char(&(waitList[Courier][i]));
	cout << endl;
}

void Printer::print8char(struct printInfo *info) {
	if (info->count == -1) {
		++tab_count;
		return;  // Nothing to print
	}
	for (unsigned int i = 0; i < tab_count; ++i) {
		cout << "\t";
	}
	
	tab_count = 1;
	if (info->count == 0) {
		cout << info->ch;
	} else if (info->count == 1) {
		cout << info->ch << info->v1;
	} else if (info->count == 2) {
		cout << info->ch << info->v1 << "," << info->v2;
	}
	info->count = -1;
}

void Printer::print( Kind kind, char state ) {
	if (waitList[kind]->count != -1)
		flush();
	waitList[kind]->count = 0;
	waitList[kind]->ch = state;
}

void Printer::print( Kind kind, char state, int value1 ) {
	if (waitList[kind]->count != -1)
		flush();
	waitList[kind]->count = 1;
	waitList[kind]->ch = state;
	waitList[kind]->v1 = value1;
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
	if (waitList[kind]->count != -1)
		flush();
	waitList[kind]->count = 2;
	waitList[kind]->ch = state;
	waitList[kind]->v1 = value1;
	waitList[kind]->v2 = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
	if (waitList[kind][lid].count != -1)
		flush();
	waitList[kind][lid].count = 0;
	waitList[kind][lid].ch = state;
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
	if (waitList[kind][lid].count != -1)
		flush();
	waitList[kind][lid].count = 1;
	waitList[kind][lid].ch = state;
	waitList[kind][lid].v1 = value1;
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
	if (waitList[kind][lid].count != -1)
		flush();
	waitList[kind][lid].count = 2;
	waitList[kind][lid].ch = state;
	waitList[kind][lid].v1 = value1;
	waitList[kind][lid].v2 = value2;
}
