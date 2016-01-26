#!/usr/bin/python

"""
Expand Error Log For Specified Test Code
"""
import random, sys, re
from optparse import OptionParser, OptionGroup


class Error:
    def __init__(self, file, msg, line):
        self.file = file;
        self.msg = msg;
        self.line = int(line);

class Logger:

    def __init__(self, log_file, l_offset=15, r_offset=5):
        f = open(log_file, 'r')
        self.log = f.read().splitlines()
        f.close()
        self.errors = [];
        self.files = [];
        self.cache = {};
        self.L_OFFSET = l_offset;
        self.R_OFFSET = r_offset;

    def cache_file(self):
        files_to_cache = set([x.file for x in self.errors])
        for filename in files_to_cache:
            f = open(filename, 'r')
            self.cache[filename] = f.read().splitlines();
            f.close()

    def get_range(self, f, line_num):

        r_bound = min(len(f), line_num + self.R_OFFSET + 1);
        l_bound = max(0, line_num - self.L_OFFSET)
        return range(l_bound, r_bound)

    def print_error(self):
        LINE_MARK = "--> | "
        for (i, error) in enumerate(self.errors):
            print
            print '='*80
            print "#%d Failed assertion at line %s, file %s: \n\n  \"%s\"" \
                % (i + 1, error.line - 1, error.file, error.msg)
            print '='*80

            f = self.cache[error.file]
            lines = []
            for line in self.get_range(f, error.line):
                if (line == error.line - 1):
                    lines.append('{: <3}-->'.format(line) + f[line])
                else:
                    lines.append('{: <6}'.format(line) + f[line])

            print
            for line in lines:
                print line


    def parse_log(self):
        regex = re.compile('^Assertion failed: (.+?), .+?file (.+?), line (\d+).$');
        for line in self.log:
            match = regex.search(line)
            if match:
                err = Error(match.group(2), match.group(1), match.group(3));
                self.errors.append(err);

    def run(self):
        self.parse_log()
        self.cache_file()
        self.print_error()


def main():
    version_msg = "%prog 1.0"
    usage_msg = """%prog [OPTION]... LOG_FILE [-n number_of_lines_offset]

Expand error log for specified test code."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)

    parser.add_option("-n", action="store", dest="l_offset",type="int",
                        default=15, help="Set test code region")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        log_file = args[0]
    except:
        parser.error("wrong number of files")
    try:
        Logger(log_file, l_offset=options.l_offset).run();

    except IOError as err:
        errno, strerror = err.args;
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

if __name__ == "__main__":
    main()
