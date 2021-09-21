import numpy as np


class Polygon:
    def __init__(self,
                 img):
        self.img = img # as numpy array
        self.shapes = [] # list of verticies of each shape (polygon)

    def get_all_shapes(self):
        
