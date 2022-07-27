import random

# graph is a list of lists that contains their edges
# triangle = [[1,2],[0,2],[0,1]]
# 1st step: generate random graph on a plane

def RandomGraph(howmany, edge_radius, random_seed=0):

    # return a random graph, with connectivity if in "radius" range
    random.seed(random_seed)

    points = []

    for i in range(0,howmany):
        points.append((random.random(),random.random()))

    graph = []

    for i in range(0,howmany):
        graph.append(filter((lambda x:(x!=i) and (points[x][0]-points[i][0])*(points[x][0]-points[i][0]) + (points[x][1]-points[i][1])*(points[x][1]-points[i][1])<edge_radius*edge_radius), range(0,howmany)))

    return graph

def CFL(graph, max_color, beta = 0.5, max_cycle = 10000):
    #input: graph. output: if coloring with max_color colors is possible, the coloring; else [] 
    #coloring is a list of color number from 0 to max_color-1
    
    #first, initialize the distribution to be uniform distribution
    coloring = []
    coloring_distribution = []
    for i in range(0,len(graph)):
        coloring_distribution.append(max_color*[1.0/max_color])
    
    cycle=0

    while(cycle<=max_cycle):
        cycle+=1

        color_successful = True

        coloring = [0]*len(graph)

        # first, randomly pickup a color from the distribution    
        for i in range(0,len(graph)):
                    
            rand = random.random()
            
            coloring[i]=0
            picker = coloring_distribution[i][coloring[i]]
            
            while (picker<rand):
                coloring[i]+=1
                picker += coloring_distribution[i][coloring[i]]



        # second, calculate the next step distribution
        # examine whether there are same color adjacent node

        for i in range(0,len(graph)):
            have_samecolor_adj = False

            adjacents = graph[i]
            for adj in adjacents:
                if (coloring[adj] == coloring[i]):
                    have_samecolor_adj = True
            
            if(not have_samecolor_adj):
                # fix the color here
                coloring_distribution[i] = max_color*[0.0]
                coloring_distribution[i][coloring[i]] = 1.0
            else:
                color_successful = False
                new_coloring_distribution = map(lambda x:((1-beta)*x), coloring_distribution[i])
                anti_delta = max_color*[1.0]
                anti_delta[coloring[i]] = 0.0
                anti_delta = map(lambda x:beta*x/(max_color-1), anti_delta)

                for j in range(0, len(new_coloring_distribution)):
                    new_coloring_distribution[j]+=anti_delta[j]
                coloring_distribution[i] = new_coloring_distribution

        if(color_successful):
            return coloring, cycle
            

    return [],cycle


def main():
    # grid search on howmany and interference_radius
    f = open('result.txt', 'w')

    COUNT_MAX = 6*3*2*20
    count = 0

    for node_count in range(10,40,5):
        for interference_radius in [0.1,0.2,0.4]:
            for max_color in [5,10]:
                for REPEAT in range(0,20):

                    count+=1

                    if(count*100 % COUNT_MAX ==0):
                        print "\b\b\b\b\b\b\b\b\b" + str(count*100 / COUNT_MAX) + "%"
                    

                    graph = RandomGraph(node_count, interference_radius)
                    result = CFL(graph, max_color)
                    is_solved = (len(result[0])!=0)
                    f.write(repr((node_count,interference_radius,max_color,is_solved,result[1])))
                    f.write("\n")
    f.close()

if __name__=='__main__':
    main()
