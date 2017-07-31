object= main.o head.o dec.o

serve:$(object)
	cc -o $(object)
main.o: head.h dec.h
	cc -c main.c 
head.o:head.h head.c
	cc -c head.c
dec.o: dec.h dec.c
	cc -c dec.c 

.PHONY:clean
clean:
	rm -rf $(object)