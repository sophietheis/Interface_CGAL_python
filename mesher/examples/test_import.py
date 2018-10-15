#from mesher.cgal_mesher import ConstrainedDelaunayTriangulation as CDT
from mesher.cgal_mesher import load_obj
from mesher.cgal_mesher import Polyhedron


def main():

    # Load the input file
    file_name = "examples/embryo.obj"
    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))
    print(type(coords))

    print(coords[0], coords[1], coords[2])

    print(coords[-3], coords[-2], coords[-1])

    print(tris[-3], tris[-2], tris[-1])
    # Build an empty polyhedron
    P = Polyhedron
    #P.make_triangle(coords[0], coords[1], coords[2])
    print(P)

if __name__ == '__main__':
    main()
