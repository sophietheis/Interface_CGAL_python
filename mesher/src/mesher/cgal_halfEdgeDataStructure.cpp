#include <pybind11/pybind11.h>
#include <boost/lexical_cast.hpp>

namespace py = pybind11;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_vertex_base_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>


using K             = CGAL::Exact_predicates_inexact_constructions_kernel;
using Vb            = CGAL::Delaunay_mesh_vertex_base_2<K>;
using Fb            = CGAL::Delaunay_mesh_face_base_2<K>;
using Tds           = CGAL::Triangulation_data_structure_2<Vb, Fb>;
using CDT           = CGAL::Constrained_Delaunay_triangulation_2<K, Tds>;
using Point         = CDT::Point;




PYBIND11_MODULE(cgal_HalfEdgeDataStructure, m)
{
    py::class_<Point>(m, "Point")
                    .def(py::init<int,int,int>(), py::arg("x"), py::arg("y"), py::arg("z"))
                    .def(py::init<double,double,double>(), py::arg("x"), py::arg("y"), py::arg("z"))

                    .def_property_readonly("x", &Point::x)
                    .def_property_readonly("y", &Point::y)
                    .def_property_readonly("z", &Point::z)

                    .def("__eq__",
                         [](const Point &p, const Point &q){
                            return p == q;
                         })
    ;

    py::class_<Vertex>(m, 'Vertex')
    // Definir les bases de ce qu'on a besoin pour les vertex depuis cgal
    // Ajouter les informations connus par la librairie tyssue
    // Idem pour edge et face et cell.
    ;

    py::class_<HalfEdges>()
    ;

    py::class_<Faces>()
    ;

}


