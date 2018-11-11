#!/bin/bash
rm writer
rm reader
gcc -o writer writer.c
gcc -o reader reader.c
./writer
