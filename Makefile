graph-to-qubo:
	g++ ./GraphQUBO/graphToQubo.cpp -o graphToQubo.out -std=c++20
	./graphToQubo.out graphMapping.txt quboInput.txt graph.txt

qubo-to-binary:
	python QUBO/QUBOSolver.py --env_file .env

binary-to-edges:
	g++ ./GraphQUBO/quboToGraph.cpp -o quboToGraph.out -std=c++20
	./quboToGraph.out graphMapping.txt quboOutput.txt edges.txt

graph-to-edges:
	$(MAKE) graph-to-qubo
	$(MAKE) qubo-to-binary
	$(MAKE) binary-to-edges 
