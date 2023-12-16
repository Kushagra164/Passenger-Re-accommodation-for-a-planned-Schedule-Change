convert-schedule:
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/schedule.xlsx"	\
		--output_file_path="./Processing/Intermediates/schedule.csv" \
		> ./Processing/Logs/excelToCsvSchedule.txt

convert-inventory:
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/inventory.xlsx"	\
		--output_file_path="./Processing/Intermediates/inventory.csv" \
		> ./Processing/Logs/excelToCsvInventory.txt

convert-booking:
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/booking.xlsx"	\
		--output_file_path="./Processing/Intermediates/booking.csv" \
		> ./Processing/Logs/excelToCsvBooking.txt

convert-passenger:
	python Parser/Drivers/excelToCsv.py	--input_file_path="./Input/booking.xlsx"	\
		--output_file_path="./Processing/Intermediates/booking.csv" \
		> ./Processing/Logs/excelToCsvPassenger.txt

excel-to-csv:
	$(MAKE) convert-schedule
	$(MAKE) convert-inventory
	$(MAKE) convert-booking
	$(MAKE) convert-passenger

csv-to-graph:
	g++ ./Parser/Drivers/parser.cpp -o ./Processing/Executables/parser.out -std=c++20
	./Processing/Executables/parser.out \
	./Processing/Intermediates/schedule.csv \
	./Processing/Intermediates/inventory.csv \
	./Processing/Intermediates/booking.csv \
	./Processing/Intermediates/passenger.csv \
	./Processing/Intermediates/graph.txt \
	./Processing/Intermediates/cancelledAndDelayedFlights.txt > ./Processing/Logs/parser.txt

graph-to-qubo:
	g++ ./GraphQUBO/graphToQubo.cpp -o ./Processing/Executables/graphToQubo.out -std=c++20
	./Processing/Executables/graphToQubo.out \
	./Processing/Intermediates/graphMapping.txt \
	./Processing/Intermediates/quboInput.txt \
	./Processing/Intermediates/graph.txt > ./Processing/Logs/graphToQubo.txt

qubo-to-binary:
	python QUBO/QUBOSolver.py \
	--input_file_path ./Processing/Intermediates/quboInput.txt \
	--output_file_path ./Processing/Intermediates/quboOutput.txt \
	> ./Processing/Logs/quboToBinary.txt

binary-to-edges:
	g++ ./GraphQUBO/binaryToEdges.cpp -o ./Processing/Executables/binaryToEdges.out -std=c++20
	./Processing/Executables/binaryToEdges.out \
	./Processing/Intermediates/graphMapping.txt \
	./Processing/Intermediates/quboOutput.txt \
	./Processing/Intermediates/graph.txt \
	./Processing/Intermediates/edges.txt > ./Processing/Logs/binaryToEdges.txt

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
	./Processing/Intermediates/solution.txt > ./Processing/Logs/edgesToTxt.txt

txt-to-excel:
	python ./Parser/Drivers/txtToExcel.py \
	--input_file_path ./Processing/Intermediates/solution.txt \
	--tmp_folder ./Processing/Intermediates \
	--output_folder ./Output > ./Processing/Logs/txtToExcel.txt

clean-Executables:
	rm -rf ./Processing/Executables
	mkdir ./Processing/Executables

clean-Intermediates:
	rm -rf ./Processing/Intermediates
	mkdir ./Processing/Intermediates

clean-Logs:
	rm -rf ./Processing/Logs
	mkdir ./Processing/Logs

clean:
	$(MAKE) clean-Executables
	$(MAKE) clean-Intermediates
	$(MAKE) clean-Logs

install-dep-parser:
	pip install -r ./Parser/requirements.txt

install-dep-qubo:
	pip install -r ./QUBO/requirements.txt

install:
	$(MAKE) install-dep-parser
	$(MAKE) install-dep-qubo

run:
	$(MAKE) excel-to-csv
	$(MAKE) csv-to-graph
	$(MAKE) graph-to-edges
	$(MAKE) edges-to-txt
	$(MAKE) txt-to-excel
