#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_traits_3.h>
#include <CGAL/Polyhedron_items_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/Exact_integer.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/box_intersection_d.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>

namespace py = pybind11;
namespace PMP = CGAL::Polygon_mesh_processing;

using boost::lexical_cast;

using K_exact               = CGAL::Exact_predicates_inexact_constructions_kernel;
using K                     = CGAL::Simple_cartesian<double>;
using Triangle              = K::Triangle_3;
using Point_3               = K::Point_3;
using Plane                 = K::Plane_3;
using Vector                = K::Vector_3;
using Segment               = K::Segment_3;
using Ray                   = K::Ray_3;
using P_t                   = CGAL::Polyhedron_traits_3<K>;
using P_i                   = CGAL::Polyhedron_items_3;
using Polyhedron            = CGAL::Polyhedron_3<P_t, P_i>;
using Halfedge_handle       = Polyhedron::Halfedge_handle;
using Halfedge_const_handle = Polyhedron::Halfedge_const_handle;
using Facet_const_iterator  = Polyhedron::Facet_const_iterator;
using Facet_const_handle    = Polyhedron::Facet_const_handle;
using Primitive             = CGAL::AABB_face_graph_triangle_primitive<Polyhedron>;
using Traits                = CGAL::AABB_traits<K, Primitive>;
using Tree                  = CGAL::AABB_tree<Traits>;
using Segment_intersection  = boost::optional <Tree::Intersection_and_primitive_id<Segment>::Type>;
using Plane_intersection    = boost::optional <Tree::Intersection_and_primitive_id<Plane>::Type>;
using Primitive_id          = Tree::Primitive_id;

using Mesh                  = CGAL::Surface_mesh<Point_3>;
using face_descriptor       = boost::graph_traits<Mesh>::face_descriptor;
using Vertex_index          = Mesh::Vertex_index;
using Face_index            = Mesh::Face_index;





int get_first_integer(const char *v)
{
    int i;
    std::string s(v);
    std::replace(s.begin(), s.end(), '/', ' ');
    sscanf(s.c_str(), "%d", &i);
    return i;
}


template<typename T>
T StringToNumber(const std::string& numberAsString)
{
  T valor;

  std::stringstream stream(numberAsString);
  stream >> valor;
  if (stream.fail()) {
     std::runtime_error e(numberAsString);
     throw e;
  }
  return valor;
}


std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<std::size_t> >> load_obj(std::string filename)
{
    std::vector<std::vector<double>> points;
    std::vector<std::vector<std::size_t>> faces;


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
            std::string str;
            str =  std::string(line);
            std::istringstream tokenStream(str);
            std::string token;
            points.push_back(std::vector<double>());
            while (std::getline(tokenStream, token, ' '))
            {
                if (token[0] != 'v')
                    points.back().push_back(StringToNumber<double>(token));
            }
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
    return std::make_tuple(points,faces);

}

bool does_self_intersect (Mesh& mesh)
{
    return PMP::does_self_intersect(mesh, PMP::parameters::vertex_point_map(get(CGAL::vertex_point, mesh)));
}

//std::vector<std::pair<face_descriptor, face_descriptor>> self_intersections(Mesh& mesh)
std::vector<std::tuple<int, int>> self_intersections(Mesh& mesh)

{
    std::vector<std::pair<face_descriptor, face_descriptor>> intersected_tris;
    PMP::self_intersections(mesh, std::back_inserter(intersected_tris));

    std::vector<std::tuple<int, int>> list;
    for (std::size_t i=0; i< intersected_tris.size(); i++)
    {
        std::tuple<int, int> tmp = std::make_tuple(intersected_tris[i].first, intersected_tris[i].second);

        list.push_back(tmp);

    }

    return list;

}


