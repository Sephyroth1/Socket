CC = gcc
FLAGS = -g -Wall -std=c99 -lm -Werror
LINK = -lws2_32
I1 = server.c
O1 = server.o
I2 = client.c
O2 = cleint.o
SEXE = server.exe
CEXE = client.exe

main: $(I1) $(I2)
	$(CC) $(FLAGS) -c $(I1) -o $(O1) $(LINK) | $(CC) $(FLAGS) -c $(I2) -o $(O2) $(LINK)
	
exe:$(O1) $(O2)
	$(CC) $(O1) -o $(SEXE) $(LINK) | $(CC) $(O2) -o $(CEXE) $(LINK)
	
run: $(SEXE) $(CEXE)
	./$(SEXE) | ./$(CEXE)

clean:
	del -f $(O1) $(SEXE)
	del -f $(O2) $(CEXE)
	cls
