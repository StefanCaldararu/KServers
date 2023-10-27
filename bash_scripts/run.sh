#!/bin/bash
cd ../
mkdir output
cd ./cpp
g++ -std=c++11 -o ../output/csv_run -O3 csv_parser_parallel2.cpp ALGS/alg.cpp mspace.cpp ALGS/randomAlg.cpp ALGS/optAlg.cpp ALGS/greedyAlg.cpp ALGS/WFAlg.cpp ALGS/doubleCoverageAlg.cpp RAII_Classes/getInput.cpp RAII_Classes/mcfp.cpp RAII_Classes/writeOutput.cpp ALGS/KCentersAlg.cpp RAII_Classes/buffer.cpp ALGS/lazyDoubleCoverageAlg.cpp
g++ -std=c++11 -o ../output/geninp -O3 ../generateInpts/generateStar1.cpp ./RAII_Classes/writeOutput.cpp
cd ../output
len="12"
# Second loop: size from 0 to 10
for size in {6..12}; do
    # Innermost loop: k from 0 to 3
    for k in {2..4}; do
        # Set an environment variable for the file name
        inpFile="file_inp${len}_size${size}_k${k}.csv"
        # Run the script with command line arguments len, size, and k
        ./geninp "$len" "$size" "$k"
        
        # Rename file.csv to the value stored in the environment variable
        mv file.csv "$inpFile"
        outFile="file_inp${len}_size${size}_k${k}_out.csv"
        ./csv_run -i "$inpFile" -o "$outFile"
    done
done
