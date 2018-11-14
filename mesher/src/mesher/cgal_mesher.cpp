#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/numpy.h>
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>

namespace py = pybind11;
namespace PMP = CGAL::Polygon_mesh_processing;


using K                     = CGAL::Simple_cartesian<double>;
using Point_3               = K::Point_3;
using Plane                 = K::Plane_3;

using Mesh                  = CGAL::Surface_mesh<Point_3>;
using face_descriptor       = boost::graph_traits<Mesh>::face_descriptor;
using Vertex_index          = Mesh::Vertex_index;
using Face_index            = Mesh::Face_index;



Mesh import_sheet_into_Mesh(py::array_t<double> faces, py::array_t<double> vertices)
{

    Mesh mesh;

    // scan vertices numpy array
    py::buffer_info info_vertices = vertices.request();
    for (int i=0; i<info_vertices.shape[0]*3; i=i+3)
    {
        mesh.add_vertex(Point_3(((double*)info_vertices.ptr)[i],
                                ((double*)info_vertices.ptr)[i+1],
                                ((double*)info_vertices.ptr)[i+2]));
    }

    // scan faces numpy array
    py::buffer_info info_faces = faces.request();
    for (int i=0; i<info_faces.shape[0]; i=i+3)
    {
        std::vector<int> vertice_in_face;

        vertice_in_face.push_back(((double*)info_faces.ptr)[i]);
        vertice_in_face.push_back(((double*)info_faces.ptr)[i+1]);
        vertice_in_face.push_back(((double*)info_faces.ptr)[i+2]);

        mesh.add_face(vertice_in_face);
    }

    return mesh;
}


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

    m.def("import_sheet_into_Mesh", &import_sheet_into_Mesh);

    m.def("does_self_intersect", &does_self_intersect);

    m.def("self_intersections", &self_intersections);


    py::class_<Vertex_index>(m, "Vertex_index")
                .def(py::init<>())
    ;

    py::class_<Face_index>(m, "Face_index")
                .def(py::init<>())
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
                .def("add_face",
                     [](Mesh& m, std::vector<Vertex_index>& v)
                     {
                        return m.add_face(v);
                     })
    ;

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

}
