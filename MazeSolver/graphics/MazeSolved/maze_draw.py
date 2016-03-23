__author__ = 'elladzenitis'
# map_plot_Dzenitis.py
# draws a map of campus, draws points for locations, and displays paths between locations
# Ella Dzenitis, CS1 2/23/15

from cs1lib import *
from load_maze import *

# constants for the map width and height, and the sleeptime
DISPLAY_WIDTH = 600
DISPLAY_HEIGHT = 600
SLEEP_TIME = 0.05

# draw the maze
def draw_maze():

    # load the graph
    maze_nodes = load_maze("mazetest.dat")

    node_size_x = DISPLAY_WIDTH/maze_nodes[0].maze_x
    node_size_y = DISPLAY_HEIGHT/maze_nodes[0].maze_y

    avatar_data = load_avatars("avatar_log.txt", node_size_x, node_size_y)

    avatar_radius = node_size_x/2 - 1

    enable_smoothing()

    i = 0
    while not window_closed():

        clear()

        # draw all nodes
        for node in maze_nodes:
            # print str(node)
            node.draw_mazenode(0, 0, 1)

        # each time, draw a new set of coordinates
        for avatar in avatar_data:
            print i
            print "moving to"
            print str(avatar[i]) + str(avatar[i+1])
            disable_stroke()
            set_fill_color(1, 0, 1)
            draw_circle(avatar[i]*node_size_x+(node_size_x/2), avatar[i+1]*node_size_y + (node_size_y/2), avatar_radius)
            i+=2

        request_redraw()
        sleep(SLEEP_TIME)

# display graphics
start_graphics(draw_maze, "Amazing Maze", DISPLAY_WIDTH, DISPLAY_HEIGHT)





