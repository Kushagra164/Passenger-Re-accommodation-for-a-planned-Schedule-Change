graph-to-qubo:
	g++ ./GraphQUBO/graphToQubo.cpp -o ./Processing/Executables/graphToQubo.out -std=c++20
	./Processing/Executables/graphToQubo.out \
	./Processing/Intermediates/graphMapping.txt \
	./Processing/Intermediates/quboInput.txt \
	./Processing/Intermediates/graph.txt

qubo-to-binary:
	python QUBO/QUBOSolver.py --env_file .env \
	--input_file_path ./Processing/Intermediates/quboInput.txt \
	--output_file_path ./Processing/Intermediates/quboOutput.txt

binary-to-edges:
	g++ ./GraphQUBO/binaryToEdges.cpp -o ./Processing/Executables/binaryToEdges.out -std=c++20
	./Processing/Executables/binaryToEdges.out \
	./Processing/Intermediates/graphMapping.txt \
	./Processing/Intermediates/quboOutput.txt \
	./Processing/Intermediates/edges.txt

graph-to-edges:
	$(MAKE) graph-to-qubo
	$(MAKE) qubo-to-binary
	$(MAKE) binary-to-edges 

clean-Executables:
	rm -rf ./Processing/Executables
	mkdir ./Processing/Executables

clean-Intermediates:
	rm -rf ./Processing/Intermediates
	mkdir ./Processing/Intermediates

clean:
	$(MAKE) clean-Executables
	$(MAKE) clean-Intermediates
