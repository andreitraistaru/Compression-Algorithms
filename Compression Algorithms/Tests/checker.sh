#!/bin/bash

for i in $(seq 0 29); do
  echo "                    Test$i"
  echo "---------------------------------------------"
  echo "          Huffman Compression"
  echo
  echo "     Compression"
  echo
  echo Input size: $(ls -lh test$i.in | cut -d " " -f5)
  echo
  valgrind --log-fd=1 ./HuffmanCompression 1 test$i.in test$i.huf | grep 'Execution\|usage'
  echo
  echo Compressed size: $(ls -lh test$i.huf | cut -d " " -f5)
  echo
  echo "     Decompression"
  echo
  valgrind --log-fd=1 ./HuffmanCompression 2 test$i.huf test$i.out | grep 'Execution\|usage'
  echo
  diff test$i.in test$i.out -s
  echo
  echo "          LZW Compression"
  echo
  echo "     Compression"
  echo
  echo Input size: $(ls -lh test$i.in | cut -d " " -f5)
  echo
  valgrind --log-fd=1 ./LZWCompression 1 test$i.in test$i.lzw | grep 'Execution\|usage'
  echo
  echo Output size: $(ls -lh test$i.lzw | cut -d " " -f5)
  echo
  echo "     Decompression"
  echo
  valgrind --log-fd=1 ./LZWCompression 2 test$i.lzw test$i.out | grep 'Execution\|usage'
  echo
  diff test$i.in test$i.out -s
  echo
  echo
done
