#pragma once

// fwd decls
class Bank;
class Printer;

_Task Parent {
    Printer &prt;
    Bank &bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
    void main();
  public:
    Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};

