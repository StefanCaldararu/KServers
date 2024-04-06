#!/bin/bash
len="12"
# Second loop: size from 0 to 10
for size in {2..4}; do
    # Innermost loop: k from 0 to 3
    for k in {2..4}; do
        # Set an environment variable for the file name
        inpFile="file_inp${len}_size${size}_k${k}.csv"
        # Run the script with command line arguments len, size, and k
        ./gen_inp "$size" "$len" "$k"
        
        # Rename file.csv to the value stored in the environment variable
        mv file.csv "$inpFile"
        outFile="file_inp${len}_size${size}_k${k}_out.csv"
        ./run_all -i "$inpFile" -o "$outFile"
    done
done
