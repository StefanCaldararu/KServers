csv: alg.cpp alg.h csv_parser.cpp mspace.cpp randomAlg.cpp greedyAlg.cpp mspace.h randomAlg.h RAII_Classes/getInput.cpp
	g++ -o csv_run -g csv_parser.cpp alg.cpp mspace.cpp randomAlg.cpp  greedyAlg.cpp RAII_Classes/getInput.cpp -lboost_graph

runcsv:
	./csv_run -i sample_input.csv -o outputFile.csv

test: alg.cpp alg.h test.cpp mspace.cpp greedyAlg.cpp randomAlg.cpp mspace.h randomAlg.h
	g++ -o test_run -g test.cpp alg.cpp mspace.cpp greedyAlg.cpp randomAlg.cpp

runtest:
	./test_run