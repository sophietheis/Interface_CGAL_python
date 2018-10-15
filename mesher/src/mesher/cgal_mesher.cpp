#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_traits_3.h>
#include <CGAL/Polyhedron_items_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
//#include "polyhedron_builder.h"

namespace py = pybind11;


using K             = CGAL::Simple_cartesian<double>;
using P_t           = CGAL::Polyhedron_traits_3<K>;
using P_i           = CGAL::Polyhedron_items_3;
using Polyhedron    = CGAL::Polyhedron_3<P_t, P_i>;
//using HalfedgeDS    = Polyhedron::HalfedgeDS;
using Point_3       = Polyhedron::Point_3;

int get_first_integer(const char *v)
{
    int i;
    std::string s(v);
    std::replace(s.begin(), s.end(), '/', ' ');
    sscanf(s.c_str(), "%d", &i);
    return i;
}

std::tuple<std::vector<double>, std::vector<std::vector<std::size_t> >> load_obj(std::string filename)
{
    // two vectors to hold point coordinates and
    // triangle vertex indices
    std::vector<double> coords;
    //std::vector<int>    tris;
    std::vector<std::vector<std::size_t> > faces;


    double x, y, z;
    char line[1024];
    char v0[1024], v1[1024], v2[1024];


    // Open file
    FILE *f = fopen(filename.c_str(), "r");
    //if (!f) return false;

    while(fgets(line, 1024, f))
    {
        if (line[0] == 'v' && line[1]!='n' )
        {
            sscanf( line, "%*s%lf%lf%lf", &x, &y, &z );
            coords.push_back( x );
            coords.push_back( y );
            coords.push_back( z );
        }
        else if(line[0] == 'f')
        {
            std::string str;
            str =  std::string(line);
            std::istringstream iss(str.substr(1));
            int i;
            faces.push_back( std::vector<std::size_t>() );
            while (iss >> i)
            {
                faces.back().push_back(i-1);
                iss.ignore(256, ' ');
            }
        }
    }
    return std::make_tuple(coords,faces);

}

// Didn't work...
/*template<class HDS>
class Polyhedron_builder : public CGAL::Modifier_base<HDS> {
public:
    std::vector<double> &coords;
    std::vector<int>    &tris;
    Polyhedron_builder( std::vector<double> &_coords, std::vector<int> &_tris ) : coords(_coords), tris(_tris) {}
    void operator()( HDS& hds) {
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;

        // create a cgal incremental builder
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        B.begin_surface( coords.size()/3, tris.size()/3 );

        // add the polyhedron vertices
        for( int i=0; i<(int)coords.size(); i+=3 ){
            B.add_vertex( Point( coords[i+0], coords[i+1], coords[i+2] ) );
        }

        // add the polyhedron triangles
        for( int i=0; i<(int)tris.size(); i+=3 ){
            B.begin_facet();
            B.add_vertex_to_facet( tris[i+0] );
            B.add_vertex_to_facet( tris[i+1] );
            B.add_vertex_to_facet( tris[i+2] );
            B.end_facet();
        }

        // finish up the surface
        B.end_surface();
    }
};*/


