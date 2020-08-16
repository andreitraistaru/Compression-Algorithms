#!/bin/bash

for i in $(seq 0 29); do
  echo "                    Test$i"
  echo "---------------------------------------------"
  echo "          Huffman Compression"
  echo
  echo "     Compression"
  echo
  ./HuffmanCompression 1 test$i.in test$i.huf
  echo
  echo "     Decompression"
  echo
  ./HuffmanCompression 2 test$i.huf test$i.out
  echo
  diff test$i.in test$i.out -s
  echo
  echo "          LZW Compression"
  echo
  echo "     Compression"
  echo
  ./LZWCompression 1 test$i.in test$i.lzw
  echo
  echo "     Decompression"
  echo
  ./LZWCompression 2 test$i.lzw test$i.out
  echo
  diff test$i.in test$i.out -s
  echo
  echo
done
