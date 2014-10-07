#!/bin/bash
echo "a" > foo.txt
echo "bc" >> foo.txt
echo `id -u` >> foo.txt
chmod 750 foo.txt
/bin/cat foo.txt | wc
