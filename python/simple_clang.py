import argparse
import sys

import clang.cindex
# import asciitree

def get_dest_contents(translation_unit, source_contents):
    '''
    Return: str
    Build a string 'dest_contents' throught the clang token.
    For a given token, simply print it's spelling.
    '''
    dest_contents = ""
    previous_end_offset = None
    previous_end_line = None
    cursors_kind = []
    cursors_usr = []
    cursors_displayname = []
    tokens_kind = []

    ## Walk over every tokens
    for t in translation_unit.get_tokens(extent=translation_unit.cursor.extent):
        # print("PASS 0")
        extent = t.extent
        # print ("t.cursor.lexical_parent:", t.cursor.lexical_parent)
        # print ("t.cursor.smeantic_parent:", t.cursor.lexical_parent)

        # print ("t.cursor.lexical_parent.spelling:", t.cursor.lexical_parent.spelling)
        # print ("t.cursor.semantic_parent.spelling:", t.cursor.semantic_parent.spelling)

        # print("(%d, %d) -> (%d, %d)" %
        #       (extent.start.line, extent.start.column, extent.end.line, extent.end.column))
    
        cur_start_offset = extent.start.offset

        if (previous_end_line and ((extent.end.line - previous_end_line) != 0)):
            dest_contents += "\nTOKENS KIND: %s" % " ".join(tokens_kind)
            dest_contents += "\nCURSORS KIND: %s" % " ".join(cursors_kind)
            # dest_contents += "\nCURSORS USR: %s" % " ".join(cursors_usr)
            dest_contents += "\nCURSORS DISPLAYNAME: %s" % " ".join(cursors_displayname)
            cursors_kind = []
            cursors_usr = []
            cursors_displayname = []
            tokens_kind = []

        ## Store the non token character from the input
        if (previous_end_offset and ((extent.start.offset - previous_end_offset) != 0)):
            dest_contents += "%s" % (source_contents[previous_end_offset : extent.start.offset])

        cursors_kind.append(str(t.cursor.kind).split(".")[1])
        # cursors_usr.append(str(t.cursor.usr()).split(".")[1])
        cursors_usr.append(str(t.cursor.get_usr()))
        cursors_displayname.append(str(t.cursor.displayname))
        tokens_kind.append(str(t.kind).split(".")[1])

        ## Store the spelling of the current token
        dest_contents += "%s" % (t.spelling)

        previous_end_offset = extent.end.offset
        previous_end_line = extent.end.line

    dest_contents += "\nTOKENS KIND: %s" % " ".join(tokens_kind)
    dest_contents += "\nCURSORS KIND: %s" % " ".join(cursors_kind)

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
    parser.add_argument('-v', '--verbose',
                        action='store_true',
                        help='Print more clang information.')

    return parser.parse_args(args)

def lookahead(iterable):
    """Pass through all values from the given iterable, augmented by the
    information if there are more values to come after the current one
    (True), or if it is the last value (False).
    """
    # Get an iterator and pull the first value.
    it = iter(iterable)
    last = next(it)
    # Run the iterator to exhaustion (starting from the second value).
    for val in it:
        # Report the *previous* value (more to come).
        yield last, True
        last = val
    # Report the last value.
    yield last, False

def walk(cursor, nb_indent, has_more = True, depth_has_no_more = -1):
    opening = None
    if (nb_indent == 0):
        opening = ""
    elif (has_more and (depth_has_no_more == -1)):
        opening = "| " * (nb_indent - 1)
    else:
        opening = "| " * depth_has_no_more + "  " * (nb_indent - depth_has_no_more - 1)
    
    connector = None
    if (nb_indent == 0):
        connector = ""
    elif (has_more):
        connector = "|-"
    else:
        connector = "`-"

    # "| " * ((nb_indent - 1) if ((nb_indent - 1) >= 0) else 0) + \
    # print "has_more: %d | depth_has_no_more: %d" % (has_more, depth_has_no_more)
    print \
        opening + \
        connector + \
        str(cursor.kind).split(".")[1] + " : " + \
        " ".join(["[" + t.spelling + ":" + str(t.cursor.kind).split(".")[1] + ":" + \
                  ("T" if (t.cursor == cursor) else "F") + "]" for t in cursor.get_tokens()])
    # " : " + " ".join(["[" + t.spelling + "]" for t in cursor.get_tokens()])
    # " : " + str((cursor.displayname if cursor.displayname else cursor.get_tokens().next().spelling))
    # ("|-" if ((nb_indent > 0) and has_more) else "`-") + \
    # (str(cursor.displayname) if cursor.displayname else str(cursor.spelling)) + " : " + \

    children_str = "CHILDREN:\n"
    nb_children = 0
    for child in cursor.get_children():
        # help(child.semantic_parent)
        # if (child.semantic_parent):
        #     print "TTTTTTTTTTTTTTTTTTTTTTTT", child.semantic_parent == cursor
        # print "CHILD [" + "".join([t.spelling for t in child.get_tokens()]) + "]"
        nb_children += 1
        children_str += "  "
        # if (child.semantic_parent):
        #     children_str += "semantic: " + ("T" if (child.semantic_parent == cursor) else "F") + " | "
        if (child.lexical_parent):
            children_str += "LEXICAL: " + ("T" if (child.lexical_parent == cursor) else "F") + " -- "
        # help(child.translation_unit)
        children_str += "# " + " ".join([t.spelling for t in child.get_tokens()]) + " #"
        children_str += " K # " + str(child.kind).split(".")[1]
        children_str += "\n"
    children_str = children_str[ : -1 ]
    if ( 0 < nb_children):
        print children_str

    for child, has_more in lookahead(cursor.get_children()):
        # local_idx_in_level += 1

        # depth_has_no_more = None
        if (depth_has_no_more != -1):
            depth_has_no_more = depth_has_no_more
        elif ((not has_more) and (depth_has_no_more == -1)):
            depth_has_no_more = nb_indent
        else:
            depth_has_no_more = -1
            
        walk(child, nb_indent + 1, has_more, depth_has_no_more)

def main():
    args = parse_arguments(sys.argv[1:])

    ## Retrieve input argument
    input_file_name = args.files[0]
    output_file_name = args.files[1]

    ## Read input file
    source_contents = read_input(input_file_name)

    ## Clang magic: building the Abstract Structure Tree (AST) as a translation unit
    idx = clang.cindex.Index.create()
    fake_input = "osef.cpp"
    tu = idx.parse(fake_input, args=['-std=c++11'],
                   unsaved_files=[(fake_input, source_contents)], options = 0)
    # try:
    #     tu = idx.parse(input_file_name, args=['-std=c++11'])
    # catch:
    

    ## Compute the output
    dest_contents = get_dest_contents(tu, source_contents)

    ## Write output
    write_output(dest_contents, output_file_name)

    # tu = index.parse(source)
    walk(tu.cursor, 0)
    print ""

    # print asciitree.draw_tree(tu.cursor,
    #                           lambda n: n.get_children(),
    #                           lambda n: "%s (%s)" % (n.spelling or n.displayname, str(n.kind).split(".")[1]))


if __name__ == '__main__':
    main()
