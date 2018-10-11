#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

//#include <CGAL/IO/OBJ_reader.h>

namespace py = pybind11;




int get_first_integer(const char *v)
{
    int i;
    std::string s(v);
    std::replace(s.begin(), s.end(), '/', ' ');
    sscanf(s.c_str(), "%d", &i);
    return i;
}

std::tuple<std::vector<double>, std::vector<int>> load_obj(std::string filename)
//bool load_obj(std::string filename)
{
    //std::string filename="embryo.obj";
    // two vectors to hold point coordinates and
    // triangle vertex indices
    std::vector<double> coords;
    std::vector<int>    tris;


    double x, y, z;
    char line[1024];
    char v0[1024], v1[1024], v2[1024];


    // Open file
    FILE *f = fopen(filename.c_str(), "r");
    //if (!f) return false;

    while(fgets(line, 1024, f))
    {
        if (line[0] == 'v' )
        {
            sscanf( line, "%*s%lf%lf%lf", &x, &y, &z );
            coords.push_back( x );
            coords.push_back( y );
            coords.push_back( z );
        }
        else if(line[0] == 'f')
        {
            sscanf( line, "%*s%s%s%s", v0, v1, v2 );
            tris.push_back( get_first_integer( v0 )-1 );
            tris.push_back( get_first_integer( v1 )-1 );
            tris.push_back( get_first_integer( v2 )-1 );
        }
    }
    return std::make_tuple(coords,tris);

}


PYBIND11_MODULE(cgal_mesher, m)
{
    m.def("load_obj", &load_obj);


}