#from mesher.cgal_mesher import ConstrainedDelaunayTriangulation as CDT
from mesher.cgal_mesher import load_obj
from mesher.cgal_mesher import (Polyhedron,
                                Point_3,
                                Halfedge_handle
                                )


def main():

    # Load the input file
    file_name = "examples/embryo.obj"
    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))

    # Build an empty polyhedron
    P = Polyhedron()
    print(P)
    P.make_triangle(Point_3(coords[0][0], coords[0][1], coords[0][2]),
                    Point_3(coords[1][0], coords[1][1], coords[1][2]),
                    Point_3(coords[2][0], coords[2][1], coords[2][2]))

    h = Halfedge_handle()
    h = P.make_triangle()
    print(P.is_triangle(h))

if __name__ == '__main__':
    main()
