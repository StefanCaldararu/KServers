csv: csv_parser.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp ALGS/KCentersAlg.cpp
	g++ -std=c++11 -o csv_run -g csv_parser.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp ALGS/KCentersAlg.cpp

runcsv:
	./csv_run -i sample_input.csv -o outputFile.csv

test: csv_parser.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp
	g++ -std=c++11 -o test_run -g test.cpp ALGS/alg.cpp mspace.cpp ALGS/greedyAlg.cpp ALGS/randomAlg.cpp

runtest:
	./test_run

csv_p1: csv_parser.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp ALGS/KCentersAlg.cpp
	g++ -std=c++11 -o csv_run -O3 csv_parser_parallel.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp ALGS/KCentersAlg.cpp -fopenmp

csv_p2: csv_parser.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp ALGS/KCentersAlg.cpp RAII_Classes/buffer.cpp
	g++ -std=c++11 -o csv_run -O3 csv_parser_parallel2.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp ALGS/KCentersAlg.cpp RAII_Classes/buffer.cpp