PYBIND11_MODULE(cgal_mesher, m)
{
    m.def("load_obj", &load_obj);

    /*py::class_<Polyhedron_builder>(m,"Polyhedron_builder")
                //.def(py::init<std::vector<double>, std::vector<int>>())
    ;*/

    py::class_<Point_3>(m,"Point_3")
                .def(py::init<int,int,int>(), py::arg("x"), py::arg("y"), py::arg("z"))
                .def(py::init<float,float,float>(), py::arg("x"), py::arg("y"), py::arg("z"))
                .def_property_readonly("x", &Point_3::x)
                .def_property_readonly("y", &Point_3::y)
                .def_property_readonly("z", &Point_3::z)

    ;

    py::class_<Polyhedron>(m, "Polyhedron")
                .def(py::init<P_t&>())
                .def("make_triangle",
                     [](Polyhedron& p, const Point_3& p1,const Point_3& p2,const Point_3& p3)
                     {
                        p.make_triangle(p1, p2, p3);
                     })

                /*.def("delegate", [](Polyhedron & p, Modifier_base<HDS>& modifier)
                     {
                        p.delegate(modifier);
                     })*/
    ;



}
/*
#include <pybind11/pybind11.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_vertex_base_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>

#include <boost/lexical_cast.hpp>
#include <iterator>
#include <cstddef>

namespace py = pybind11;

using K             = CGAL::Exact_predicates_inexact_constructions_kernel;
using Vb            = CGAL::Delaunay_mesh_vertex_base_2<K>;
using Fb            = CGAL::Delaunay_mesh_face_base_2<K>;
using Tds           = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using CDT           = CGAL::Constrained_Delaunay_triangulation_2<K, Tds>;
using Criteria      = CGAL::Delaunay_mesh_size_criteria_2<CDT>;
using Mesher        = CGAL::Delaunay_mesher_2<CDT, Criteria>;
using Point         = CDT::Point;
using Vertex_handle = CDT::Vertex_handle;


template <typename T>
class TypedInputIterator
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::input_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;

    explicit TypedInputIterator(py::iterator& py_iter):
            py_iter_(py_iter){}

    explicit TypedInputIterator(py::iterator&& py_iter):
            py_iter_(py_iter){}

    value_type operator*(){
        return (*py_iter_).template cast<value_type>();
    }

    TypedInputIterator operator++(int){
        auto copy = *this;
        ++py_iter_;
        return copy;
    }

    TypedInputIterator& operator++(){
        ++py_iter_;
        return *this;
    }

    bool operator != (TypedInputIterator &rhs) {
        return py_iter_ != rhs.py_iter_;
    }

    bool operator == (TypedInputIterator &rhs) {
        return py_iter_ == rhs.py_iter_;
    }

private:
    py::iterator py_iter_;
};


// template from https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/20170989#20170989
template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}


PYBIND11_MODULE(cgal_mesher, m)
{

    py::class_<Point>(m, "Point")
                    .def(py::init<int,int>(), py::arg("x"), py::arg("y"))
                    .def(py::init<double,double>(), py::arg("x"), py::arg("y"))
                    .def_property_readonly("x", &Point::x)
                    .def_property_readonly("y", &Point::y)
                    .def("__repr__",
                         [](const Point &p){
                            std::string r("Point(");
                            r += boost::lexical_cast<std::string>(p.x());
                            r += ", ";
                            r += boost::lexical_cast<std::string>(p.y());
                            r +=")";
                            return r;
                         })
                    .def("__hash__",
                         [](const Point &p){
                            std::hash<double> double_hash;
                            auto x_hash = double_hash(p.x());
                            auto y_hash = double_hash(p.y());
                            return y_hash ^ x_hash + 0x9e1779b9 + (y_hash << 6) + (y_hash >> 2);
                         })
                    .def("__eq__",
                         [](const Point &p, const Point & q) {
                            return p == q;
                         })
                    ;

    py::class_<Vertex_handle>(m,"Vertexhandle");

    py::class_<CDT>(m, "ConstrainedDelaunayTriangulation")
                    .def(py::init())
                    .def("insert", [](CDT & cdt, const Point & p) {return cdt.insert(p); })
                    .def("insert_constraint",
                         [](CDT & cdt, Vertex_handle a, Vertex_handle b)
                         {
                            cdt.insert_constraint(a, b);
                         })
                    .def("number_of_vertices", &CDT::number_of_vertices)
                    .def("number_of_faces", &CDT::number_of_faces)
                    .def("finite_vertices", [](CDT & cdt) -> py::iterator
                    {
                        return py::make_iterator(cdt.finite_vertices_begin(), cdt.finite_vertices_end());
                    })
                    .def("finite_faces", [](CDT & cdt) -> py::iterator
                    {
                        return py::make_iterator(cdt.finite_faces_begin(), cdt.finite_faces_end());
                    })
                    ;

    py::class_<Criteria>(m, "Criteria")
                    .def(py::init<double,double>(),
                         py::arg("aspect_bound")=0.125,
                         py::arg("size_bound")=0.0)
                    .def_property("size_bound", &Criteria::size_bound, & Criteria::set_size_bound)
                    .def_property("aspect_bound",
                                  [](const Criteria & c) {c.bound();},
                                  [](Criteria & c, double bound) {c.set_bound(bound); })
                    ;

    py::class_<Mesher>(m, "Mesher")
                    .def(py::init<CDT&>())
                    .def("seeds_from", [](Mesher & mesher, py::iterable iterable)
                    {
                        py::iterator iterator = py::iter(iterable);
                        TypedInputIterator<Point> points_begin(iterator);
                        TypedInputIterator<Point> points_end(py::iterator::sentinel());
                        mesher.set_seeds(points_begin, points_end);
                    })
                    .def("refine_mesh", &Mesher::refine_mesh)
                    .def_property(
                                  "criteria",
                                  &Mesher::get_criteria,
                                  [](Mesher& mesher, const Criteria & criteria)
                                  {
                                    mesher.set_criteria(criteria);
                                  })
                    ;

    m.def("make_conforming_delaunay",
                    &CGAL::make_conforming_Delaunay_2<CDT>,
                    py::arg("cdt"),
                    "Make a triangulation conform to the Delaunay property"
                    );

    m.def("make_conforming_gabriel",
                    &CGAL::make_conforming_Gabriel_2<CDT>,
                    py::arg("cdt"),
                    "Make a triangulation conform to the Gabriel property"
                    );

    m.def("lloyd_optimize", [](
                    CDT& cdt,
                    int max_iteration_number,
                    double time_limit,
                    double convergence,
                    double freeze_bound)
                    {
                        CGAL::lloyd_optimize_mesh_2(cdt,
                            CGAL::parameters::max_iteration_number = max_iteration_number,
                            CGAL::parameters::time_limit = time_limit,
                            CGAL::parameters::convergence = convergence,
                            CGAL::parameters::freeze_bound = freeze_bound
                            );
                    },
                    py::arg("cdt"),
                    py::arg("max_iteration_number") = 0,
                    py::arg("time_limit") = 0.0,
                    py::arg("convergence") = 0.001,
                    py::arg("freeze_bound") = 0.001
                    );

    m.def("print_faces_iterator_value_type", [](){
                    std::cout << type_name<CDT::Finite_faces_iterator::value_type>();
                    });
}



*/