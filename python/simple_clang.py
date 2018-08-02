import argparse
import sys

import clang.cindex

def get_dest_contents(translation_unit, source_contents):
    '''
    Return: str
    Build a string 'dest_contents' throught the clang token.
    For a given token, simply print it's spelling.
    '''
    dest_contents = ""
    previous_end_offset = None
    ## Walk over every tokens
    for t in translation_unit.get_tokens(extent=translation_unit.cursor.extent):
        extent = t.extent
        # print("(%d, %d) -> (%d, %d)" %
        #       (extent.start.line, extent.start.column, extent.end.line, extent.end.column))
    
        cur_start_offset = extent.start.offset
        ## Store the non token character from the input
        if (previous_end_offset and ((extent.start.offset - previous_end_offset) != 0)):
            dest_contents += "%s" % (source_contents[previous_end_offset : extent.start.offset])

        ## Store the spelling of the current token
        dest_contents += "%s" % (t.spelling)
    
        previous_end_offset = extent.end.offset

    ## Store the end of file
    dest_contents += "%s" % (source_contents[previous_end_offset : ])

    return dest_contents
    

def read_input(input_file_name):
    '''
    Return: str
    Read input file or stdin if the input file == "", return a string of the input content.
    '''
    if (input_file_name == "-"):
        input_file = sys.stdin
        source_contents = input_file.read()
    else:
        input_file = in_file = open(input_file_name, "rb")
        source_contents = input_file.read()
        in_file.close()

    return source_contents

def write_output(dest_contents, output_file_name):
    '''
    Return: None
    Write output file or in stdout if the output file == "".
    '''
    if (output_file_name == "-"):
        output_file = sys.stdout
        output_file.write(dest_contents)
    else:
        output_file = open(sys.argv[2], "w")
        output_file.write(dest_contents)
        output_file.close()

def parse_arguments(args):
    '''
    Return: parser_object
    Parse input arguments.
    '''
    parser = argparse.ArgumentParser(description='C++-identity')
    parser.add_argument('files', nargs=2, help='<input_file> <output_file>')

    return parser.parse_args(args)

def main():
    args = parse_arguments(sys.argv[1:])

    ## Retrieve input argument
    input_file_name = args.files[0]
    output_file_name = args.files[1]

    ## Read input file
    source_contents = read_input(input_file_name)

    ## Clang magic: building the Abstract Structure Tree (AST) as a translation unit
    idx = clang.cindex.Index.create()
    tu = idx.parse(input_file_name, args=['-std=c++11'])

    ## Compute the output
    dest_contents = get_dest_contents(tu, source_contents)
    
    ## Write output
    write_output(dest_contents, output_file_name)


if __name__ == '__main__':
    main()
