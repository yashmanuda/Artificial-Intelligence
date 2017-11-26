#include <iostream>
#include <fstream>
#include <time.h>
#include <queue>
using namespace std;

int backTracks = 0;

class Element{
public:
	Element(){};
	Element(int x, int y, int array[9], int d){
		xC = x;
		yC = y;
		nD = d;
		for(int i = 0; i < 9; i++){
			domain[i] = array[i];
		}
	}

	int xC, yC, nD, domain[9];
};


struct CompareDomain{
	bool operator()(const Element& a, const Element& b) const{
		return a.nD >= b.nD;
	}
};


int checkBox(int **assignment, int a, int b){
	int tempArray[9];
	for(int i = 0; i < 9; i++){
		tempArray[i] = 0;
	}
	for(int i = 3*a; i < 3*a + 3; i++){
		for(int j = 3*b; j < 3*b + 3; j++){
			if(assignment[i][j] != 0){
				if(tempArray[assignment[i][j] - 1] == 1){
					return 0;
				}
				else{
					tempArray[assignment[i][j] - 1] = 1;
				}
			}
		}
	}
	return 1;
}

int checkIfValid(int **assignment, int a, int b){

	int tempArray[9];
	for(int i = 0; i < 9; i++){
		tempArray[i] = 0;
	}

	//row check
	for(int i = 0; i < 9; i++){
		if(assignment[a][i] != 0){
			if(tempArray[assignment[a][i] - 1] == 1){
				return 0;
			}
			else{
				tempArray[assignment[a][i] - 1] = 1;
			}
		}
	}

	for(int i = 0; i < 9; i++){
		tempArray[i] = 0;
	}

	//col check
	for(int i = 0; i < 9; i++){
		if(assignment[i][b] != 0){
			if(tempArray[assignment[i][b] - 1] == 1){
				return 0;
			}
			else{
				tempArray[assignment[i][b] - 1] = 1;
			}
		}
	}

	//box check
	int boxRow, boxCol;

	boxRow = a/3;
	boxCol = b/3;

	if(checkBox(assignment, boxRow, boxCol) == 1){
		return 1;
	}
	else{
		return 0;
	}
}

int recursiveBacktrack(int **problem){
	int flag = 0;
	int row = 0, col = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(problem[i][j] == 0){
				flag = 1;
				row = i;
				col = j;
				break;
			}
		}
		if(flag == 1){
			break;
		}
	}
	if(flag == 0){
		return 1;
	}
	else{
		for(int i = 1; i <= 9; i++){
			problem[row][col] = i;
			if(checkIfValid(problem, row, col) == 1){
				if(recursiveBacktrack(problem) == 1){
					return 1;
				}
				else{
					backTracks++;
					problem[row][col] = 0;
				}
			}
			else{
				problem[row][col] = 0;
			}
		}
	}
	return 0;
}


int recursiveBacktrackBSI(int **problem, priority_queue<Element, vector<Element>, CompareDomain> *myList){
	Element temp;
	int row, col;
	if(myList->empty()){
		return 1;
	}
	else{
		temp = myList->top();
		// cout<<myList->size()<<endl;
		row = temp.xC;
		col = temp.yC;
		for(int i = 1; i <= 9; i++){
			if(temp.domain[i - 1] == 1){
				problem[row][col] = i;
				myList->pop();
				if(checkIfValid(problem, row, col) == 1){
					if(recursiveBacktrackBSI(problem, myList) == 1){
						return 1;
					}
					else{
						backTracks++;
						problem[row][col] = 0;
						myList->push(temp);
					}
				}
				else{
					myList->push(temp);
					problem[row][col] = 0;
				}
			}
		}
	}
	return 0;
}


void getDomainPriority(int **assignment, int valuePriority[9], int domain[9], int domainOfEach[][9], int row, int col){
	for(int i = 0; i < 9; i++){
		if(domain[i] == 1){
			//row check
			for(int j = 0; j < 9; j++){
				if(domainOfEach[row*9 + j][i] == 1){
					valuePriority[i] += 1;
				}
			}

			//col check
			for(int j = 0; j < 9; j++){
				if(domainOfEach[j*9 + col][i] == 1){
					valuePriority[i] += 1;
				}
			}

			//box check
			int rowN = row/3;
			rowN = rowN*3;
			int colN = col/3;
			colN = colN*3;

			for(int j = rowN; j < rowN + 3; j++ ){
				for(int k = colN; k < colN + 3; k++ ){
					if(domainOfEach[j*9 + k][i] == 1){
						valuePriority[i] += 1;
					}
				}
			}
		}
	}
}

