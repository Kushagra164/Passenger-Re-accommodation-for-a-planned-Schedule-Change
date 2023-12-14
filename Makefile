test:
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/schedule.xlsx"	--output_file_path="./Processing/Intermediates/schedule.csv"
excel-to-csv:
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/inventory.xlsx"	--output_file_path="./Processing/Intermediates/inventory.csv"
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/booking.xlsx"	--output_file_path="./Processing/Intermediates/booking.csv"
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/passenger.xlsx"	--output_file_path="./Processing/Intermediates/passenger.csv"
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/schedule.xlsx"	--output_file_path="./Processing/Intermediates/schedule.csv"

csv-to-graph:
	g++ ./Parser/Drivers/parser.cpp -o ./Processing/Executables/parser.out -std=c++20
	./Processing/Executables/parser.out \
	./Processing/Intermediates/schedule.csv \
	./Processing/Intermediates/inventory.csv \
	./Processing/Intermediates/booking.csv \
	./Processing/Intermediates/passenger.csv \
	./Processing/Intermediates/graph.txt \
	./Processing/Intermediates/cancelledAndDelayedFlights.txt

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

edges-to-txt:
	g++ ./Parser/Drivers/output.cpp -o ./Processing/Executables/output.out -std=c++20
	./Processing/Executables/output.out \
	./Processing/Intermediates/schedule.csv \
	./Processing/Intermediates/inventory.csv \
	./Processing/Intermediates/booking.csv \
	./Processing/Intermediates/passenger.csv \
	./Processing/Intermediates/cancelledAndDelayedFlights.txt \
	./Processing/Intermediates/edges.txt \
	./Processing/Intermediates/solution.txt

txt-to-excel:
	python ./Parser/Drivers/txtToExcel.py \
	--input_file_path ./Processing/Intermediates/solution.txt \
	--output_folder ./Output

clean-Executables:
	rm -rf ./Processing/Executables
	mkdir ./Processing/Executables

clean-Intermediates:
	rm -rf ./Processing/Intermediates
	mkdir ./Processing/Intermediates

clean:
	$(MAKE) clean-Executables
	$(MAKE) clean-Intermediates

install-dep-parser:
	pip install -r ./Parser/requirements.txt

install-dep-qubo:
	pip install -r ./QUBO/requirements.txt

install-dep:
	$(MAKE) install-dep-parser
	$(MAKE) install-dep-qubo

run:
	$(MAKE) excel-to-csv
	$(MAKE) csv-to-graph
	$(MAKE) graph-to-edges
	$(MAKE) edges-to-txt
	$(MAKE) txt-to-excel
