#!/bin/bash

# Find all makefiles in subdirectories and run make command
find . -name "Makefile" -execdir make \;