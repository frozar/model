import argparse
import sys

import clang.cindex

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

    cursor_kind = str(cursor.kind).split(".")[1]
    cursor_type = str(cursor.type.kind).split(".")[1]
    cursor_str = None
    if (cursor_kind == "DECL_REF_EXPR"):
        cursor_str = cursor_kind + ", " + cursor_type
    else:
        cursor_str = cursor_kind
 
    # "| " * ((nb_indent - 1) if ((nb_indent - 1) >= 0) else 0) + \
    # print "has_more: %d | depth_has_no_more: %d" % (has_more, depth_has_no_more)
    # token_str = ["[" + t.spelling + ":" + str(t.cursor.kind).split(".")[1] + ":" + \
    #              ("T" if (t.cursor == cursor) else "F") + "]" for t in cursor.get_tokens()]
    # token_str.append(t.spelling + "(" + str(t.cursor.kind).split(".")[1] + ")")

    # token_str = None
    # in_child = False
    # # if ((cursor_kind == "VAR_DECL") or (cursor_kind == "CALL_EXPR")):
    # #     token_str = []
    # #     for t in cursor.get_tokens():
    # #         if (not (t.cursor.kind == cursor.kind)):
    # #             if (not in_child):
    # #                 token_str.append("[")
    # #                 in_child = True
    # #             token_str.append(t.spelling)
    # #         else:
    # #             if (in_child):
    # #                 token_str.append("]")
    # #                 in_child = False
    # #             token_str.append(t.spelling)

    # #     # Tail code
    # #     if (in_child):
    # #         token_str.append("]")
    # #         in_child = False
    # # else:
    # #     token_str = [t.spelling for t in cursor.get_tokens()]

    # token_str = []
    # for t in cursor.get_tokens():
    #     if (not (t.cursor.kind == cursor.kind)):
    #         if (not in_child):
    #             token_str.append("$<")
    #             in_child = True
    #         # token_str.append(t.spelling + "(" + str(t.cursor.kind).split(".")[1] + ")")
    #         token_str.append(t.spelling)
    #     else:
    #         if (in_child):
    #             token_str.append(">$")
    #             in_child = False
    #         # token_str.append(t.spelling + "(" + str(t.cursor.kind).split(".")[1] + ")")
    #         token_str.append(t.spelling)

    # # Tail code
    # if (in_child):
    #     token_str.append("]")
    #     in_child = False

    l_child_offset = []
    for child in cursor.get_children():
        child_tokens = list(child.get_tokens())
        first_token, last_token = child_tokens[0], child_tokens[-1]
        first_offset, last_offset = first_token.location.offset, last_token.location.offset
        l_child_offset.append( (first_offset, last_offset) )

    it_child_offset = iter(l_child_offset)
    token_str = None
    in_child = False
    depth = 0
    token_str = []

    cur_child_offset = None
    try:
        cur_child_offset = next(it_child_offset)
    except StopIteration:
        cur_child_offset = None

    l_cursor_token = list(cursor.get_tokens())
    idx_token = 0
    while (idx_token < len(l_cursor_token)):
        tok = l_cursor_token[idx_token]

        if (cur_child_offset is None):
            token_str.append(tok.spelling)
            idx_token += 1

        else:
            offset_0, offset_1 = cur_child_offset
            cur_offset = tok.location.offset
            if (cur_offset < offset_0):
                token_str.append(tok.spelling)
                idx_token += 1
                continue
            
            if ((offset_0 <= cur_offset) and (cur_offset <= offset_1)):
                if (not in_child):
                    token_str.append("$<")
                    in_child = True
                token_str.append(tok.spelling)
                idx_token += 1

            if (offset_1 < cur_offset):
                if (in_child):
                    token_str.append(">$")
                    in_child = False

                try:
                    cur_child_offset = next(it_child_offset)
                except StopIteration:
                    cur_child_offset = None

    # Tail code
    if (in_child):
        token_str.append(">$")
        in_child = False


    nb_token = 10000
    if (nb_token * 2 < len(token_str)):
        token_str = token_str[ : nb_token] + [" ... "] + token_str[ -nb_token : ]
    
    ast_dump_str = opening + \
                   connector + \
                   cursor_str + " : " + \
                   " ".join(token_str) + \
                   "\n"

    # for i, child in enumerate(cursor.get_children()):
    #     child_tokens = list(child.get_tokens())
    #     first_token, last_token = child_tokens[0], child_tokens[-1]
    #     first_offset, last_offset = first_token.location.offset, last_token.location.offset
    #     print "child %d: %d -> %d (%s -> %s)" % (i, first_offset, last_offset, first_token.spelling, last_token.spelling)

    # " : " + " ".join(["[" + t.spelling + "]" for t in cursor.get_tokens()])
    # " : " + str((cursor.displayname if cursor.displayname else cursor.get_tokens().next().spelling))
    # ("|-" if ((nb_indent > 0) and has_more) else "`-") + \
    # (str(cursor.displayname) if cursor.displayname else str(cursor.spelling)) + " : " + \

    # children_str = "CHILDREN:\n"
    # nb_children = 0
    # for child in cursor.get_children():
    #     # help(child.semantic_parent)
    #     # if (child.semantic_parent):
    #     #     print "TTTTTTTTTTTTTTTTTTTTTTTT", child.semantic_parent == cursor
    #     # print "CHILD [" + "".join([t.spelling for t in child.get_tokens()]) + "]"
    #     nb_children += 1
    #     children_str += "  "
    #     # if (child.semantic_parent):
    #     #     children_str += "semantic: " + ("T" if (child.semantic_parent == cursor) else "F") + " | "
    #     if (child.lexical_parent):
    #         children_str += "LEXICAL: " + ("T" if (child.lexical_parent == cursor) else "F") + " -- "
    #     # help(child.translation_unit)
    #     children_str += "# " + " ".join([t.spelling for t in child.get_tokens()]) + " #"
    #     children_str += " K # " + str(child.kind).split(".")[1]
    #     children_str += "\n"
    # children_str = children_str[ : -1 ]
    # if ( 0 < nb_children):
    #     print children_str

    for child, has_more in lookahead(cursor.get_children()):
        if (depth_has_no_more != -1):
            depth_has_no_more = depth_has_no_more
        elif ((not has_more) and (depth_has_no_more == -1)):
            depth_has_no_more = nb_indent
        else:
            depth_has_no_more = -1

        sub_ast_dump_str = walk(child, nb_indent + 1, has_more, depth_has_no_more)

        ast_dump_str += sub_ast_dump_str

    return ast_dump_str

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

    ## Compute the Abstract Syntac Tree (AST) dump
    ast_dump_str = walk(tu.cursor, 0, has_more = True, depth_has_no_more = -1)

    ## Write output
    write_output(ast_dump_str, output_file_name)


if __name__ == '__main__':
    main()
