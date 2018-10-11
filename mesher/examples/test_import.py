#from mesher.cgal_mesher import ConstrainedDelaunayTriangulation as CDT
from mesher.cgal_mesher import load_obj


def main():
    file_name = "examples/embryo.obj"
    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))


if __name__ == '__main__':
    main()
