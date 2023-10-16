from random import randint

class Sector: # This is a simple sector that represents a room in the game
    #             North  South  East   West
    enterences = [False, False, False, False] # Stores which entrances are open

    def __init__(self, north : bool, south : bool, east : bool, west : bool): # Simple initialiser
        self.enterences = [north, south, east, west]

    def __repr__(self) -> str: # Returns a string representation of the room, using 'X' to represent walls
        printStr = ""
        if self.enterences[0]:
            printStr += "X X\n"
        else:
            printStr += "XXX\n"
        if self.enterences[3]:
            printStr += "  "
        else:
            printStr += "X "
        if self.enterences[2]:
            printStr += " \n"
        else:
            printStr += "X\n"
        if self.enterences[1]:
            printStr += "X X\n"
        else:
            printStr += "XXX\n"
        return printStr

    # These functions just switch the each entrance from being open to close and close to open
    def switchNorth(self):
        self.enterences[0] = not self.enterences[0]

    def switchSouth(self):
        self.enterences[1] = not self.enterences[1]

    def switchEast(self):
        self.enterences[2] = not self.enterences[2]

    def switchWest(self):
        self.enterences[3] = not self.enterences[3]

    def __eq__(self, o) -> bool:
        if not isinstance(o, Sector):
            return False
        return True

    def __ne__(self, o) -> bool:
        if not isinstance(o, Sector):
            return False
        return not self == o

def printHugeString(theHugeString : str): #   As the program produces a string that is normally too long to print
    lines = theHugeString.split("\n") # This print function splits it up into chunks that can be printed
    for i in range(0, len(lines), 10):
        print("\n".join(lines[i: i + 10]))

def printBoard(board : list): # This function prints the board into the console
    printList = ["-" * (len(board) * 3)]
    printList += ["" for _ in range(len(board) * 3)]
    printList += ["-" * (len(board) * 3)]
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j] != None:
                printSector = str(board[i][j]).split("\n")
            else:
                printSector = ["XXX", "XXX", "XXX"]
            for x in range(3):
                printList[i * 3 + x + 1] += printSector[x]
    printHugeString("\n".join(["|" + x + "|" for x in printList]))

BOARDSIZE = 11 # Stores what the width and height are for the board

def generatePaths(board : list, current : list, layerMax : int) -> tuple: # This function continually generates paths from a given input of starting positions that could be rooms
    layer = 0                                # This returns all the entrances that are open on each sides
    currentNorth = []
    currentSouth = []
    currentEast = []
    currentWest = []
    while len(current) > 0: # Will continue to generate rooms until there are no open entrances left that are not on the edges
        newCurrent = []
        for pos in current:
            north = False
            if board[pos[0]][pos[1]] != None: # Checks if there is a room already in that spot
                continue

            # Goes through each possible entrance and sees if it has to be open (Because a room next to it has the entrance open)
            # Or it randomises a possibility that it should be open
            pathCount = 0
            if pos[0] > 0 and board[pos[0] - 1][pos[1]] != None:
                if board[pos[0] - 1][pos[1]].enterences[1] == True:
                    north = True
                pathCount += 1
            else:
                r = randint(0, 2)
                if r == 0:
                    north = True
                    pathCount += 1

            south = False
            if pos[0] < BOARDSIZE - 1 and board[pos[0] + 1][pos[1]] != None:
                if board[pos[0] + 1][pos[1]].enterences[0] == True:
                    south = True
                pathCount += 1
            else:
                r = randint(0, 2)
                if r == 0:
                    south = True
                    pathCount += 1

            east = False
            if pos[1] < BOARDSIZE - 1 and board[pos[0]][pos[1] + 1] != None:
                if board[pos[0]][pos[1] + 1].enterences[3] == True:
                    east = True
                pathCount += 1
            else:
                r = randint(0, 2)
                if r == 0:
                    east = True
                    pathCount += 1

            west = False
            if pos[1] > 0 and board[pos[0]][pos[1] - 1] != None:
                if board[pos[0]][pos[1] - 1].enterences[2] == True:
                    west = True
                pathCount += 1
            else:
                r = randint(0, 2)
                if r == 0:
                    west = True
                    pathCount += 1

            # This checks to see if the room has enough entrances open to produce a big enough maze
            # If not it will randomise a few more entrances to be openned
            if pathCount == 1 and layer < layerMax:
                options = [north, south, east, west]
                r = randint(0, 2)
                c = 0
                for i in range(4):
                    if options[i]:
                        continue
                    if c == r:
                        options[i] = True
                        break
                    c += 1
                r = randint(0, 2)
                if r != 2:
                    r = randint(0, 2)
                    c = 0
                    for i in range(4):
                        if options[i]:
                            continue
                        if c == r:
                            options[i] = True
                            break
                        c += 1
                north = options[0]
                south = options[1]
                east = options[2]
                west = options[3]

            # This does another check for rooms that are even closer to the centre, so there is less possibility of a maze that is extremely small being generated
            if pathCount == 2 and layer < layerMax - int(BOARDSIZE / 3):
                r = randint(0, 2)
                if r != 2:
                    options = [north, south, east, west]
                    r = randint(0, 2)
                    c = 0
                    for i in range(4):
                        if options[i]:
                            continue
                        if c == r:
                            options[i] = True
                            break
                        c += 1
                    north = options[0]
                    south = options[1]
                    east = options[2]
                    west = options[3]

            # This appends any new entrance made, without a room next to it to the next list of rooms to be generated
            if north and pos[0] > 0 and board[pos[0] - 1][pos[1]] == None:
                newCurrent.append((pos[0] - 1, pos[1]))

            if south and pos[0] < BOARDSIZE - 1 and board[pos[0] + 1][pos[1]] == None:
                newCurrent.append((pos[0] + 1, pos[1]))

            if east and pos[1] < BOARDSIZE - 1 and board[pos[0]][pos[1] + 1] == None:
                newCurrent.append((pos[0], pos[1] + 1))

            if west and pos[1] > 0 and board[pos[0]][pos[1] - 1] == None:
                newCurrent.append((pos[0], pos[1] - 1))

            # Creates the room and puts it onto the board
            board[pos[0]][pos[1]] = Sector(north, south, east, west)

        current = [x for x in newCurrent]

    # This finds all the entrances on the edge of the board
    for i in range(BOARDSIZE):
        if board[0][i] != None and board[0][i].enterences[0]:
            currentNorth.append(i)
        if board[-1][i] != None and board[-1][i].enterences[1]:
            currentSouth.append(i)
        if board[i][-1] != None and board[i][-1].enterences[2]:
            currentEast.append(i)
        if board[i][0] != None and board[i][0].enterences[3]:
            currentWest.append(i)

    return board, currentNorth, currentSouth, currentEast, currentWest


