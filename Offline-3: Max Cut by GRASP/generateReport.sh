#!/bin/bash

# Create or overwrite the report.csv file with headers
# echo ",Problem,,,Constructive Algorithm,,,Local search,,,,,,GRASP,,,Upper Bound " >> report22.csv
# echo "Name,|V|,|E|,Randomized,Greedy,Semi-Greedy,Local-1(random),,Local-2(greedy),,Local-3(alpha=0.7),, GRASP-1(greedy max_it=3),,GRASP-2(alpha=0.6 max_it=50) , " >> report22.csv
# echo ",,,,,, Iteration, Best Value, Iteration, Best value,Iteration, Best value,Iteration, Best value,Iteration, Best value," >> report22.csv


# Compile the C++ code
g++ 1905120.cpp -o a.out



# Run the compiled executable with different input files 

for i in {22..24}; do
    ./a.out $i
done
