CXX = u++
CXXFLAGS = -std=c++11 -Wall -MMD -multi -O2 -g -Wextra
EXEC = soda
OBJECTS = main.o bottlingPlant.o nameServer.o printer.o student.o truck.o vendingMachine.o bank.o groupoff.o parent.o watcard.o watcardoffice.o config.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
