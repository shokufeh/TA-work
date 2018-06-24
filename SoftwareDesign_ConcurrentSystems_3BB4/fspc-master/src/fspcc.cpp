/*
 *  fspc - A Finite State Process compiler and LTS analyzer
 *
 *  Copyright (C) 2013-2014  Vincenzo Maffione
 *  Email contact: <v.maffione@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Main FspDriver class, instantiating the parser. */
#include "fsp_driver.hpp"
#include "code_generator.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;


void help()
{
    cout << "fspc - A Finite State Process compiler and LTS analisys tool.\n";
    cout << "USAGE: fspc [-dpgasSh] [-i FILE | -l FILE] [-o FILE]\n";
    cout << "   -i FILE : Specifies FILE as the input file containing "
        "FSP definitions.\n";
    cout << "   -l FILE : Specifies FILE as the input file containing "
        "compiled LTSs.\n";
    cout << "   -o FILE : Specifies the output FILE, e.g. the file that "
        "will contain the compiled LTSs.\n";
    cout << "   -d : Runs deadlock/error analysis on every FSP.\n";
    cout << "   -p : Runs all the specified progress verifications on "
        "every FSP.\n";
    cout << "   -g : Outputs a graphviz representation file of every FSP.\n";
    cout << "   -a : The same as '-d -p -g'\n";
    cout << "   -s : Runs an LTS analysis interactive shell\n";
    cout << "   -S FILE : Runs an LTS analysis script\n";
    cout << "   -D NUM : The maximum depth of process references accepted "
        "within a process definition (default is 1000)\n";
    cout << "   -v : Shows versioning information\n";
    cout << "   -h : Shows this help.\n";
}

void help_with_suggestion()
{
        help();
        cout << "\n    [Hint] Try\n";
        cout << "       $ fspcc -s -i input.fsp\n";
        cout << "    to open 'input.fsp' and run the interactive shell\n";
}

void process_args(CompilerOptions& co, int argc, char **argv)
{
    int ch;
    int il_options = 0;

    /* Set default values. */
    co.input_file = NULL;
    co.input_type = CompilerOptions::InputTypeFsp;
    co.output_file = NULL;
    co.deadlock = false;
    co.progress = false;
    co.graphviz = false;
    co.shell = false;
    co.script = false;
    co.max_reference_depth = 1000;

    while ((ch = getopt(argc, argv, "i:l:o:adpghsvS:D:")) != -1) {
        switch (ch) {
            default:
                cout << "\n";
                help_with_suggestion();
                exit(1);
                break;

            case 'i':
                il_options++;
                co.input_file = optarg;
                co.input_type = CompilerOptions::InputTypeFsp;
                break;

            case 'l':
                il_options++;
                co.input_file = optarg;
                co.input_type = CompilerOptions::InputTypeLts;
                break;

            case 'o':
                co.output_file = optarg;
                break;

            case 'a':
                co.deadlock = co.progress = co.graphviz = true;
                break;

            case 'd':
                co.deadlock = true;
                break;

            case 'p':
                co.progress = true;
                break;

            case 'g':
                co.graphviz = true;
                break;

            case 'h':
                help();
                exit(0);
                break;

            case 's':
                co.shell = true;
                break;

            case 'S':
                co.script = true;
                co.script_file = optarg;
                break;

            case 'D':
                co.max_reference_depth = atoi(optarg);
                break;

            case 'v':
                cout << "fspc 1.8 (August 2014)\n";
                cout << "Copyright 2013-2014 Vincenzo Maffione\n";
                exit(0);
        }
    }

    if (il_options > 1) {
        cerr << "Error: Cannot specify more than one input file\n\n";
        help();
        exit(-1);
    }

    if (!co.input_file) {
        cerr << "Error: Missing input file\n\n";
        help_with_suggestion();
        exit(-1);
    }

    if (!co.output_file && !co.shell && !co.script) {
        /* There's (normally) no point in running this program if one
           neither runs a shell nor a script nor generates a compiled
           output. Assume a default '-o' option. */
        co.output_file = "out.lts";
    }
}


int main (int argc, char ** argv)
{
    FspDriver driver;
    CompilerOptions co;
    int ret;

    process_args(co, argc, argv);

    ret = driver.parse(co);

    return ret;
}
