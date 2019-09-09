#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include "Graph.h"
using namespace std;



int main(int argc, char **argv)
{
    string file_name;

    for (int i = 1; i < argc; i++)
    {
        // ******add test to confirm if the file given as a parameter is found
        file_name = argv[i];
        Graph g(file_name);

        // print the name of the data file passed as a parameter to the main() function
        cout << "-------------------------" << endl;
        cout << "---    " << file_name << "    ---" << endl;
        cout << "-------------------------" << endl << endl;

        // print a visual representation of the adjacency lists of the graph
        cout << "Display Graph:" << endl;
        g.display_graph();
        cout << endl;

        // print the result of the one_stroke() function thus indicating whether or not the graph can be drawn in one stroke
        if (g.one_stroke()) {
            cout << "one_stroke() = True" << endl;
        }
        else {
            cout << "one_stroke() = False" << endl;
        }

        // print the value of the starting vertex for the one stroke function, as calculated by the start_vertex() function
        if (g.start_vertex() != (-1)) {
            cout << "start_vertex() = " << g.start_vertex() << endl << endl;
        }
        else {
            cout << "start_vertex() = n/a" << endl << endl;
        }

        // print the path taken to draw the graph in one stroke
        cout << "Path: " << endl;
        g.path();
        cout << endl;
    }

    return 0;
}