# -*- coding: utf-8 -*-
import utils
import graph
import string

g = utils.build_graph()

utils.clear()
ans = "s"

print("Welcome to Network of Thrones")
while ans == "s" or ans == "S":
	print("""
Faça sua escolha:

1) Distância entre 2 personagens.
2) Caminho entre 2 personagens.
3) Encontrar Pontos de Articulação.
4) Encontrar Pontes.

0) Sair""")
	choice = int(input(">>> "))
	utils.clear()

	if choice == 0:
		break
	if choice == 3:
		points = graph.articulation_point(g)
		print("Os pontos de articulação são:")
		print("\n".join(points))
	elif choice == 4:
		bridges = graph.bridges(g)
		print("As pontes são:")
		for key, value in bridges.items():
			if not key: continue
			print(key + " - " + ", ".join(value))
	else:
		c1 = raw_input("Personagem 1 >>> ")
		c2 = raw_input("Personagem 2 >>> ")
		c1 = c1.lower().capitalize()
		c2 = c2.lower().capitalize()

		if choice == 1:
			dist = graph.distance_between(g, c1, c2)
			print("A distância entre [{0}] e [{1}] é {2}".format(c1, c2, dist))
		elif choice == 2:
			path = graph.find_path(g, c1, c2)
			if path != []:
				print(" - ".join(path))
			else:
				print("O caminho entre [{0}] e [{1}] não existe!".format(c1, c2))

	print("\n\nNovamente? (s/n)")
	ans = raw_input(">>> ")
	utils.clear()

print("Bye bye!")
