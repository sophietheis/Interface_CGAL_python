from mesher.cgal_mesher import (import_sheet_into_Mesh,
                                does_self_intersect,
                                self_intersections)

from mesher.cgal_mesher import (Point_3,
                                Mesh)

import os
import matplotlib.pyplot as plt
import numpy as np

from pathlib import Path
from tyssue import Sheet, config, SheetGeometry
from tyssue.io import hdf5, obj
from mpl_toolkits.mplot3d import *

from tyssue.generation import three_faces_sheet


def triangular_to_polygonal_mesh(sheet, vertices):
    vertices = vertices[sheet.Nf:]

    sheet.vert_df[['x', 'y', 'z']] = vertices


def list_intersected_facesolve_intersection(sheet):
    # save the previous position of vertices
    sheet.vert_df[['x_old', 'y_old', 'z_old']] = sheet.vert_df[['x', 'y', 'z']]

    """
    ----------
    Execute some events
    ----------

    """

    # Sheet in surface mesh
    #sheet = sheet.extract_bounding_box(z_boundary=[-100, 100])
    #sheet = sheet.extract("is_fold_patch")

    vertices, faces, _ = sheet.triangular_mesh(sheet.coords)
    vertices = np.array(vertices, order='C')
    faces = np.array(faces, order='C')
    mesh = import_sheet_into_Mesh(faces, vertices)
    print(len(faces))
    if does_self_intersect(mesh):
        print("There is at least one intersection")
        list_intersected_faces = self_intersections(mesh)
        print(len(list_intersected_faces))
        """
        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1, projection='3d')
        draw_ugly(list_intersected_faces, vertices, faces, ax)
        plt.show()
        """

    else:
        print("There is no interaction")


def draw_ugly(list_intersected_faces, vertices, faces, ax):
    for i in range(0, len(list_intersected_faces)):

        if (0 < vertices[faces[list_intersected_faces[i][0]][0]][2] < 80 and
                0 < vertices[faces[list_intersected_faces[i][1]][0]][2] < 80):
            if vertices[faces[list_intersected_faces[i][0]][0]][0] > 0:
                a1 = vertices[faces[list_intersected_faces[i][0]][0]]
                a2 = vertices[faces[list_intersected_faces[i][0]][1]]
                a3 = vertices[faces[list_intersected_faces[i][0]][2]]
                b1 = vertices[faces[list_intersected_faces[i][1]][0]]
                b2 = vertices[faces[list_intersected_faces[i][1]][1]]
                b3 = vertices[faces[list_intersected_faces[i][1]][2]]
            else:
                a1 = vertices[faces[list_intersected_faces[i][1]][0]]
                a2 = vertices[faces[list_intersected_faces[i][1]][1]]
                a3 = vertices[faces[list_intersected_faces[i][1]][2]]
                b1 = vertices[faces[list_intersected_faces[i][0]][0]]
                b2 = vertices[faces[list_intersected_faces[i][0]][1]]
                b3 = vertices[faces[list_intersected_faces[i][0]][2]]

            x = [a1[0], a2[0], a3[0], a1[0]]
            y = [a1[1], a2[1], a3[1], a1[1]]
            z = [a1[2], a2[2], a3[2], a1[2]]
            ax.plot(x, y, z, color='k')
            x = [b1[0], b2[0], b3[0], b1[0]]
            y = [b1[1], b2[1], b3[1], b1[1]]
            z = [b1[2], b2[2], b3[2], b1[2]]
            ax.plot(x, y, z, color='red')


def main():

    # --------- Chargement d'un fichier HDF5 --------- #

    SIM_DIR = Path(
        '/media/admin-suz/Sophie/2018/Papiers-EMT-Melanie/datas/2018-05-29_principal_results')
    dirname = SIM_DIR / '1.6400000000000001_contractility_5_critical_area_75_radialtension/'
    current = 'invagination_0001.hf5'

    #SIM_DIR = Path('/home/admin-suz/Documents/Code/tyssue/tyssue/stores')
    #dirname = SIM_DIR
    #current = 'small_hexagonal.hf5'


    dsets = hdf5.load_datasets(os.path.join(dirname, current),
                               data_names=['vert', 'edge', 'face'])

    specs = config.geometry.cylindrical_sheet()
    sheet = Sheet('ellipse', dsets, specs)
    SheetGeometry.update_all(sheet)
    list_intersected_facesolve_intersection(sheet)


if __name__ == '__main__':
    main()
