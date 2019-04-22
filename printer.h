#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <map>

using namespace std;

_Monitor Printer {
  public:
  	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
  private:
	struct printInfo {
		int count = -1;
		char ch;
		int v1;
		int v2;
	};
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;
	map<Kind, struct printInfo *> waitList;
	void flush();
	void print8char(struct printInfo *info);
  public:
  	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
  	~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};

#endif //__PRINTER_H__