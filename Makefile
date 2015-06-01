all: clean lob

lob:
	gcc lob.c -lgmp -o lob

clean:
	rm lob
	
# vim: set noexpandtab:
