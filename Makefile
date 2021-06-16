PROGS   = Maquinas
CFLAGST = -D_REENTRANT -Wall
LIBS    = pthread

all: $(PROGS)

Maquinas : phtrdsMsg.o machMain.o
	$(CC) $(CFLAGST) -o Maquinas machMain.o phtrdsMsg.o -l $(LIBS)

phtrdsMsg.o : phtrdsMsg.c phtrdsMsg.h pMaquinasConf.h
	$(CC) $(CFLAGST) -g -c phtrdsMsg.c

machMain.o : machMain.c pMaquinasConf.h
	$(CC) $(CFLAGST) -g -c machMain.c

clean:
	rm -f $(PROGS) *~ *.o
