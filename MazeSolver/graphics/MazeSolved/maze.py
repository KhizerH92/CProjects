__author__ = 'elladzenitis'
# maze structure

from cs1lib import *

# # constants for radius and stroke width
# VERTEX_RADIUS = 9
STROKE_WIDTH = 3
DISPLAY_WIDTH = 600
DISPLAY_HEIGHT = 600
NODE_SIZE = 50

class Maze:
    def __init__(self, maze_x, maze_y, x, y, up, down, right, left):
        self.maze_x = int(maze_x)
        self.maze_y = int(maze_y)
        self.x = int(x)
        self.y = int(y)
        self.up = up
        self.down = down
        self.right = right
        self.left = left

    # draw an individual vertex
    def draw_mazenode(self, r, g, b):
        node_size_x = DISPLAY_WIDTH/self.maze_x
        node_size_y = DISPLAY_HEIGHT/self.maze_y
        enable_stroke()
        set_stroke_color(r, g, b)
        set_stroke_width(STROKE_WIDTH)
        x = self.x*node_size_x
        y = self.y*node_size_y
        if(self.up):
            draw_line(x, y, x+node_size_x, y)
        if(self.down):
            draw_line(x, y+node_size_y, x+node_size_x, y+node_size_y)
        if(self.left):
            draw_line(x, y, x, y+node_size_y)
        if(self.right):
            draw_line(x+node_size_x, y, x+node_size_x, y+node_size_y)


    def __str__(self):

        return str(self.x) + " " + str(self.y)




