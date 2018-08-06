import argparse
import sys

def toto(tmp_str):
    tmp_str += "toto"

def main():
    tmp_str = ""
    toto(tmp_str)
    print "|" + tmp_str + "|"

if __name__ == '__main__':
    main()
