import numpy as np


class Polyarea:
    def __init__(self,
                 x,
                 y):
        self.x = x
        self.y = y
        self.A = self.area()

    def area(self):
        sum = (self.x[-1] * self.y[0] - self.y[-1] * self.x[0]) / 2
        for idx, _ in enumerate(self.x[:-1]):
            sum += (self.x[idx] * self.y[idx + 1] - self.y[idx] * self.x[idx + 1]) / 2
        return sum

    def center(self):
        def count_one_cord(__x, __y):
            sum = 0
            for idx, _ in enumerate(__x[:-1]):
                sum += (__x[idx] + __x[idx + 1]) * (__x[idx] * __y[idx + 1] - __x[idx + 1] * __y[idx])
            return sum / 6 / self.A
        return count_one_cord(self.x, self.y),
               count_one_cord(self.y, self.x)
