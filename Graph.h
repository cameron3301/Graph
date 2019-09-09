#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <stdexcept>
using namespace std;


class Graph
{
    private:
        int num_vert;
        int num_edge;
        vector<list<int>> adj;

        void allocate_memory() {
            vector<list<int>> temp(num_edge);
            adj = temp;
        }

    public:
        Graph(string filename);

        void add_edge(int v, int w) {
            adj[v-1].push_back(w);
            adj[w-1].push_front(v);
        }

        void display_graph();
        int num_vertices() { return num_vert; }
        int num_edges() { return num_edge; }

        bool has_odd_edges(int u) {
            if ((adj[u].size() % 2) == 1)
                return true;
            return false;
        }

        void DFS_recur(int v, bool visited[]);
        bool is_connected();

        bool one_stroke();
        int start_vertex();
        void path();        
};



Graph::Graph(string filename)
{
    ifstream in(filename);

    in >> num_vert;
    in >> num_edge;

    allocate_memory();

    int a, b;

    for (int i = 0; i < num_edge; i++) {
        in >> a;
        in >> b;

        add_edge(a, b);        
    }
}



void Graph::DFS_recur(int v, bool visited[])
{
    // mark the current node as visited and print it
    visited[v] = true;

    // recur for all the vertices adjacent to this vertex
    list<int>::iterator lit;
    for (lit = adj[v].begin(); lit != adj[v].end(); lit++) {
        if (!visited[*lit])
            DFS_recur(*lit, visited);
    }
}



bool Graph::is_connected()
{
    // mark all vertices as not visited
    bool visited[num_vert];
    
    // ensure that there are no vertices of degree 0
    for (int i = 0; i < num_vert; i++) {
        if (adj[i].size() == 0)
            return false;
    }

    // start DFS traversal from the first vertex
    DFS_recur(0, visited);

    // Check if all non-zero degree vertices are visited
    for (int i = 0; i < num_vert; i++) {
        if ((visited[i] == false) && (adj[i].size() > 0))
            return false;
    }

    return true;
}



bool Graph::one_stroke()
{
    // check if all non-zero degree vertices are connected
    if (is_connected() == false)
        return 0;

    // count vertices with odd degree
    int odd = 0;
    for (int i = 0; i < num_vert; i++) {
        if (adj[i].size() & 1)
            odd++;
    }

    // if count is more than 2, then the graph is not eulerian
    if (odd > 2)
        return false;

    return true;
}



// returns the VALUE of the start vertex of the one_stroke path
int Graph::start_vertex()
{
    // if the graph cannot be drawn with one stroke, return -1 -> represents an invalid output
    if (!one_stroke()) {
        return -1;
    }

    // else, find the start vertex
    int num_odd = 0;
    for (int i = 0; i < num_vert; i++) {
        if (has_odd_edges(i)) {
            num_odd++;
        }
    }

    // if all vertices have even number of edges, then start at any vertex
    //   > start at vertex 1
    if (num_odd == 0)
        return 1;

    // if exactly two vertices have odd number of edges, then start at one of the two vertices w/ odd number of edges
    //   > use the first vertex w/ odd number of edges
    if (num_odd == 2) 
    {
        for (int i = 0; i < num_vert; i++) {
            if (has_odd_edges(i))
                return (i+1);
        }
    }

    // if both of the following two conditions fail,
    //
    //     (1) all vertices have even number of edges
    //     (2) the number of vertices w/ odd edges DNE two vertices
    //
    // the `one_stroke()` function should return 'false' at the beginning of
    // this function and this point should never be reached
    return -1;
}


vector<list<int>> removeEdge(vector<list<int>> adj_list, int a, int b)
{
    // the parameters 'a' and 'b' represent vertex INDEXES

    list<int>::iterator lit;

    // remove the vertex w/ value 'b' from adj. list for vertex 'a'
    for (lit = adj_list[a].begin(); lit != adj_list[a].end(); lit++) {
        if (*lit == (b+1)) {
            adj_list[a].erase(lit);
            break;
        }
    }

    // remove the vertex w/ value 'a' from adj. list for vertex 'b'
    for (lit = adj_list[b].begin(); lit != adj_list[b].end(); lit++) {
        if (*lit == (a+1)) {
            adj_list[b].erase(lit);
            break;
        }
    }
    return adj_list;
}


void Graph::path()
{
    // make a copy of the graph's adjacency list
    vector<list<int>> adj_list = adj;

    // if the graph can't be drawn in one stroke, end function
    if (!one_stroke()) {
        cout << "No Solution." << endl;
        return;
    }

    // find the index of the starting vertex
    int start_ind = start_vertex() - 1;

    // initialize a stack and a path
    list<int> s;      // `s` contains vertex INDEXES
    list<int> path;   // `path` contains vertex VALUES
    
    // initialize variable for the current index
    int cur_ind = start_ind;    // 'cur' represents a vertex INDEX
    int next_cur_ind;

    while ((s.size() != 0) || (adj_list[cur_ind].size() != 0))
    {   
        if (adj_list[cur_ind].size() != 0)  // if the current vertex has at least 1 adjacent vertex
        {
            next_cur_ind = adj_list[cur_ind].front() - 1;

            s.push_back(cur_ind);   // pushes the INDEX of the vertex (not the value of the vertex. the value is the index + 1)
            adj_list = removeEdge(adj_list, cur_ind, next_cur_ind);
            cur_ind = next_cur_ind;
        }
        else    // if the current vertex has 0 adjacent vertices
        {
            path.push_back(cur_ind + 1);
            cur_ind = s.back();
            s.pop_back();
        }
    }

    // print the path
    cout << (start_ind + 1);

    list<int>::iterator it = path.end();
    it--;

    while (it != path.begin()) {
        cout << " -> " << *it;
        it--;
    }
    cout << " -> " << path.front() << endl;

}


/* this function prints a visual representation of the adjacency lists of the graph */
void Graph::display_graph()
{
    list<int>::iterator lit;

    for (int i = 0; i < num_vert; i++) {
        if (adj[i].size() != 0)
        {
            cout << (i+1);
            for (lit = adj[i].begin(); lit != adj[i].end(); lit++)
            {
                cout << " -> " << *lit;
            }
            cout << endl;
        }
    }
}
