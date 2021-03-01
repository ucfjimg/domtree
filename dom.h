/**
Copyright 2020 Jim Geist

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**/
#ifndef DOM_H_
#define DOM_H_

#include <vector>

// Compute dominators based on the Lengauer-Tarjan algorithm
// For exposition, see "A Fast Algorithm for Finding Dominators in a Flowgraph",
// Thomas Lengauer and Robert Tarjan, 1979.

class DomTree
{
public:
    // invalid vertex flag
    const int invalid_vtx = -1;

    // add an edge to the graph
    auto add_edge(int from, int to) -> void;

    // compute dominators relative to the graph
    auto run(int root) -> void;

    // return the dominator of the given vertex
    auto dom(int vertex) -> int;


private:
    // below, `vertex` is a vertex number in the original graph
    // `vertex id` is the preorder index a vertex is disovered in the DFS
    const int invalid_id = -1;

    int _next_id;
    std::vector<std::vector<int>> _edges;               // edge map
    std::vector<int> _sdoms;                            // semi-dominators by vertex
    std::vector<int> _vertex;                           // vertex by vertex id
    std::vector<int> _parent;                           // parent of vertex in minimum spanning tree 
    std::vector<std::vector<int>> _redges;              // graph as reverse edges
    std::vector<int> _label;                            // relabel vertices during compression
    std::vector<int> _ancestor;                         // ancestor by vertex
    std::vector<int> _dom;                              // dom by vertex


private:
    auto build_mst(int from) -> void;
    auto eval(int v) -> int;
    auto compress(int v) -> int;
    auto link(int from, int to) -> void;
};

#endif 

