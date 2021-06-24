import graphviz
import sys
# By Tianxu He and Yikang Wang
# use this in commond line:
# python3 drawCircuit.py inputFileName outputFileName
# for example:
# python3 drawCircuit.py vector.txt myOutput.png

# you must have inputFileName
# outputFileName default as 'output.png'


class CircuitGraph:
    def __init__(self):
        # start with a graphviz object
        # Create the graph. New nodes are created automatically
        # when first named.
        self.graph = graphviz.Digraph()
        self.graph.attr(rankdir='LR')
        self.graph.attr('node', shape='rectangle')
        self.graph.attr(dpi='300')

    def connect(self, nodeA, nodeB, mod='concentrate'):
        '''
        Wrapper of edge method for graph object in graphviz
        '''
        # how we connect two circuits
        tail = 'n' if mod == 'concentrate' else 's'
        col = 'blue' if mod == 'concentrate' else 'red'
        self.graph.edge(nodeA, nodeB, color=col, headport='w', tailport=tail,
                        arrowtail='normal')

    def connectFromVector(self, li):
        '''
        convert the units vector to the vector of CUnits.
        Parameter:
            li: list of units.
        '''

        # the first edge: connect the 'Feed' to the first node.
        self.graph.edge('Feed', str(li[0]), color='blue', headport='w',
                        tailport='e', arrowhead='normal', arrowtail='normal')

        # find the number of concentrate and tailing
        len_nodes = (len(li)-1)//2+2
        final_tail = len_nodes-1
        final_con = len_nodes-2

        # max and second max elements in li
        max_li = max(li)
        max2_li = sorted(list(set(li)))[-2]

        # check if we have con and tail in vector:
        if not (max_li == final_tail and max2_li == final_con):
            raise ValueError("Input vector do not contain tailings or \
concentrate")

        # check whether all units have inputs.
        if set(li[1:]) != set(range(len_nodes)):
            raise ValueError("Not all units have at least one input")

        # connect the circuits
        for i in range(final_con):
            i_li = i*2+1
            # check whether each element in vector is valid
            if not(0 <= li[i_li] < len_nodes):
                raise ValueError("Invalid unit number {} out of range [{}, \
{}]".format(li[i_li], 0, final_tail))

            # check whether each element has self-cycle.
            if i == li[i_li] or i == li[i_li+1]:
                raise ValueError("Unit {} has self-cycle".format(i))

            # check wheter two outputs are same for each unit.
            if li[i_li] == li[i_li+1]:
                raise ValueError("Unit {} has two same outputs".format(i))

            # connect units after checking
            if li[i_li] == final_con:
                self.connect(str(i), 'Concentrate', mod='concentrate')
            elif li[i_li] == final_tail:
                self.connect(str(i), 'Tailings', mod='concentrate')
            else:
                self.connect(str(i), str(li[i_li]), mod='concentrate')

            if li[i_li+1] == final_tail:
                self.connect(str(i), 'Tailings', mod='tailings')
            elif li[i_li+1] == final_con:
                self.connect(str(i), 'Concentrate', mod='tailings')
            else:
                self.connect(str(i), str(li[i_li+1]), mod='tailings')

        # make Concentrate and Tailinngs at the end of picture.
        with self.graph.subgraph(name='cluster_0') as c:
            c.attr(color='white')
            c.node('Concentrate', color='blue')
            c.node('Tailings', color='red')

    def fromText(self, path):
        '''
        read vector from text file, and this will automatically
        run the self.connectFromVector after loading data from file.
        '''
        with open(path) as f:
            tmp = [int(i) for i in f.readline().strip().split()]
            if not tmp:
                print('No data from the file {}'.format(path))
                return
            self.connectFromVector(tmp)

    def render2File(self, path):
        '''
        render the graph to png file,
        if path ends with .png already, we remove it first.
        '''
        if path.endswith('.png'):
            path = path[:-4]
        self.graph.render(filename=path, cleanup=True, format='png')
        print('Have Rendered to {}'.format(path+'.png'))

# cg = CircuitGraph()
# cg.fromText('vector.txt')
# cg.connectFromVector([0,4,3,2,0,5,4,4,6,2,1])
# cg.connectFromVector([0,1,3,8,0,6,9,1,0,8,1,8,4,9,4,10,4,7,4,2,5])
# cg.connectFromVector([6,16,7,7,13,11,12,15,5,3,6,0,2,14,12,1,12,14,11,5,16,11,9,4,1,0,8,5,10,2,6])
# cg.connectFromVector([0,4,6,5,12,4,11,7,4,5,14,7,8,4,2,15,5,7,3,16,11,5,1,12,13,4,0,12,9,5,10])
# cg.render2File('output')


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1]:
        inputName = sys.argv[1]
    else:
        print("Pleaze input the vector text file:")
        inputName = input()

    if len(sys.argv) > 2 and sys.argv[2]:
        outputName = sys.argv[2]
    else:
        outputName = 'output'
    cg = CircuitGraph()
    cg.fromText(inputName)
    cg.render2File(outputName)
