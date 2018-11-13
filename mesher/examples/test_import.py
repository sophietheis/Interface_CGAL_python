#from mesher.cgal_mesher import ConstrainedDelaunayTriangulation as CDT
from mesher.cgal_mesher import (load_obj,
                                import_sheet_into_Mesh,
                                does_self_intersect,
                                self_intersections,
                                draw)

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

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


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


def main_tmp():
    mesh = Mesh()
    print(mesh)
    file_name = "examples/embryo_triangle.obj"
    file_name = "examples/embryo_intersect.obj"

    coords, tris = load_obj(file_name)
    print(len(coords))
    print(len(tris))
    list_coords_address = []
    [list_coords_address.append(mesh.add_vertex(
        Point_3(coord[0], coord[1], coord[2]))) for coord in coords]

    print(mesh.number_of_vertices())

    list_faces_adress = []
    """[list_faces_adress.append(mesh.add_face(list_coords_address[tri[0]], list_coords_address[
                              tri[1]], list_coords_address[tri[2]])) for tri in tris]
    """
    for tri in tris:
        list_face = []
        for t in tri:
            list_face.append(list_coords_address[t])
        list_faces_adress.append(mesh.add_face(list_face))

    print(mesh.number_of_faces())

    if does_self_intersect(mesh):
        print("There is at least one intersection")
        res = self_intersections(mesh)
        print("nombre d'intersection", len(res))
        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1, projection='3d')
        for i in range(0, len(res)):

            print("face0:", res[i][0], "face1:", res[i][1])
            print(tris[res[i][0]], tris[res[i][1]])
            for j in range(0, 3):
                print(coords[tris[res[i][0]][j]], coords[tris[res[1][0]][j]])
            if (0 < coords[tris[res[i][0]][0]][2] < 80 and
                    0 < coords[tris[res[i][1]][0]][2] < 80):
                if coords[tris[res[i][0]][0]][0] > 0:
                    a1 = coords[tris[res[i][0]][0]]
                    a2 = coords[tris[res[i][0]][1]]
                    a3 = coords[tris[res[i][0]][2]]
                    b1 = coords[tris[res[i][1]][0]]
                    b2 = coords[tris[res[i][1]][1]]
                    b3 = coords[tris[res[i][1]][2]]
                else:
                    a1 = coords[tris[res[i][1]][0]]
                    a2 = coords[tris[res[i][1]][1]]
                    a3 = coords[tris[res[i][1]][2]]
                    b1 = coords[tris[res[i][0]][0]]
                    b2 = coords[tris[res[i][0]][1]]
                    b3 = coords[tris[res[i][0]][2]]

                x = [a1[0], a2[0], a3[0], a1[0]]
                y = [a1[1], a2[1], a3[1], a1[1]]
                z = [a1[2], a2[2], a3[2], a1[2]]
                ax.plot(x, y, z, color='k')
            #print(tris[res[0][0]], tris[res[0][1]])
                x = [b1[0], b2[0], b3[0], b1[0]]
                y = [b1[1], b2[1], b3[1], b1[1]]
                z = [b1[2], b2[2], b3[2], b1[2]]
                ax.plot(x, y, z, color='red')
        plt.show()

    else:
        print("There is no intersection")

import os
import time
import numpy as np
from pathlib import Path

from tyssue import Sheet, config
from tyssue.io import hdf5


def triangular_to_polygonal_mesh(sheet, vertices):
    vertices = vertices[sheet.Nf:]

    sheet.vert_df[['x', 'y', 'z']] = vertices


def resolve_intersection(sheet):
    # save the previous position of vertices
    sheet.vert_df[['x_old', 'y_old', 'z_old']] = sheet.vert_df[['x', 'y', 'z']]

    """
    ----------
    Execute some events
    ----------

    """

    # Sheet in surface mesh
    mesh = Mesh()
    vertices, faces, _ = sheet.triangular_mesh(sheet.coords)
    mesh = import_sheet_into_Mesh(faces, vertices)

    #a, f = import_sheet_into_Mesh_python(sheet, mesh)

    if does_self_intersect(mesh):
        print("There is at least one intersection")
        list_intersected_faces = self_intersections(mesh)
        print(len(list_intersected_faces))
        for i in range(0, len(list_intersected_faces)):
            face1 = sheet.edge_df.loc[list_intersected_faces[i][0], 'face']
            face2 = sheet.edge_df.loc[list_intersected_faces[i][1], 'face']

            if face1 != face2:
                center_face1 = sheet.face_df.loc[face1, ['x', 'y', 'z']].values
                center_face2 = sheet.face_df.loc[face2][['x', 'y', 'z']].values
                list_edge1 = sheet.edge_df[(sheet.edge_df.face == face1)][
                    'srce'].values
                list_edge2 = sheet.edge_df[(sheet.edge_df.face == face2)][
                    'srce'].values

                for edge in list_edge1:
                    pos_e = sheet.vert_df.loc[edge][['x', 'y', 'z']].values
                    print(pos_e)


    else:
        print("There is no interaction")



def main():

    # --------- Chargement d'un fichier HDF5 --------- #
    SIM_DIR = Path(
        '/media/admin-suz/Sophie/2018/Papiers-EMT-Melanie/datas/SimulationsReview/2018-06-26/')

    dirname = SIM_DIR / '1.32_contractility_5_critical_area_50_radialtension/'
    current = 'invagination_0026.hf5'
    dsets = hdf5.load_datasets(os.path.join(dirname, current),
                               data_names=['vert', 'edge', 'face', 'cell'])

    specs = config.geometry.cylindrical_sheet()
    sheet = Sheet('ellipse', dsets, specs)

    resolve_intersection(sheet)

    """print(sheet.vert_df.loc[0]['x'])
    # -------- HDF5 dans mesh -------- #
    mesh = Mesh()

    # triangulation of the epithelium
    vertices, faces, _ = sheet.triangular_mesh(sheet.coords)

    vertices[sheet.Nf][0] = 22

    triangular_to_polygonal_mesh(sheet, vertices)
    print(sheet.vert_df.loc[0]['x'])
    """

    """list_coords_address = []
    [list_coords_address.append(mesh.add_vertex(
        Point_3(coord['x'], coord['y'], coord['z'])))
    for  index, coord in sheet.vert_df[['x', 'y', 'z']].iterrows()]

    for tri in tris:
        list_face = []
        for t in tri:
            list_face.append(list_coords_address[t])
        list_faces_adress.append(mesh.add_face(list_face))
    """


if __name__ == '__main__':
    main()

    """fig = plt.figure()
    ax = fig.add_subplot(1,1, 1, projection='3d')
    ax.plot([1, 2], [1, 2], [1, 2], color='k')
    ax.plot([2, 3], [2, 3], [1, 2], color='k')
    ax.plot([3, 4], [3, 4], [1, 2], color='k')
    plt.show()
    """
