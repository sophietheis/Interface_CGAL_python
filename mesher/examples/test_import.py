#from mesher.cgal_mesher import ConstrainedDelaunayTriangulation as CDT
from mesher.cgal_mesher import load_obj
from mesher.cgal_mesher import (Polyhedron,
                                Point_3,
                                Halfedge_handle,
                                Segment,
                                Tree
                                )


def main_1():

    # Load the input file
    #file_name = "examples/embryo.obj"
    file_name = "examples/embryo_triangle.obj"

    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))

    # Build an empty polyhedron
    P = Polyhedron()
    print(P)
    h = Halfedge_handle()
    print(h)
    h = [P.make_triangle(Point_3(coords[tri[0]][0], coords[tri[0]][1], coords[tri[0]][2]),
                         Point_3(coords[tri[1]][0], coords[
                                 tri[1]][1], coords[tri[1]][2]),
                         Point_3(coords[tri[2]][0], coords[tri[2]][1], coords[tri[2]][2]))
         for tri in tris]
    """
    h = P.make_triangle(Point_3(coords[0][0], coords[0][1], coords[0][2]),
                        Point_3(coords[1][0], coords[1][1], coords[1][2]),
                        Point_3(coords[2][0], coords[2][1], coords[2][2]))
    """
    #h = P.make_triangle()
    # print(P.is_triangle(h))
    # print(h)


def main():
    # Test colision
    p = Point_3(1.0, 0.0, 0.0)
    q = Point_3(0.0, 1.0, 0.0)
    r = Point_3(0.0, 0.0, 1.0)
    s = Point_3(0.0, 0.0, 0.0)

    polyhedron = Polyhedron()
    polyhedron.make_tetrahedron(p, q, r, s)

    # Construct AABB_tree
    tree = Tree()
    tree.insert(polyhedron)
    tree.build()
    print(tree)

    # Intersect line with polyhedron
    a = Point_3(-0.5, 0.1, -0.1)
    #b = Point_3(-1.3, 0.2, 1.3)
    b = Point_3(1, 1, 1)

    segment_query = Segment(a, b)
    print(a)
    print(segment_query)

    if tree.do_intersect(segment_query):
        print('intersection(s)')
    else:
        print('no intersection')

    print(tree.number_of_intersected_primitives(
        segment_query), "intersection(s)")


    #Distance
    tree.accelerate_distance_queries()
    p = Point_3(0.0, 0.0, 2.0)
    sqd = tree.squared_distance(p)
    print(sqd)

    closest_point = tree.closest_point(p)
    print (closest_point)


if __name__ == '__main__':
    main()
