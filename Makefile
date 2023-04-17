csv: alg.cpp alg.h csv_parser.cpp mspace.cpp randomAlg.cpp greedyAlg.cpp mspace.h randomAlg.h RAII_Classes/getInput.cpp
	g++ -std=c++11 -o csv_run -g csv_parser.cpp alg.cpp mspace.cpp randomAlg.cpp optAlg.cpp greedyAlg.cpp WFAlg.cpp doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp

runcsv:
	./csv_run -i sample_input.csv -o outputFile.csv

test: alg.cpp alg.h test.cpp mspace.cpp greedyAlg.cpp randomAlg.cpp mspace.h randomAlg.h
	g++ -std=c++11 -o test_run -g test.cpp alg.cpp mspace.cpp greedyAlg.cpp randomAlg.cpp

runtest:
	./test_run