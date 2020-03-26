#! /bin/sh

g++ -c threadsetting.cpp && \
    g++ -c singleton.cpp && \
    g++ -o singleton threadsetting.o singleton.o
