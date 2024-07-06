order = 2
levels = int(input("levels: "))
d = order+1
no_of_vertices = 1
res = 0
for i in range(0,levels):
    res += (d-1)**i
no_of_vertices += d*res

#vertices available on each level
vertices_at_each_level = [0]*levels
vertices_at_each_level[0] = 1;vertices_at_each_level[1] = 3
for i in range(2, len(vertices_at_each_level)+1):
    vertices_at_each_level.append(2*vertices_at_each_level[i-1])

class nodeC:
    def __init__(self,data):
        self.val = data
        self.prev = None
        self.next = None

node_data = [int(i) for i in input().split()]

nodes = []
temp = []
idx = 0
for i in vertices_at_each_level:
    for _ in range(1,i+1):
        temp.append(nodeC(node_data[idx]))
        idx += 1
    nodes.append(temp)
    temp = []

