all:
build:
	gcc ../Huffman\ Compression/*.c -o HuffmanCompression
	gcc ../LZW\ Compression/*.c -o LZWCompression
check: build
	./checker.sh > Results.txt										
quickCheck:
	./quickChecker.sh > Results.txt
clean:
	rm *.out
	rm *.huf
	rm *.lzw
	rm Results.txt
	rm HuffmanCompression
	rm LZWCompression
