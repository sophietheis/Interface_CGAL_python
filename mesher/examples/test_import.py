#from mesher.cgal_mesher import ConstrainedDelaunayTriangulation as CDT
from mesher.cgal_mesher import load_obj
from mesher.cgal_mesher import Polyhedron


def main():

    # Load the input file
    file_name = "examples/embryo.obj"
    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))

    # Build an empty polyhedron
    P = Polyhedron
    print (P)

if __name__ == '__main__':
    main()
