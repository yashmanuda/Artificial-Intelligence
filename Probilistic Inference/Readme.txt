Yash Ubale - 2014csb1040
Eshwar Dev Harsha - 2014csb1023
-----------------------------------------------------------------------------------------


Variable Elemination is implemented in C++ and Rejection Sampling is implemented in Python.

First part: Variable Elimination

To compile: g++ part1.cpp
To run the file: ./a.out b1.txt q1.txt 

Output will be shown in : output_varElimination.txt

Note: Output of only variable elimination queries will be shown in this output file.
Apart from this , on the terminal the last table of distribution of variables after reducing, joining, eliminating (summing) are shown query wise. Each query table wil be separated by "----------------------------------------------"
Containing basically defines the variables in the table.


Sample: 
./a.out b1.txt q1.txt 

Sample Output (On terminal): 
Containing: 3 2 1 5 4 
3 2 -1 5 4 Pro: 0.03125
3 2 -1 5 -4 Pro: 0.03125
3 2 -1 -5 4 Pro: 0.03125
3 2 -1 -5 -4 Pro: 0.03125

-----------------------------------
Containing: 3 2 4 
3 -2 4 Pro: 0.125
3 -2 -4 Pro: 0.125

-----------------------------------
Containing: 3 2 1 5 4 
3 2 1 5 4 Pro: 0.03125
3 2 1 5 -4 Pro: 0.03125
3 2 1 -5 4 Pro: 0.03125
3 2 1 -5 -4 Pro: 0.03125
-3 2 1 5 4 Pro: 0.03125
-3 2 1 5 -4 Pro: 0.03125
-3 2 1 -5 4 Pro: 0.03125
-3 2 1 -5 -4 Pro: 0.03125
3 2 -1 5 4 Pro: 0.03125
3 2 -1 5 -4 Pro: 0.03125
3 2 -1 -5 4 Pro: 0.03125
3 2 -1 -5 -4 Pro: 0.03125
-3 2 -1 5 4 Pro: 0.03125
-3 2 -1 5 -4 Pro: 0.03125
-3 2 -1 -5 4 Pro: 0.03125
-3 2 -1 -5 -4 Pro: 0.03125
3 -2 1 5 4 Pro: 0.03125
3 -2 1 5 -4 Pro: 0.03125
3 -2 1 -5 4 Pro: 0.03125
3 -2 1 -5 -4 Pro: 0.03125
-3 -2 1 5 4 Pro: 0.03125
-3 -2 1 5 -4 Pro: 0.03125
-3 -2 1 -5 4 Pro: 0.03125
-3 -2 1 -5 -4 Pro: 0.03125
3 -2 -1 5 4 Pro: 0.03125
3 -2 -1 5 -4 Pro: 0.03125
3 -2 -1 -5 4 Pro: 0.03125
3 -2 -1 -5 -4 Pro: 0.03125
-3 -2 -1 5 4 Pro: 0.03125
-3 -2 -1 5 -4 Pro: 0.03125
-3 -2 -1 -5 4 Pro: 0.03125
-3 -2 -1 -5 -4 Pro: 0.03125

-----------------------------------



================================================================================================================================================
The input format for python program (rejection sampling) :

python p2.py b1.txt q1.txt

The methods which are not implemented in seperate files are given output value of -1 and for the methods implemented in the program (python/c++) 
will be written to "output.txt".

Note: Output of only rejection sampling queries will be shown in this output file