PYBIND11_MODULE(cgal_mesher, m)
{
    m.def("load_obj", &load_obj);

    m.def("does_self_intersect", &does_self_intersect);

    m.def("self_intersections", &self_intersections);

    py::class_<Halfedge_handle>(m,"Halfedge_handle")
                .def(py::init<>())
    ;

    py::class_<Vertex_index>(m, "Vertex_index")
                .def(py::init<>())
    ;
    py::class_<Face_index>(m, "Face_index")
                .def(py::init<>())
                /*.def_property_readonly("i", &Face_index::i)
                .def("__repr__",
                     [](const Face_index &fi)
                     {
                        return boost::lexical_cast<std::string>(fi.i());
                     })*/
    ;


    py::class_<Mesh>(m, "Mesh")
                .def(py::init<>())
                .def(py::init<Mesh&>())
                .def("number_of_vertices",
                     [](Mesh& m)
                     {
                        return m.number_of_vertices();
                     })
                .def("number_of_faces",
                     [](Mesh& m)
                     {
                        return m.number_of_faces();
                     })
                .def("add_vertex",
                     [](Mesh& m)
                     {
                        return m.add_vertex();
                     })
                .def("add_vertex",
                     [](Mesh& m, const Point_3& p)
                     {
                        return m.add_vertex(p);
                     })
                .def("add_face",
                     [](Mesh& m, Vertex_index& v0, Vertex_index& v1, Vertex_index& v2)
                     {
                        return m.add_face(v0, v1, v2);
                     })
    ;

    /*py::class_<Polygon_mesh_processing>(m, "Polygon_mesh_processing")

    ;*/

    py::class_<Point_3>(m,"Point_3")
                .def(py::init<int,int,int>(), py::arg("x"), py::arg("y"), py::arg("z"))
                .def(py::init<float,float,float>(), py::arg("x"), py::arg("y"), py::arg("z"))

                .def_property_readonly("x", &Point_3::x)
                .def_property_readonly("y", &Point_3::y)
                .def_property_readonly("z", &Point_3::z)

                .def("__repr__",
                     [](const Point_3 &p)
                     {
                        std::string s("Point(");
                        s += boost::lexical_cast<std::string>(p.x());
                        s += ", ";
                        s += boost::lexical_cast<std::string>(p.y());
                        s += ", ";
                        s += boost::lexical_cast<std::string>(p.z());
                        s += ")";
                        return s;
                     })


    ;

    py::class_<Polyhedron>(m, "Polyhedron")
                .def(py::init<>())
                .def(py::init<P_t&>())
                //.def_property_readonly("face", &Polyhedron::Face)

                .def("size_of_facets",
                     [](Polyhedron& p)
                     {
                        return p.size_of_facets();
                     })

                .def("make_triangle",
                     [](Polyhedron& p)
                     {
                        return p.make_triangle();
                     })
                .def("make_triangle",
                     [](Polyhedron& p, const Point_3& p1, const Point_3& p2, const Point_3& p3)
                     {
                        return p.make_triangle(p1, p2, p3);
                     })
                .def("make_tetrahedron",
                     [](Polyhedron& p, const Point_3& p1, const Point_3& p2, const Point_3& p3, const Point_3& p4)
                     {
                        return p.make_tetrahedron(p1, p2, p3, p4);
                     })
                .def("is_triangle",
                     [](Polyhedron& p, Halfedge_handle h)
                     {
                        return p.is_triangle(h);
                     })
                .def("is_tetrahedron",
                     [](Polyhedron& p, Halfedge_handle h)
                     {
                        return p.is_tetrahedron(h);
                     })
    ;


    py::class_<Segment>(m,"Segment")
                .def(py::init<>())
                .def(py::init<Point_3,Point_3>())

    ;
    py::class_<Triangle>(m, "Triangle")
                .def(py::init<>())
                .def(py::init<Point_3, Point_3, Point_3>())
    ;

    py::class_<Tree>(m,"Tree")
                // Default no args constructor
                .def(py::init<>())
                .def("insert",
                     [](Tree &t, Polyhedron &p)
                     {
                        return t.insert(faces(p).first, faces(p).second, p);
                     })
                .def("build",
                     [](Tree& t)
                     {
                        return t.build();
                     })

                .def("do_intersect",
                     [](Tree& t, Segment& q)
                     {
                        return t.do_intersect(q);
                     })
                /*.def("do_intersect_p",
                     [](Tree& t, Polyhedron p)
                     {
                        return t.do_intersect(p);
                     })*/


                .def("number_of_intersected_primitives",
                     [](Tree& t, Segment& q)
                     {
                        return t.number_of_intersected_primitives(q);
                     })
                .def("number_of_intersected_primitives",
                     [](Tree& t, Triangle& tr)
                     {
                        return t.number_of_intersected_primitives(tr);
                     })
                /*.def("any_intersected_primitive",
                     [](Tree& t, Segment& q)
                     {
                        return t.any_intersected_primitive(q);
                     })*/
                /*.def("all_intersected_primitives",
                     [](Tree& t, Segment& q, std::list<Segment_intersection>& l)
                     {
                        return t.all_intersected_primitives(q, std::back_inserter(l));
                     })*/

                .def("any_intersection",
                     [](Tree& t, Segment& q)
                     {
                        return t.any_intersection(q);
                     })
                .def("any_intersection",
                     [](Tree& t, Triangle& tr)
                     {
                        return t.any_intersection(tr);
                     })

                .def("accelerate_distance_queries",
                     [](Tree& t)
                     {
                        t.accelerate_distance_queries();
                     })
                .def("squared_distance",
                     [](Tree& t, Point_3& p)
                     {
                        return t.squared_distance(p);
                     })
                .def("closest_point",
                     [](Tree& t, Point_3& p)
                     {
                        return t.closest_point(p);
                     })

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