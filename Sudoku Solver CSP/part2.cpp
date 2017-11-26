#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;


int main(int argc, char* argv[]){
	clock_t t;
	t = clock();
	ifstream fileRead, fileReadSecond;
	fileRead.open(argv[1]);
	string line;
	fileReadSecond.open(argv[1]);
	ofstream finalOut;
	finalOut.open(argv[2]);
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

	for(int i = 0;i < numPuzzles; i++){
		myArray[i] = new int*[9];
		for(int j = 0; j < 9; j++){
			myArray[i][j] = new int[9];
		}
	}

	int individual[numPuzzles];


	for(int i = 0; i < numPuzzles; i++){
		individual[i] = 0;
		for(int j = 0; j < 9; j++){
			for(int k = 0; k < 9; k++){
				fileReadSecond >> c;
				if(c == '.'){
					myArray[i][j][k] = 0;
				}
				else{
					myArray[i][j][k] = c - 48;
					individual[i]++;
				}
			}	
		}
	}
	int nVars = 729;
	int nClauses = 4*81*37;


//--------------------------------------------------------------------
	for(int l = 0; l < numPuzzles; l++){
		ofstream fileOut;
		fileOut.open("inputToMiniSat.txt");
		fileOut<<"p cnf "<<nVars<<" "<<(nClauses + individual[l])<<endl;

		int tempArray[9];
		for(int i = 1; i <= 9; i++){
			tempArray[i - 1] = i;
		}

		for(int j = 0; j < 81; j++){
			for(int k = 0; k < 9; k++){
				fileOut<<tempArray[k]<<" ";
			}
			fileOut<<"0"<<endl;
			for(int p = 0; p < 9; p++){
				for(int q = p + 1; q < 9; q++){
					fileOut<<"-"<<tempArray[p]<<" -"<<tempArray[q]<<" 0"<<endl;
				}
			}
			for(int r = 0; r < 9; r++){
				tempArray[r] = tempArray[r] + 9;
			}
		}

		for(int i = 0; i < 9; i++){
			for(int k = 1; k <= 9; k++){
				for(int j = 0; j < 9; j++){
					tempArray[j] = 81*i + 9*j + k;
					fileOut<<(81*i + 9*j + k)<<" ";
				}
				fileOut<<"0"<<endl;
				for(int u = 0; u < 9; u++){
					for(int v = u + 1; v < 9; v++){
						fileOut<<"-"<<tempArray[u]<<" -"<<tempArray[v]<<" 0"<<endl;
					}
				}
			}
		}

		for(int i = 0; i < 9; i++){
			for(int k = 1; k <= 9; k++){
				for(int j = 0; j < 9; j++){
					tempArray[j] = 81*j + 9*i + k;
					fileOut<<(81*j + 9*i + k)<<" ";
				}
				fileOut<<"0"<<endl;
				for(int u = 0; u < 9; u++){
					for(int v = u + 1; v < 9; v++){
						fileOut<<"-"<<tempArray[u]<<" -"<<tempArray[v]<<" 0"<<endl;
					}
				}
			}
		}


		int countIndex = 0;
		for(int r = 0; r < 7; r = r + 3){
			for(int c = 0; c < 7; c = c + 3){
				for(int k = 1; k <= 9; k++){
					countIndex = 0;
					for(int i = r; i < r + 3; i++){
						for( int j = c; j < c + 3; j++){
							tempArray[countIndex++] = 81*j + 9*i + k;
							fileOut<<(81*i + 9*j + k)<<" ";
						}
					}
					fileOut<<"0"<<endl;
					for(int u = 0; u < 9; u++){
						for(int v = u + 1; v < 9; v++){
							fileOut<<"-"<<tempArray[u]<<" -"<<tempArray[v]<<" 0"<<endl;
						}
					}
				}
			}
		}

		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				if(myArray[l][i][j] != 0){
					fileOut<<(81*i + 9*j + myArray[l][i][j])<<" 0"<<endl;
				}
			}
		}

		system("minisat inputToMiniSat.txt inputFromMiniSat.txt");

		ifstream readMiniOut;
		readMiniOut.open("inputFromMiniSat.txt");
		int inputMini[730];
		for(int i = 0; i < 730; i++){
			inputMini[i] = 0 ;
		}
		char c;
		int x;
		int cou = 0;
		readMiniOut >> c;
		readMiniOut >> c;
		readMiniOut >> c;
		while(readMiniOut >> x){
			inputMini[cou++] = x;
		}

		int counter = 0;
		for(int i = 0; i < 729; i++){
			if(inputMini[i] > 0){
				
				myArray[l][counter/9][(counter%9)] = (inputMini[i]%9 == 0)?9:inputMini[i]%9;
				counter ++;
			}
		}

		for(int j = 0; j < 9; j++){
			for(int k = 0; k < 9; k++){
				finalOut<<myArray[l][j][k];
			}
		}
		finalOut<<endl;
	}
}