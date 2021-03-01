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

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

namespace
{
    const int root = 0;

    // This is the graph, and resulting dominator tree, from the original
    // paper.
    //
    const std::vector<std::pair<char, char>> testgraph =
    {
        { 'R', 'C' },
        { 'R', 'B' },
        { 'R', 'A' },
        { 'C', 'F' },
        { 'C', 'G' },
        { 'F', 'I' },
        { 'G', 'J' },
        { 'G', 'I' },
        { 'J', 'I' },
        { 'I', 'K' },
        { 'K', 'I' },
        { 'K', 'R' },
        { 'B', 'E' },
        { 'B', 'A' },
        { 'B', 'D' },
        { 'E', 'H' },
        { 'H', 'E' },
        { 'H', 'K' },
        { 'A', 'D' },
        { 'D', 'L' },
        { 'L', 'H' }
    };

    const std::vector<std::pair<char, char>> answer =
    {
        { 'A', 'R' },
        { 'B', 'R' },
        { 'C', 'R' },
        { 'D', 'R' },
        { 'E', 'R' },
        { 'F', 'C' },
        { 'G', 'C' },
        { 'H', 'R' },
        { 'I', 'R' },
        { 'J', 'G' },
        { 'K', 'R' },
        { 'L', 'D' }
    };

    // convert vertex to index, with 0 being root (R)
    //
    auto tovert(char ch) -> int
    {
        if (ch == 'R')
        {
            return 0;
        }

        return 1 + ch - 'A';
    }

    // convert index back to vertex name
    //
    auto toname(int vert) -> string
    {
        if (vert == 0)
        {
            return "R";
        }

        return string{ (char)('A' - 1 + vert) };
    }
}

auto main(int argc, char *argv[]) -> int
{
    DomTree tree;
    int errors = 0;

    // add the test graph
    for (const auto &edge : testgraph)
    {
        tree.add_edge(tovert(edge.first), tovert(edge.second));
    }

    // compute the dominators
    tree.run(root);

    // verify
    for (const auto &edge : answer)
    {
        int vert = tovert(edge.first);
        int dom = tovert(edge.second);

        if (dom != tree.dom(vert))
        {
            cout 
                << "tests failed!! dom(" 
                << edge.first 
                << ") is "
                << toname(tree.dom(vert))
                << " but should be "
                << edge.second
                << endl;
            errors++;
        }
    }

    return errors;
}