int recursiveBacktrackBSII(int **problem, priority_queue<Element, vector<Element>, CompareDomain> *myList, int domainOfEach[][9]){
	Element temp;
	int randTemp;
	int priorityOfValues[9], marked[9], tempMarking[9];
	for(int i = 0; i < 9; i++){
		priorityOfValues[i] = -3;
		marked[i] = 0;
		tempMarking[i] = i;
	}
	int row, col, min;
	if(myList->empty()){
		return 1;
	}
	else{
		temp = myList->top();
		// cout<<myList->size()<<endl;
		for(int i = 0; i < 9; i++){
			if(temp.domain[i] == 0){
				marked[i] = 1;
			}
		}
		row = temp.xC;
		col = temp.yC;
		getDomainPriority(problem, priorityOfValues, temp.domain, domainOfEach, row, col);
		for(int i = 0; i < 9; i++){
			for(int j = i + 1; j < 9; j++){
				if(priorityOfValues[i] > priorityOfValues[j]){
					randTemp = priorityOfValues[i];
					priorityOfValues[i] = priorityOfValues[j];
					priorityOfValues[j] = randTemp;

					randTemp = tempMarking[i];
					tempMarking[i] = tempMarking[j];
					tempMarking[j] = randTemp;
				}
			}
		}

		for(int i = 0; i < 9; i++){
			if(priorityOfValues[i] != -3){
				problem[row][col] = tempMarking[i] + 1;
				myList->pop();
				if(checkIfValid(problem, row, col) == 1){
					if(recursiveBacktrackBSII(problem, myList, domainOfEach) == 1){
						return 1;
					}
					else{
						backTracks++;
						problem[row][col] = 0;
						myList->push(temp);
					}
				}
				else{
					myList->push(temp);
					problem[row][col] = 0;
				}
			}
		}

	}
	return 0;
}


int getDomain(int **assignment, int a, int b, int varDomain[9]){
	int tempArray[9];
	for(int i = 0; i < 9; i++){
		tempArray[i] = 0;
	}

	//row check
	for(int i = 0; i < 9; i++){
		if(assignment[a][i] != 0 && tempArray[assignment[a][i] - 1] == 0){
			tempArray[ assignment[a][i] - 1] = 1;
		}
	}

	// col check
	for(int i = 0; i < 9; i++){
		if(assignment[i][b] != 0 && tempArray[assignment[i][b] - 1] == 0){
			tempArray[ assignment[i][b] - 1] = 1;
		}
	}

	// box check
	int row = a/3, col = b/3;
	row = row*3;
	col = col*3;

	for(int i = row; i < row + 3; i++){
		for(int j = col; j < col + 3; j++){
			if(assignment[i][j] != 0 && tempArray[assignment[i][j] - 1] == 0){
				tempArray[assignment[i][j] - 1] = 1;
			}
		}
	}

	int count = 0;
	for(int i = 0; i < 9; i++){
		if(tempArray[i] == 0){
			varDomain[i] = 1;
			count++;
		}
	}

	return count;

}

