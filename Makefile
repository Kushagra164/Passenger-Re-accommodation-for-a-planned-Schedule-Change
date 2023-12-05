graph-to-qubo:
	g++ ./GraphQUBO/graphToQubo.cpp -o graphToQubo.out -std=c++20
	./graphToQubo.out graphMapping.txt quboInput.txt graphUVW.txt

qubo-to-graph:
	python QUBO/QUBOSolver.py

graph-to-edges:
	g++ ./GraphQUBO/quboToGraph.cpp -o quboToGraph.out -std=c++20
	./quboToGraph.out graphMapping.txt quboOutput.txt edges.txt