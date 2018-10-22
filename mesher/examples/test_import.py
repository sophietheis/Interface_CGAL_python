#from mesher.cgal_mesher import ConstrainedDelaunayTriangulation as CDT
from mesher.cgal_mesher import (load_obj,
                                does_self_intersect,
                                self_intersections)

from mesher.cgal_mesher import (Polyhedron,
                                Point_3,
                                Halfedge_handle,
                                Segment,
                                Tree,
                                Triangle,
                                Mesh,
                                Vertex_index,
                                Face_index
                                )


def main_1():

    # Load the input file
    #file_name = "examples/embryo.obj"
    file_name = "examples/embryo.obj"

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


def main2():
    file_name = "examples/embryo_intersect.obj"

    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))

    # Build an empty polyhedron
    P = Polyhedron()
    h = Halfedge_handle()
    h = [P.make_triangle(Point_3(coords[tri[0]][0], coords[tri[0]][1], coords[tri[0]][2]),
                         Point_3(coords[tri[1]][0], coords[
                                 tri[1]][1], coords[tri[1]][2]),
                         Point_3(coords[tri[2]][0], coords[tri[2]][1], coords[tri[2]][2]))
         for tri in tris]

    print(intersection(P))


def main3():
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

    print(intersection(polyhedron))

    # Distance
    tree.accelerate_distance_queries()
    p = Point_3(0.0, 0.0, 2.0)
    sqd = tree.squared_distance(p)
    print(sqd)

    closest_point = tree.closest_point(p)
    print(closest_point)


def main_2():
    file_name = "examples/embryo.obj"

    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))

    # Build an empty polyhedron
    polyhedron = Polyhedron()
    h = Halfedge_handle()
    h = [polyhedron.make_triangle(Point_3(coords[tri[0]][0], coords[tri[0]][1], coords[tri[0]][2]),
                                  Point_3(coords[tri[1]][0], coords[
                                      tri[1]][1], coords[tri[1]][2]),
                                  Point_3(coords[tri[2]][0], coords[tri[2]][1], coords[tri[2]][2]))
         for tri in tris]

    # Construct AABB_tree
    tree = Tree()
    tree.insert(polyhedron)
    tree.build()
    print(tree)

    cpt = 0
    for i in range(0, len(tris)):
        triangle_query = Triangle(Point_3(coords[tris[i][0]][0], coords[tris[i][0]][1], coords[tris[i][0]][2]),
                                  Point_3(coords[tris[i][1]][0], coords[
                                          tris[i][1]][1], coords[tris[i][1]][2]),
                                  Point_3(coords[tris[i][2]][0], coords[tris[i][2]][1], coords[tris[i][2]][2]))

        if tree.number_of_intersected_primitives_t(
                triangle_query) == 1:
            cpt = cpt + 1
    print(cpt)


def main():
    mesh = Mesh()
    print(mesh)
    #file_name = "examples/embryo.obj"
    file_name = "examples/embryo_intersect.obj"

    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))
    list_coords_address = []
    [list_coords_address.append(mesh.add_vertex(
        Point_3(coord[0], coord[1], coord[2]))) for coord in coords]

    print(mesh.number_of_vertices())

    list_faces_adress = []
    [list_faces_adress.append(mesh.add_face(list_coords_address[tri[0]], list_coords_address[
                              tri[1]], list_coords_address[tri[2]])) for tri in tris]

    print(mesh.number_of_faces())
    if does_self_intersect(mesh):
        print("There is at least one intersection")
        res = self_intersections(mesh)
        print("nombre d'intersection de face", len(res))
        print("face0:", res[0][0], "face1:", res[0][1])
        print(tris[res[0][0]], tris[res[0][1]])
    else:
        print("There is no intersection")

if __name__ == '__main__':
    main()
