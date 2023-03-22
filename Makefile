csv: alg.cpp alg.h csv_parser.cpp mspace.cpp randomAlg.cpp mspace.h randomAlg.h
	g++ -o csv_run -g csv_parser.cpp alg.cpp mspace.cpp randomAlg.cpp

runcsv:
	./csv_run -i inputFile.csv -o outputFile.csv -a 1

test: alg.cpp alg.h test.cpp mspace.cpp randomAlg.cpp mspace.h randomAlg.h
	g++ -o test_run -g test.cpp alg.cpp mspace.cpp randomAlg.cpp

runtest:
	./test_run