def generateBoard() -> tuple: # This returns a board that has generated a maze, as well as all the entrances on the side
    board = [[None for _ in range(BOARDSIZE)] for _ in range(BOARDSIZE)]
    midPoint = BOARDSIZE // 2 + 1
    board[midPoint][midPoint] = Sector(True, True, True, True)
    current = [(midPoint - 1, midPoint), (midPoint, midPoint - 1), (midPoint + 1, midPoint), (midPoint, midPoint + 1)]
    return generatePaths(board, current, int(BOARDSIZE * 2 / 3))

# These next functions are for moving the board, by deleting one row and adding another row and then calling the generation function
def moveNorth(board : list, current : list) -> tuple:
    newCurrent = [(0, x) for x in current]
    del board[-1]
    board.insert(0, [None for _ in range(BOARDSIZE)])
    return generatePaths(board, newCurrent, 5)

def moveSouth(board : list, current : list) -> tuple:
    newCurrent = [(BOARDSIZE - 1, x) for x in current]
    del board[0]
    board.append([None for _ in range(BOARDSIZE)])
    return generatePaths(board, newCurrent, 5)

def moveEast(board : list, current : list) -> tuple:
    newCurrent = [(x, BOARDSIZE - 1) for x in current]
    for i in range(BOARDSIZE):
        del board[i][0]
        board[i].append(None)

    return generatePaths(board, newCurrent, 2)

def moveWest(board : list, current : list) -> tuple:
    newCurrent = [(x, 0) for x in current]
    for i in range(BOARDSIZE):
        del board[i][-1]
        board[i].insert(0, None)

    return generatePaths(board, newCurrent, 5)

currentNorth = []
currentSouth = []
currentEast = []
currentWest = []

# This generates the board and allows the user to move in the different directions
board, currentNorth, currentSouth, currentEast, currentWest = generateBoard()
printBoard(board)
while True:
    inp = input("Direction: ").lower()
    if inp == "u":
        board, currentNorth, currentSouth, currentEast, currentWest = moveNorth(board, currentNorth)
    elif inp == "d":
        board, currentNorth, currentSouth, currentEast, currentWest = moveSouth(board, currentSouth)
    elif inp == "r":
        board, currentNorth, currentSouth, currentEast, currentWest = moveEast(board, currentEast)
    elif inp == "l":
        board, currentNorth, currentSouth, currentEast, currentWest = moveWest(board, currentWest)
    printBoard(board)