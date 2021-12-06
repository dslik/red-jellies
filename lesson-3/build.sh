#!/bin/bash

gcc test_utils.cc spi_flash.cc main.cc -Wno-c++11-compat-deprecated-writable-strings -o main