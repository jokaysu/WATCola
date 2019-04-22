#include "bank.h"
#include <iostream>
using namespace std;

Bank::Bank(unsigned int numStudents):
    amounts(new unsigned int[numStudents]),
    conds(new uCondition[numStudents])
{
	for (unsigned int i = 0; i < numStudents; ++i)
		amounts[i] = 0;
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    amounts[id] += amount;
    conds[id].signal();
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
    while (amounts[id]<amount) {
        conds[id].wait();
    }
    amounts[id] -= amount;
}

Bank::~Bank() {
    delete [] amounts;
    delete [] conds;
}
