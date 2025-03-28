#/bin/bash

#Program name "syntax_analyzer.cpp"
#Author: R. Nishikawa
#This file is the script file that accompanies the "syntax_analyzer.cpp" program.
#Prepare for execution in normal mode (not gdb mode).

#Delete some un-needed files
rm *.o
rm *.out

echo "Compile the source file syntax_analyzer.cpp and make an executable file"
g++ -o syntax_analyzer syntax_analyzer.cpp

#give the program permission to execute
chmod +x syntax_analyzer

#run the program for all 10 test cases
echo "Run the program syntax_analyzer with test cases and output to terminal"

echo "running test case 1"
./syntax_analyzer < <(sed -n '1p' input.txt)
echo "done"

echo "This bash script will now terminate."
