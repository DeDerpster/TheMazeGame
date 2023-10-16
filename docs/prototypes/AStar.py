from colorama import Fore
from math import sqrt

# This is the for storing nodes, NOTE: they do not store their own coordinates as they will be stored on a node map so they don't have to store them
class Node:
    def __init__(self, parent : tuple, gCost : float, hCost : float):
        self.parent = parent
        self.gCost = gCost
        self.hCost = hCost
        self.fCost = gCost + hCost

#This is just for turning the string board at the bottom into the board that is used throughout the algorithm (A 2d boolean array)
def getBoard(stringBoard : str) -> list:
    board = []
    for line in stringBoard.split("\n"): # Goes through each line of the board
        if len(line) != 19:
            continue # Will ignore the line if it does not have the regular amount of characters
        board.append([x == "X" for x in line]) # Produces a boolean array and adds it to the board
    return board

# This is for returning if there is a colision in at a point
def isSolid(board : list, pos : tuple) -> bool:
    return board[pos[1]][pos[0]]

# This is for printing the board and the path
def printBoard(board : list, path : list):
    for y in range(0, len(board)):
        line = ""
        for x in range(0, len(board[0])):
            if (x, y) in path: # If the position is part of the path it prints an 'O'
                line += Fore.GREEN + "O" + Fore.RESET
            elif board[y][x]: # If it is a solid wall it prints an 'X'
                line += "X"
            else:
                line += " " # Otherwise prints a space
        print(line)

# This uses pythagorous' theorem to get the distance between two coordinates
def getDistance(start : tuple, end : tuple) -> float:
    return sqrt((start[0] - end[0]) ** 2 + (start[1] - end[1]) ** 2)

# This is the A* algorithm that takes in the board, start position and destination and returns a list of positions to follow to get to the destination
def getPath(board : list, startPos : tuple, destPos : tuple) -> list:
    openList = [startPos] # This is a list that stores the positions that is needs to close and open the neighbours
    closedList = [[False for _ in range(len(board[0]))] for _ in range(len(board))] # This a 2d boolean array that directly links to the nodeMap and stores whether each node has been visited or not
    nodeMap = [[None for _ in range(len(board[0]))] for _ in range(len(board))] # This stores all the nodes in a 2d array that relate to the position on the board
    nodeMap[startPos[1]][startPos[0]] = Node((-1, -1), 0.0, getDistance(startPos, destPos)) # Creates the first node

    # A while loop that will keep running until the destination is found
    while openList[0] != destPos:
        pos = openList[0]

         # Removes the position from the list and closes it
        del openList[0]
        currentNode = nodeMap[pos[1]][pos[0]]
        closedList[pos[1]][pos[0]] = True

        # Goes around the positon looking where it can go
        for offset in [(-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1), (-1, 0)]:
            nextPos = (pos[0] + offset[0], pos[1] + offset[1])
            # Checks if it is out of bounds, closed or is solid, if it is its skips
            if nextPos[0] < 0 or nextPos[0] >= len(board[0]) or nextPos[1] < 0 or nextPos[1] >= len(board) or closedList[pos[1]][pos[0]] or isSolid(board, nextPos):
                continue

            # Calculates the gCost and hCost of the nextNode
            gCost = currentNode.gCost + getDistance(nextPos, pos)
            hCost = getDistance(nextPos, destPos)

            # Checks if is open
            if(nodeMap[nextPos[1]][nextPos[0]] is not):
                if nodeMap[nextPos[1]][nextPos[0]].fCost < gCost + hCost: # If it has a higher fCost it continues
                    continue
                # Deletes the position from the open list (so it can be later added in the correct position)
                openList.remove(nextPos)

            # Creates the next node
            nodeMap[nextPos[1]][nextPos[0]] = Node(pos, gCost, hCost)

            # Gets the index based on its fCost (as the list is sorted based on the fCost)
            index = 0
            for each in openList:
                if nodeMap[each[1]][each[0]].fCost >  nodeMap[nextPos[1]][nextPos[0]].fCost:
                    break
                index += 1

            # Inserts the position into the open list
            openList.insert(index, nextPos)

    # Gets the list of paths by going throuhgh the parents of the nodes
    path = []
    pos = openList[0]
    while pos != (-1, -1):
        path.append(pos)
        pos = nodeMap[pos[1]][pos[0]].parent

    # Returns the inverted list (as the destination is at index 0 at the start)
    return path[::-1]



stringBoard = """
XXXXXXXXXXXXXXXXXXX
X X               X
X X XXXXXXXXXX XXXX
X XXX  X X     X  X
X      X X  X XXX X
X XXXXXX XXXX     X
X X           XXXXX
X   XXXXX  XXXX   X
X X X   XX X    X X
X XXXXX XX X XXXX X
X X   X    X X    X
X XXX XXXXXXXX XXXX
X                 X
XXXXXXXXXXXXXXXXXXX
"""

board = getBoard(stringBoard)

printBoard(board, getPath(board, (1, 1), (12, 10)))