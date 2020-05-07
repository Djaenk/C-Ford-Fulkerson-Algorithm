import random

for i in range(5, 8):
	#each iteration produces a graph increasing linearly in number of vertices
	vertex_count = i

	filename = f'graph{i - 4}'
	with open(filename + '.txt', "w+") as file:
		#write graph name
		file.write(filename)

		#write column vertex labels
		for vertex in range(vertex_count):
			file.write(f',{vertex}')
		file.write('\n')

		#write rows
		for vertex in range(vertex_count):
			#write row vertex labels
			file.write(f'{vertex}')

			#write directed edge weights
			for edge in range(vertex_count):
				weight = 0

				#linear edge density of approximately 2
				if (random.uniform(0, 1) < 2.5/vertex_count):
					#weight randomized between 0 and one less than the number of vertices
					weight = random.randint(1, 2)
				
				file.write(f',{weight}')

			file.write('\n')
