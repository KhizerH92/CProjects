__author__ = 'elladzenitis'

from maze import Maze

# initializes vertex dictionary
maze_nodes = []

# function to create maze from a file
def load_maze(maze_file_name):

    maze_file = open(maze_file_name, "r")

    i = 1
    for line in maze_file:
        print i
        if (i == 1):
            dimensions = line.strip().split(";")
            x_size = dimensions[0]
            y_size = dimensions[1]

        else:
            maze_info = line.split(";")

            # first part is the x and y positions in the 2D array
            coordinates = maze_info[0].split(",")
            mazenode_x = coordinates[0]
            mazenode_y = coordinates[1]

            walls = maze_info[1].strip().split(",")
            mazenode_up = (int(walls[0]) == -1)
            mazenode_down = (int(walls[1]) == -1)
            mazenode_right = (int(walls[2]) == -1)
            mazenode_left = (int(walls[3]) == -1)

            new_mazenode = Maze(x_size, y_size, mazenode_x, mazenode_y, mazenode_up, mazenode_down, mazenode_left, mazenode_right)

            maze_nodes.append(new_mazenode)

        i+=1

    maze_file.close()

    return maze_nodes


def load_avatars(avatar_file_name, node_size_x, node_size_y):
    avatar_info = []
    avatar_file = open(avatar_file_name, "r")
    i = 1
    j = 1
    for line in avatar_file:
        # print line
        if (i == 1):
            avatars = line.strip()
            num_avatars = int(avatars)
            # you will have a 2D array --> [avatar index][coordinates]
            for y in range(num_avatars):
                avatar_array = []
                avatar_info.append(avatar_array)
            # now have created proper 2D array
            i+=1

        # read file line by line
        else:
            line_split = line.split(":")
            coordinates = line_split[1].split(",")
            x = int(coordinates[0].strip())
            y = int(coordinates[1].strip())

            # add to array in first node
            avatar_info[j % num_avatars].append(x)
            avatar_info[j % num_avatars].append(y)

            j+=1


    print avatar_info

    return avatar_info



