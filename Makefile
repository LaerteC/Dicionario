all: 

	gcc -c indexerImplem.c -o implement -lm 
	gcc indexer.c implement -o indexer -lm
	
run:
	.\indexer.exe --freq 30 101.txt
	.\indexer.exe --freq-word the 101.txt
	.\indexer.exe --search "laerte e gustavo" "101.txt" "arquivo.txt"

clean:
	rm -rf *.o
