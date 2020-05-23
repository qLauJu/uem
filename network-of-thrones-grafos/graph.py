class Graph:
	def __init__(self):
		self._graph = {}

	def add(self, v, u):
		if v not in self._graph:
			self._graph[v] = set([u])
		else:
			self._graph[v].add(u)

		if u not in self._graph:
			self._graph[u] = set([v])
		else:
			self._graph[u].add(v)

	def vertices(self):
		return self._graph.keys()

	def neighbors(self, v):
		if v in self._graph.keys():
			return self._graph[v]
		return []

	def print_graph(self):
		for v, adj in self._graph.items():
			print(v + " -> " + ", ".join(adj))


def articulation_point(graph):
	visited = {}
	pred = {}
	low = {}
	d = {}
	f = {}
	time = 0
	points = set([])

 	# initialization
	for u in graph.vertices():
		pred[u] = None
		visited[u] = False
		low[u] = float("inf")
		d[u] = float("inf")
		f[u] = float("inf")

	# recursive part
	def articulation_point_visit(u, time):
		time += 1
		visited[u] = True
		d[u] = time
		low[u] = d[u]
		countSons = 0
		for v in graph.neighbors(u):
			if not visited[v]:
				pred[v] = u
				articulation_point_visit(v, time)
				if pred[u] is None:
					countSons += 1
					if countSons == 2:
						points.add(u)
				else:
					low[u] = min(low[u], low[v])
					if low[v] >= d[u]:
						points.add(u)
			else:
				if v != pred[u] and d[v] < d[u]:
					low[u] = min(low[u], d[v])
		time += 1
		f[u] = time

	for u in graph.vertices():
		if not visited[u]:
			articulation_point_visit(u, time)

	return points

def bridges(graph):
	visited = {}
	pred = {}
	low = {}
	d = {}
	f = {}
	time = 0
	bridges_result = {}

 	# initialization
	for u in graph.vertices():
		pred[u] = None
		visited[u] = False
		low[u] = float("inf")
		d[u] = float("inf")
		f[u] = float("inf")

	def bridges_visit(u, time):
		time += 1
		visited[u] = True
		low[u] = d[u] = time
		for v in graph.neighbors(u):
			if not visited[v]:
				pred[v] = u
				bridges_visit(v, time)
				low[u] = min(low[u], low[v])
				if low[v] > d[u]:
					if u not in bridges_result:
						bridges_result[u] = [v]
					else:
						bridges_result[u].append(v)
			else:
				if (v != pred[u]) and (d[v] < d[u]):
					low[u] = min(low[u], d[v])
		time += 1
		f[u] = time

	for u in graph.vertices():
		if not visited[u]:
			bridges_visit(u, time)

	return bridges_result

def bfs(graph, source):
	visited = {}
	dist = {}
	pred = {}

	visited[source] = True
	dist[source] = 0
	pred[source] = None

	queue = [source]
	while queue:
		u = queue.pop(0)
		for v in graph.neighbors(u):
			if v not in visited:
				visited[v] = True
				dist[v] = dist[u] + 1
				pred[v] = u
				queue.append(v)
		visited[u] = True

	return (dist, pred)

def find_path(graph, source, target):
	dist, pred = bfs(graph, source)

	def find(graph, source, target):
		if not source in pred or not target in pred:
			return []
		if source == target:
			return [source]
		else:
			path = find(graph, source, pred[target])
			path.append(target)
			return path
		
	return find(graph, source, target)

def distance_between(graph, source, target):
	dist, pred = bfs(graph, source)
	return dist[target]