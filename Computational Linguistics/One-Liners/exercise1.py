#one liners

#triple
def triple(s):
    return ''.join([3*char for char in s])


#cleaning right
def cleaning_right(dirty):
    return cleaning_right([dirty[i][:-1] if dirty[i].endswith(('c','o','f','e')) else dirty[i] for i in range(len(dirty))]) if any(dirty[t].endswith (('c','o','f','e')) for t in range(len(dirty))) else dirty
