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
#include "dom.h"

#include <algorithm>
#include <stdexcept>

using std::vector;

// Add an edge to the graph to be analyzed. Vertices are 
// expected to run from 0..n-1 if there are n vertices.
//
auto DomTree::add_edge(int from, int to) -> void
{
    if (from < 0 || to < 0)
    {
        throw std::range_error{ 
            "invalid (negative) vertex index passed to add_edge()" 
        };
    }

    int m = std::max(from, to);
    if (m >= _edges.size())
    {
        _edges.resize(m + 1);
    }

    _edges[from].push_back(to);
}

// Construct the dominator tree
//
auto DomTree::run(int root) -> void
{
    if (root < 0 || root >= _edges.size())
    {
        throw std::range_error{ 
            "invalid (negative or out of range) vertex index "
            "passed as root to run()" 
        };
    }
    
    int size = static_cast<int>(_edges.size());

    // initialize arrays
    //
    _sdoms.resize(size, invalid_id);
    _vertex.resize(size, invalid_vtx);
    _parent.resize(size, invalid_vtx);
    _redges.resize(size);
    _ancestor.resize(size, invalid_vtx);
    _dom.resize(size, invalid_vtx);

    vector<vector<int>> bucket(size);

    // unlike other arrays, labels are initialized as the indentity
    // function
    // 
    for (int i = 0; i < size; i++)
    {
        _label.push_back(i);
    }

    // first, do a DFS on the graph to build a minimum spanning tree
    // of the graph, and label the vertices in preorder
    //
    _next_id = 0;
    build_mst(root);

    // right now _sdoms contains the vertex id of each vertex. compute
    // the actual semi-dominator, working from the bottom of the mst
    // back towards the root
    //
    for (int i = size - 1; i > 0; --i)
    {
        int w = _vertex[i];
        for (int v : _redges[w])
        {
            int u = eval(v);
            if (_sdoms[u] < _sdoms[w])
            {
                _sdoms[w] = _sdoms[u];
            }
        }
        
        bucket[_vertex[_sdoms[w]]].push_back(w);
        link(_parent[w], w);

        int wpar = _parent[w];
        for (int v : bucket[wpar])
        {
            int u = eval(v);
            if (_sdoms[u] < _sdoms[v])
            {
                _dom[v] = u; 
            }
            else
            {
                _dom[v] = _parent[w];
            }
        }
        bucket[wpar].clear();
    }

    // compute the immediate dominators based on the semi-dominators
    //
    for (int i = 1; i < size; i++)
    {
        int w = _vertex[i];
        if (_dom[w] != _vertex[_sdoms[w]])
        {
            _dom[w] = _dom[_dom[w]];
        }
    }
}

// return the dominator of any vertex
//
auto DomTree::dom(int v) -> int
{
    if (v < 0 || v >= _edges.size())
    {
        throw std::range_error{ 
            "invalid (negative or out of range) vertex index "
            "passed to dom() (make sure run() has been called)" 
        };
    }

    return _dom[v];
}

// build a minimum spanning tree of the graph. label the vertices
// with ids based on a preorder traversal, and at the same time 
// save the reverse edges of the original graph.
//
// after this step _sdoms (which is indexed by vertex) contains 
// the id original vertex. it will be refined into the semi
// dominator later.
//
auto DomTree::build_mst(int from) -> void
{
    _sdoms[from] = _next_id;
    _vertex[_next_id] = from;
    _next_id++;

    for (int next : _edges[from])
    {
        if (_sdoms[next] == invalid_id)
        {
            _parent[next] = from;
            build_mst(next);
        }
        _redges[next].push_back(from);
    }
}

// compute u on the path from v to root, for which sdom(u) 
// is the lowest.
//
auto DomTree::eval(int v) -> int
{
    if (_ancestor[v] == invalid_vtx)
    {
        return v;
    }

    compress(v);
    return _label[v];
}

// walk up the subtree looking at already computed semi dominators,
// looking for the smallest. we do path compression here by relabeling
// vertices in _label
//
auto DomTree::compress(int v) -> int
{
    if (_ancestor[_ancestor[v]] != invalid_vtx)
    {
        compress(_ancestor[v]);
        if (_sdoms[_label[_ancestor[v]]] < _sdoms[_label[v]])
        {
            _label[v] = _label[_ancestor[v]];
        }
        _ancestor[v] = _ancestor[_ancestor[v]];
    }
}

// link the given nodes together in the forest
//
auto DomTree::link(int from, int to) -> void
{
    _ancestor[to] = from;
}


