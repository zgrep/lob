all: lob

lob:
	cc lob.c -lgmp -o lob

clean:
	rm lob
