READ ME
Yash Ubale - 2014csb1040
-------------------------------------------


To compile: 
yash@ManudaPC:/media/yash/Data/Academics/6th Semester/AI/Lab 4$ g++ solution.cpp

To run:
yash@ManudaPC:/media/yash/Data/Academics/6th Semester/AI/Lab 4$ ./a.out inputfile outputfile

Example: 
yash@ManudaPC:/media/yash/Data/Academics/6th Semester/AI/Lab 4$ ./a.out 5.txt output.txt

Sample Output (in output.txt for 12 blocks using Goal Stack Planning):
56
action(unstack 5 8)
action(release 5)
action(unstack 8 7)
action(release 8)
action(pick 8)
action(release 8)
action(unstack 7 6)
action(release 7)
action(pick 5)
action(stack 5 7)
action(pick 6)
action(stack 6 8)
action(unstack 9 12)
action(stack 9 1)
action(unstack 12 11)
action(stack 12 6)
action(unstack 9 1)
action(release 9)
action(pick 9)
action(stack 9 11)
action(unstack 9 11)
action(stack 9 11)
action(unstack 9 11)
action(stack 9 5)
action(unstack 11 10)
action(stack 11 1)
action(unstack 10 3)
action(release 10)
action(pick 10)
action(release 10)
action(pick 10)
action(stack 10 12)
action(unstack 11 1)
action(release 11)
action(pick 11)
action(release 11)
action(pick 11)
action(stack 11 3)
action(unstack 10 12)
action(release 10)
action(unstack 12 6)
action(stack 12 9)
action(unstack 12 9)
action(release 12)
action(unstack 9 5)
action(stack 9 5)
action(unstack 9 5)
action(stack 9 11)
action(pick 10)
action(stack 10 12)
action(unstack 10 12)
action(release 10)
action(pick 12)
action(stack 12 9)
action(pick 10)
action(stack 10 12)
