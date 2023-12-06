graph-to-qubo:
	g++ ./GraphQUBO/graphToQubo.cpp -o graphToQubo.out -std=c++20
	./graphToQubo.out graphMapping.txt quboInput.txt graph.txt

qubo-to-graph:
	python QUBO/QUBOSolver.py --api_key "DEV-e1a717bce213b6c8f2d454b9d2d1347bcb20ced9" --input_file "quboInput.txt" --output_file "quboOutput.txt"  --no_samples "2"

binary-to-edges:
	g++ ./GraphQUBO/quboToGraph.cpp -o quboToGraph.out -std=c++20
	./quboToGraph.out graphMapping.txt quboOutput.txt edges.txt

graph-to-edges:
	$(MAKE) graph-to-qubo
	$(MAKE) qubo-to-binary
	$(MAKE) binary-to-edges 
