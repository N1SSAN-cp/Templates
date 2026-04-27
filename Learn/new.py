vertices=['A','B','C','D']
edges =[('A','B'),('C','D'),('A','C')]
n = len(vertices)
matrix =[[0]*n for _ in range(n)]
for u,v in edges:
    i=vertices.index(u)
    j=vertices.index(v)
    matrix[i][j]=matrix[j][i]=1

print("Adjency Matrix: ")
for row in matrix:
    print(row)
