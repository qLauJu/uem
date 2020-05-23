import csv
import graph

def build_graph():
	g = graph.Graph()
	with open('stormofswords.csv', 'rU') as file:
		reader = csv.reader(file)
		for row in reader:
			v, u = row[0], row[1]
			g.add(v, u)
	return g

def clear():
	print("\n" * 100) 