int main(int argc, char* argv[]){
	clock_t t;
	t = clock();
	ifstream fileRead, fileReadSecond;
	ofstream fileOut;
	fileOut.open(argv[2]);
	fileRead.open(argv[1]);
	fileReadSecond.open(argv[1]);
	int typeMethod = argv[3][0] - '0';
	// cout<<typeMethod<<"sdad";
	char c;
	int count = 0, numPuzzles = 0;
	while( fileRead>>c ){
		count++;
		if(count == 81 ){
			count = 0;
			numPuzzles++;
		}
	}
	int ***myArray, ***myArraySearchBS, ***myArraySearchBSI, ***myArraySearchBSII, ***myArraySearchBSMAC;

	myArray = new int**[numPuzzles];
	myArraySearchBS = new int**[numPuzzles];
	myArraySearchBSI = new int**[numPuzzles];
	myArraySearchBSII = new int**[numPuzzles];
	myArraySearchBSMAC = new int**[numPuzzles];

	for(int i = 0;i < numPuzzles; i++){
		myArray[i] = new int*[9];
		myArraySearchBS[i] = new int*[9];
		myArraySearchBSI[i] = new int*[9];
		myArraySearchBSII[i] = new int*[9];
		myArraySearchBSMAC[i] = new int*[9];
		for(int j = 0; j < 9; j++){
			myArray[i][j] = new int[9];
			myArraySearchBS[i][j] = new int[9];
			myArraySearchBSI[i][j] = new int[9];
			myArraySearchBSII[i][j] = new int[9];
			myArraySearchBSMAC[i][j] = new int[9];
		}
	}

	for(int i = 0; i < numPuzzles; i++){
		for(int j = 0; j < 9; j++){
			for(int k = 0; k < 9; k++){
				fileReadSecond >> c;
				if(c == '.'){
					myArray[i][j][k] = 0;
					myArraySearchBS[i][j][k] = 0;
					myArraySearchBSI[i][j][k] = 0;
					myArraySearchBSII[i][j][k] = 0;
					myArraySearchBSMAC[i][j][k] = 0;
				}
				else{
					myArray[i][j][k] = c - 48;
					myArraySearchBS[i][j][k] = c - 48;
					myArraySearchBSI[i][j][k] = c - 48;
					myArraySearchBSII[i][j][k] = c - 48;
					myArraySearchBSMAC[i][j][k] = c - 48;
				}
			}	
		}
	}

	if(typeMethod == 1){
		for(int i = 0; i < 10; i++){
			backTracks = 0;
			recursiveBacktrack(myArraySearchBS[i]);
			cout<<backTracks<<endl;
		}
	
		for(int i = 0; i < numPuzzles; i++){
			for(int j = 0; j < 9; j++){
				for(int k = 0; k < 9; k++){
					fileOut<<myArraySearchBS[i][j][k];
				}
			}
			fileOut<<endl;
		}	
	}
	else if(typeMethod == 2){
		Element temp;
		priority_queue<Element, vector<Element>, CompareDomain> myList;
		for(int k = 0; k < 10; k++){
			int varArray[9];
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){	
					for(int z = 0; z < 9; z++){
						varArray[z] = 0; 
					}
					if(myArraySearchBSI[k][i][j] == 0){
						temp = Element(i, j, varArray, getDomain(myArraySearchBSI[k], i , j, varArray));
						myList.push(temp);
					}
				}
			}
			backTracks = 0;
			recursiveBacktrackBSI(myArraySearchBSI[k], &myList);
			cout<<backTracks<<endl;
		}

		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 9; j++){
				for(int k = 0; k < 9; k++){
					fileOut<<myArraySearchBSI[i][j][k];
				}
			}
			fileOut<<endl;
		}	
	}
	else if(typeMethod == 3){
		int domainOfEach[81][9];
		int tempCount = 0;
		Element temp;
		priority_queue<Element, vector<Element>, CompareDomain> myList;
		for(int k = 0; k < 10; k++){
			tempCount = 0;
			for(int i = 0; i < 81; i++){
				for(int j = 0; j < 9; j++){
					domainOfEach[i][j] = 0;
				}
			}
			int varArray[9];
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){	
					for(int z = 0; z < 9; z++){
						varArray[z] = 0; 
					}
					if(myArraySearchBSII[k][i][j] == 0){
						temp = Element(i, j, varArray, getDomain(myArraySearchBSII[k], i , j, varArray));
						myList.push(temp);
						for(int u = 0; u < 9; u++){
							domainOfEach[tempCount][u] = varArray[u];
						}
					}
					tempCount++;
				}
			}
			backTracks = 0;
			recursiveBacktrackBSII(myArraySearchBSII[k], &myList, domainOfEach);
			cout<<backTracks<<endl;
		}



		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 9; j++){
				for(int k = 0; k < 9; k++){
					fileOut<<myArraySearchBSII[i][j][k];
				}
			}
			fileOut<<endl;
		}	
	}
	

	t = clock() - t;
	cout<<((float)t)/CLOCKS_PER_SEC;

	return 0;
}