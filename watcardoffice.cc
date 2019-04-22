#include "watcardoffice.h"
#include "bank.h"
#include "printer.h"
#include "watcard.h"
#include "MPRNG.h"
#include <queue>
extern MPRNG mprng;

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers):
    prt(prt), numCouriers(numCouriers), couriers(new Courier*[numCouriers])
{
    for (unsigned int i = 0; i<numCouriers; ++i){
        couriers[i]=new Courier(*this, bank, prt, i);
    }
}

WATCardOffice::~WATCardOffice() {
    //jokay: you can only see the first 'F' below
    //       Because couriers did not exit and print 'F'.
    while (!cond.empty()) cond.signalBlock();
    //jokay end
    for (unsigned int i = 0; i<numCouriers; ++i)
        delete couriers[i];
    delete[] couriers;
    //jokay
    prt.print(Printer::WATCardOffice, 'F');
    //jokay end
}

void WATCardOffice::main() {
    prt.print(Printer::WATCardOffice, 'S');

    for (;;) {
        _Accept(~WATCardOffice) {
            break;
        } or _Accept(create, transfer, requestWork) {}
    }
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    prt.print(Printer::WATCardOffice, 'C', sid, amount);
    Job *newjob = new Job(Job::Args(sid, amount));
    jobs.push(newjob);
    cond.signal();
    return newjob->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard * card) {
    prt.print(Printer::WATCardOffice, 'T', sid, amount);
    Job *newjob = new Job(Job::Args(sid, amount, card));
    jobs.push(newjob);
    cond.signal();
    return newjob->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    if (jobs.empty()) {
        cond.wait();
    }
    if (jobs.empty()) {
        return NULL;
    } 
    Job* job = jobs.front();
    jobs.pop();
    prt.print(Printer::WATCardOffice, 'W');
    return job;
}

WATCardOffice::Courier::Courier(WATCardOffice &office, Bank &bank, Printer &prt, unsigned int id):
    office(office), bank(bank), prt(prt), id(id)
{
    prt.print(Printer::Kind::Courier, id, 'S');
}

void WATCardOffice::Courier::main() {
    for (;;) {
        _Accept (~Courier) {
            break;
        }
        _Else {
            Job *job = office.requestWork();
            if (!job) break;
            if (job->args.card == nullptr) { // Create
                WATCard *card = new WATCard();
                prt.print(Printer::Kind::Courier, id, 't', job->args.sid, job->args.amount);
                bank.withdraw(job->args.sid, job->args.amount);
                card->deposit(job->args.amount);
                job->result.delivery(card);
                prt.print(Printer::Kind::Courier, id, 'T', job->args.sid, job->args.amount);
            } else {
                if (mprng(0,5)==0) {
                    prt.print(Printer::Kind::Courier, id, 'L', job->args.sid);
                    delete job->args.card;
                    job->result.exception(new Lost);
                } else {
                    prt.print(Printer::Kind::Courier, id, 't', job->args.sid, job->args.amount);
                    bank.withdraw(job->args.sid, job->args.amount);
                    job->args.card->deposit(job->args.amount);
                    job->result.delivery(job->args.card);
                    prt.print(Printer::Kind::Courier, id, 'T', job->args.sid, job->args.amount);
                }
            }
            delete job;
        }
    }
    prt.print(Printer::Kind::Courier, id, 'F');
}

WATCardOffice::Courier::~Courier() {}
