#!/bin/bash
set -e
clang testapp.c -o testapp
clang climon.c -o climon

echo 'Unbuffered...'
./testapp 11 | ./climon
echo 'Fully buffered...'
./testapp 12 | ./climon
echo 'Unuffered, as script...'
script -q -c "./testapp 11" | ./climon
echo 'Linebuffered, as script...'
script -q -c "./testapp 12" | ./climon
rm typescript
