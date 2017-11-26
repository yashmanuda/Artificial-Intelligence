#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <string.h>

using namespace std;

struct conditionTable{
	int child;
	vector<int> parents;
	float **table;
	conditionTable(){};

	conditionTable(int givenChild, vector<int> givenParents){
		table = new float*[(int)pow(2,givenParents.size() + 1) + 1];
		parents = givenParents;
		child = givenChild;
		for(int i = 0 ; i < pow(2,givenParents.size() + 1) + 1; i++){
			table[i] = new float[givenParents.size() + 3];
		}

		//marker is at first column
		for(int i = 0; i < pow(2,givenParents.size() + 1); i++){
			table[i][0] = 0;
		}
		table[0][0] = -1;
		table[0][givenParents.size() + 2] = -1;
		table[0][1] = givenChild;
		for(int i = 2; i < givenParents.size() + 2; i++){
			table[0][i] = givenParents.at(i - 2);
		}
		
		int bin = pow(2,givenParents.size() + 1) - 1;
		int binCopy = bin;
		int index;
		for(int i = 1; i <= pow(2,givenParents.size() + 1); i++){
			index = givenParents.size() + 1;
			while(binCopy != 0 ){
				table[i][index--] = binCopy%2;
				binCopy = binCopy/2;
			}
			bin--;
			binCopy = bin;
		}

	}
};


struct Node{
	vector<int> query;
	float pro;
};

struct Table{
	vector<int> contains;
	vector<struct Node> configurations;
};



void printStruct(struct conditionTable item){
	cout<<endl;
	cout<<item.child<<"  ";
	for(int i = 0; i < item.parents.size(); i++){
		cout<<item.parents.at(i)<<"  ";
	}
	cout<<endl;
	for(int i = 0; i < pow(2,item.parents.size() + 1) + 1  ; i++){
		for(int j = 0; j < item.parents.size() + 3; j++){
			cout<<item.table[i][j]<<"  ";
		}
		cout<<endl;
	}

}

void printNode(Node toPrint){
	for(int i = 0; i < toPrint.query.size(); i++){
		cout<<toPrint.query.at(i)<<" ";
	}
	cout<<"Pro: "<<toPrint.pro<<endl;
}

bool containsConfiguration(vector<int> searchIn, vector<int> searchFor){
	int count = searchFor.size();
	int markCount = 0;
	for(int i = 0; i < count; i++){
		for(int j = 0; j < searchIn.size(); j++){
			if(searchFor.at(i) == searchIn.at(j)){
				markCount++;
				break;
			}
		}
	}

	return markCount == count;
}

bool containsValue(vector<int> searchIn, int searchFor){
	for(int i = 0; i < searchIn.size(); i++){
		if(searchIn.at(i) == searchFor){
			return true;
		}
	}
	return false;
}

bool containsConfigurationSame(vector<int> searchP, vector<int> searchN, int elim){
	int count = searchP.size();
	int markCount = 1;
	for(int i = 0; i < count; i++){
		if(searchP.at(i) != elim){
			for(int j = 0; j < searchN.size(); j++){
				if(searchP.at(i) == searchN.at(j)){
					markCount++;
					break;
				}
			}
		}
	}

	return markCount == count;
}


float getProbabilityValue(vector<struct conditionTable> input, vector<int> hiddenVariables, vector<int> evidence, int numVar ){
	float result = 0;
	int myArray[numVar];
	vector<int> toEliminate;
	vector<int> notToEliminate;
	vector<int> combined;

	for(int i = 0; i < hiddenVariables.size(); i++){
		combined.push_back(hiddenVariables.at(i));		
	}

	for(int i = 0; i < evidence.size(); i++){
		combined.push_back(evidence.at(i));		
	}	

	for(int i = 0; i < numVar; i++){
		myArray[i] = 0;
	}

	for(int i = 0; i < hiddenVariables.size(); i++){
		myArray[abs(hiddenVariables.at(i)) - 1] = 1;
	}

	for(int j = 0; j < evidence.size(); j++){
		myArray[abs(evidence.at(j)) - 1] = 1;
	}

	for(int i = 0; i < numVar; i++){
		if(myArray[i] == 0){
			toEliminate.push_back(i + 1);
		}
		else{
			notToEliminate.push_back(i + 1);
		}
	}

	// for(int i = 0; i < notToEliminate.size(); i++){
	// 	cout<<notToEliminate.at(i)<<" ";
	// }
	// cout<<endl;

	//reduction
	int temp;
	// cout<<"size evidence: "<<evidence.size()<<endl;
	for(int i = 0; i < evidence.size(); i++){
		// cout<<"ghusa!"<<endl;
		temp = evidence.at(i);
		if(temp > 0){
			for(int j = 0; j < input.size(); j++){
				if(input.at(j).child == temp){
					for(int k = 1; k <= pow(2,input.at(j).parents.size()); k++ ){
						input.at(j).table[k + (int)pow(2,input.at(j).parents.size())][0] = 1; 
					}
				}
				else{
					for(int k = 2; k < input.at(j).parents.size() + 2; k++){
						if(input.at(j).table[0][k] == temp){
							for(int m = 1; m <= pow(2, input.at(j).parents.size() + 1); m++ ){
								if(input.at(j).table[m][k] == 0){
									input.at(j).table[m][0] = 1;
								}
							}
						}
					}
				}
			}
		}
		else{
			temp = abs(temp);
			for(int j = 0; j < input.size(); j++){
				if(input.at(j).child == temp){
					for(int k = 1; k <= pow(2,input.at(j).parents.size()); k++ ){
						input.at(j).table[k][0] = 1; 
					}
				}
				else{
					for(int k = 2; k < input.at(j).parents.size() + 2; k++){
						if(input.at(j).table[0][k] == temp){
							for(int m = 1; m <= pow(2, input.at(j).parents.size() + 1); m++ ){
								if(input.at(j).table[m][k] == 1){
									input.at(j).table[m][0] = 1;
								}
							}
						}
					}
				}
			}
		}
	}
	

	vector<Table> indiTables;
	for(int i = 0; i < input.size(); i++){
		struct Table tempTable;
		tempTable.contains.push_back(input.at(i).child);
		for(int j = 0; j < input.at(i).parents.size(); j++){
			tempTable.contains.push_back(input.at(i).parents.at(j));
		}
		for(int j = 1; j <= pow(2,input.at(i).parents.size() + 1 ); j++ ){
			if(input.at(i).table[j][0] == 0){
				struct Node tempNode;
				for(int k = 1; k <= input.at(i).parents.size() + 1; k++){
					if(input.at(i).table[j][k] == 1){
						tempNode.query.push_back(input.at(i).table[0][k]);
					}
					else{
						tempNode.query.push_back(-input.at(i).table[0][k]);
					}
					tempNode.pro = input.at(i).table[j][input.at(i).parents.size() + 2];
				}
				tempTable.configurations.push_back(tempNode);
			}
		}
		indiTables.push_back(tempTable);
	}

	//---------------------------------------------
	//joining factors
	// cout<<indiTables.size()<<endl;

	while(toEliminate.size() != 0){
		vector<Table> tempRemoveTables;
		int val = toEliminate.at(0);

		for(int i = 0; i < indiTables.size(); i++){
			if(containsValue(indiTables.at(i).contains, val)){
				tempRemoveTables.push_back(indiTables.at(i));
				indiTables.erase(indiTables.begin() + i);
				i = 0;
			}
		}

		while(tempRemoveTables.size() > 1){
			vector<int> commonVar;
			float tempPro;
			for(int i = 0; i < tempRemoveTables.at(0).contains.size(); i++){
				for(int j = 0; j < tempRemoveTables.at(1).contains.size(); j++){
					if(tempRemoveTables.at(0).contains.at(i) == tempRemoveTables.at(1).contains.at(j)){
						commonVar.push_back(tempRemoveTables.at(0).contains.at(i));
					}
				}
			}

			struct Table mergeTable;
			for(int l = 0; l  < commonVar.size(); l++){
				mergeTable.contains.push_back(commonVar.at(l));
				// cout<<commonVar.at(l)<<endl;
			}

			for(int l = 0; l < tempRemoveTables.at(0).contains.size(); l++){
				// cout<<tempRemoveTables.at(0).contains.at(l)<<endl;
				if(!containsValue(mergeTable.contains, tempRemoveTables.at(0).contains.at(l))){
					mergeTable.contains.push_back(tempRemoveTables.at(0).contains.at(l));
				}
			}


			for(int l = 0; l < tempRemoveTables.at(1).contains.size(); l++){
				// cout<<tempRemoveTables.at(1).contains.at(l)<<endl;
				if(!containsValue(mergeTable.contains, tempRemoveTables.at(1).contains.at(l))){
					mergeTable.contains.push_back(tempRemoveTables.at(1).contains.at(l));
				}
			}

			

			// cout<<mergeTable.contains.size()<<endl;
			for(int i = 0; i < tempRemoveTables.at(0).configurations.size(); i++){
				vector<int> tempConfig;
				for(int m = 0; m < commonVar.size(); m++){
					for(int n = 0; n < tempRemoveTables.at(0).configurations.at(i).query.size(); n++){
						if(commonVar.at(m) == abs(tempRemoveTables.at(0).configurations.at(i).query.at(n))){
							tempConfig.push_back(tempRemoveTables.at(0).configurations.at(i).query.at(n));
							// tempRemoveTables.at(0).configurations.at(i).query.erase(tempRemoveTables.at(0).configurations.at(i).query.begin() + n);
							// n = 0;
							break;
						}
					}
				}


				vector<int> copyTempConfig = tempConfig;
				for(int j = 0; j < tempRemoveTables.at(1).configurations.size(); j++){
					if(containsConfiguration(tempRemoveTables.at(1).configurations.at(j).query , tempConfig) == true){
						

						for(int p = 0; p < tempRemoveTables.at(0).configurations.at(i).query.size(); p++){
							if(containsValue(tempConfig,tempRemoveTables.at(0).configurations.at(i).query.at(p)) == false){
								tempConfig.push_back(tempRemoveTables.at(0).configurations.at(i).query.at(p));
								// cout<<"Enter in second: "<<tempRemoveTables.at(1).configurations.at(j).query.at(p)<<endl;
							}
						}

						for(int p = 0; p < tempRemoveTables.at(1).configurations.at(j).query.size(); p++){
							if(containsValue(tempConfig,tempRemoveTables.at(1).configurations.at(j).query.at(p)) == false){
								tempConfig.push_back(tempRemoveTables.at(1).configurations.at(j).query.at(p));
								// cout<<"Enter in second: "<<tempRemoveTables.at(1).configurations.at(j).query.at(p)<<endl;
							}
						}


						tempPro = tempRemoveTables.at(0).configurations.at(i).pro*tempRemoveTables.at(1).configurations.at(j).pro;
						struct Node tempNodeAdd;
						tempNodeAdd.pro = tempPro;
						tempNodeAdd.query = tempConfig;
						mergeTable.configurations.push_back(tempNodeAdd);
					}
					tempConfig = copyTempConfig;
				}

			}
			tempRemoveTables.erase(tempRemoveTables.begin());
			tempRemoveTables.erase(tempRemoveTables.begin());
			tempRemoveTables.insert(tempRemoveTables.begin(), mergeTable);

			// 
			// cout<<commonVar.size()<<endl;
			// break;
		}
		


		for(int p = 0; p < tempRemoveTables.at(0).configurations.size(); p++){
			for(int q = p + 1; q < tempRemoveTables.at(0).configurations.size(); q++){
				if(containsValue( tempRemoveTables.at(0).configurations.at(p).query , val)){
					if(containsValue(tempRemoveTables.at(0).configurations.at(q).query , -val)){
						if(containsConfigurationSame(tempRemoveTables.at(0).configurations.at(p).query, tempRemoveTables.at(0).configurations.at(q).query,val)){
							for(int l = 0; l < tempRemoveTables.at(0).configurations.at(p).query.size(); l++){
								if(tempRemoveTables.at(0).configurations.at(p).query.at(l) == val){
									tempRemoveTables.at(0).configurations.at(p).query.erase(tempRemoveTables.at(0).configurations.at(p).query.begin() + l);
									break;
								}
							}
							for(int l = 0; l < tempRemoveTables.at(0).contains.size(); l++){
								if(tempRemoveTables.at(0).contains.at(l) == abs(val)){
									tempRemoveTables.at(0).contains.erase(tempRemoveTables.at(0).contains.begin() + l);
									break;
								}
							}
							tempRemoveTables.at(0).configurations.at(p).pro += tempRemoveTables.at(0).configurations.at(q).pro;
							tempRemoveTables.at(0).configurations.erase(tempRemoveTables.at(0).configurations.begin() + q);
							p = 0;
							q = 0;
						}
					}
				}
				else if(containsValue( tempRemoveTables.at(0).configurations.at(p).query , -val)){
					if(containsValue(tempRemoveTables.at(0).configurations.at(q).query , val)){
						if(containsConfigurationSame(tempRemoveTables.at(0).configurations.at(q).query, tempRemoveTables.at(0).configurations.at(p).query,val)){
							for(int l = 0; l < tempRemoveTables.at(0).configurations.at(p).query.size(); l++){
								if(tempRemoveTables.at(0).configurations.at(p).query.at(l) == -val){
									tempRemoveTables.at(0).configurations.at(p).query.erase(tempRemoveTables.at(0).configurations.at(p).query.begin() + l);
									break;
								}
							}
							for(int l = 0; l < tempRemoveTables.at(0).contains.size(); l++){
								if(tempRemoveTables.at(0).contains.at(l) == abs(val)){
									tempRemoveTables.at(0).contains.erase(tempRemoveTables.at(0).contains.begin() + l);
									break;
								}
							}
							tempRemoveTables.at(0).configurations.at(p).pro += tempRemoveTables.at(0).configurations.at(q).pro;
							tempRemoveTables.at(0).configurations.erase(tempRemoveTables.at(0).configurations.begin() + q);
							p = 0;
							q = 0;
						}
					}
				}
			}
		}





		// for(int p = 0; p < tempRemoveTables.at(0).contains.size(); p++){
		// 		cout<<tempRemoveTables.at(0).contains.at(p)<<" ";
		// 	}
		// cout<<endl;
		// for(int p = 0 ;p < tempRemoveTables.at(0).configurations.size(); p++){
		// 	printNode(tempRemoveTables.at(0).configurations.at(p));
		// }
		// cout<<endl;

		indiTables.insert(indiTables.begin(), tempRemoveTables.at(0));
		toEliminate.erase(toEliminate.begin());
		// break;
	}

	while(notToEliminate.size() != 0){
		vector<Table> tempRemoveTables;
		int val = notToEliminate.at(0);

		for(int i = 0; i < indiTables.size(); i++){
			if(containsValue(indiTables.at(i).contains, val)){
				tempRemoveTables.push_back(indiTables.at(i));
				indiTables.erase(indiTables.begin() + i);
				if(indiTables.size() == 1){
					i = -1;
				}
				else{
					i = 0;	
				}
				
			}
		}

		while(tempRemoveTables.size() > 1){
			vector<int> commonVar;
			float tempPro;
			for(int i = 0; i < tempRemoveTables.at(0).contains.size(); i++){
				for(int j = 0; j < tempRemoveTables.at(1).contains.size(); j++){
					if(tempRemoveTables.at(0).contains.at(i) == tempRemoveTables.at(1).contains.at(j)){
						commonVar.push_back(tempRemoveTables.at(0).contains.at(i));
					}
				}
			}

			struct Table mergeTable;
			for(int l = 0; l  < commonVar.size(); l++){
				mergeTable.contains.push_back(commonVar.at(l));
				// cout<<commonVar.at(l)<<endl;
			}

			for(int l = 0; l < tempRemoveTables.at(0).contains.size(); l++){
				// cout<<tempRemoveTables.at(0).contains.at(l)<<endl;
				if(!containsValue(mergeTable.contains, tempRemoveTables.at(0).contains.at(l))){
					mergeTable.contains.push_back(tempRemoveTables.at(0).contains.at(l));
				}
			}


			for(int l = 0; l < tempRemoveTables.at(1).contains.size(); l++){
				// cout<<tempRemoveTables.at(1).contains.at(l)<<endl;
				if(!containsValue(mergeTable.contains, tempRemoveTables.at(1).contains.at(l))){
					mergeTable.contains.push_back(tempRemoveTables.at(1).contains.at(l));
				}
			}

			

			// cout<<mergeTable.contains.size()<<endl;
			for(int i = 0; i < tempRemoveTables.at(0).configurations.size(); i++){
				vector<int> tempConfig;
				for(int m = 0; m < commonVar.size(); m++){
					for(int n = 0; n < tempRemoveTables.at(0).configurations.at(i).query.size(); n++){
						if(commonVar.at(m) == abs(tempRemoveTables.at(0).configurations.at(i).query.at(n))){
							tempConfig.push_back(tempRemoveTables.at(0).configurations.at(i).query.at(n));
							// tempRemoveTables.at(0).configurations.at(i).query.erase(tempRemoveTables.at(0).configurations.at(i).query.begin() + n);
							// n = 0;
							break;
						}
					}
				}


				vector<int> copyTempConfig = tempConfig;
				for(int j = 0; j < tempRemoveTables.at(1).configurations.size(); j++){
					if(containsConfiguration(tempRemoveTables.at(1).configurations.at(j).query , tempConfig) == true){
						

						for(int p = 0; p < tempRemoveTables.at(0).configurations.at(i).query.size(); p++){
							if(containsValue(tempConfig,tempRemoveTables.at(0).configurations.at(i).query.at(p)) == false){
								tempConfig.push_back(tempRemoveTables.at(0).configurations.at(i).query.at(p));
								// cout<<"Enter in second: "<<tempRemoveTables.at(1).configurations.at(j).query.at(p)<<endl;
							}
						}

						for(int p = 0; p < tempRemoveTables.at(1).configurations.at(j).query.size(); p++){
							if(containsValue(tempConfig,tempRemoveTables.at(1).configurations.at(j).query.at(p)) == false){
								tempConfig.push_back(tempRemoveTables.at(1).configurations.at(j).query.at(p));
								// cout<<"Enter in second: "<<tempRemoveTables.at(1).configurations.at(j).query.at(p)<<endl;
							}
						}


						tempPro = tempRemoveTables.at(0).configurations.at(i).pro*tempRemoveTables.at(1).configurations.at(j).pro;
						struct Node tempNodeAdd;
						tempNodeAdd.pro = tempPro;
						tempNodeAdd.query = tempConfig;
						mergeTable.configurations.push_back(tempNodeAdd);
					}
					tempConfig = copyTempConfig;
				}

			}
			tempRemoveTables.erase(tempRemoveTables.begin());
			tempRemoveTables.erase(tempRemoveTables.begin());
			tempRemoveTables.insert(tempRemoveTables.begin(), mergeTable);

			// 
			// cout<<commonVar.size()<<endl;
			// break;
		}
	

		indiTables.insert(indiTables.begin(), tempRemoveTables.at(0));
		notToEliminate.erase(notToEliminate.begin());
		// break;
	}

	for(int i = 0; i < indiTables.size(); i++){
		cout<<"Containing: ";
		for(int p = 0; p < indiTables.at(i).contains.size(); p++){
				cout<<indiTables.at(i).contains.at(p)<<" ";
			}
		cout<<endl;
		for(int p = 0 ;p < indiTables.at(i).configurations.size(); p++){
			printNode(indiTables.at(i).configurations.at(p));
		}
		cout<<endl;
	}

	cout<<"-----------------------------------"<<endl;


	for(int i = 0; i < indiTables.at(0).configurations.size(); i++){
		if(containsConfiguration(indiTables.at(0).configurations.at(i).query , combined)){
			result = indiTables.at(0).configurations.at(i).pro;
		}
	}


	float norm = 0;

	for(int i = 0; i < indiTables.at(0).configurations.size(); i++){
		if(containsConfiguration(indiTables.at(0).configurations.at(i).query , evidence)){
			norm += indiTables.at(0).configurations.at(i).pro;
		}
	}

	return result/norm;
}


int main(int argc, char* argv[]){
	int numVar = 0;
	ifstream fileConditions;
	fileConditions.open(argv[1]);
	string line;
	getline(fileConditions, line);
	numVar = atoi(line.c_str());
	vector<conditionTable> myInput;
	vector<int> givenParents;


	while(getline(fileConditions, line)){
		givenParents.clear();
		stringstream stream(line);
		int child;
		stream>>child;

		while(true){
			int n;
			stream>>n;
			if(!stream){
				break;
			}
			givenParents.push_back(n);
		}


		struct conditionTable temp = conditionTable(child, givenParents);
		for(int i = 0 ; i < pow(2,temp.parents.size()); i++){
			getline(fileConditions, line);
			stringstream floatstream(line);
			float childFloat;
			floatstream>>childFloat;
			temp.table[i + 1][temp.parents.size() + 2] = childFloat;
			temp.table[(int)pow(2,temp.parents.size()) + i + 1][ temp.parents.size() + 2 ] = 1 - childFloat;
		}
		myInput.push_back(temp);

	}

	// for(int i = 0; i < myInput.size(); i++){
	// 	printStruct(myInput.at(i));
	// }

	
	vector<int> hiddenVariables;
	vector<int> evidence;
	

	ifstream fileQueries;
	ofstream fileOut;
	fileOut.open("output_varElimination.txt");
	fileQueries.open(argv[2]);
	string queryLine;
	// getline(fileQueries, queryLine);
	while(getline(fileQueries, queryLine)){

		// CopyVector.clear();
		hiddenVariables.clear();
		evidence.clear();

		if(queryLine.substr(0,2) == "ve"){

			const char *charArray = queryLine.substr(5).c_str();
			char *query = new char[1000];
			strcpy(query, charArray);
			int indexOfE = 0;
			for(int i = 0; i < strlen(query); i++){
				if(query[i] == 'e'){
					indexOfE = i;
					break;
				}
			}

			for(int i = 0; i < strlen(query); i++){
				if(query[i] == '~'){
					query[i] = '-';
				}
			}

			char *tempChar;
			int flag = 0;
			tempChar = strtok(query, " ");
			while(tempChar != NULL){
				if(!strcmp(tempChar, "e")){
					flag = 1;
				}
				else if(flag == 0){
					hiddenVariables.push_back(atoi(tempChar));
				}
				else{
					evidence.push_back(atoi(tempChar));
				}
				tempChar = strtok(NULL, " ");
			}

			
			vector<struct conditionTable> CopyVector(myInput);
			float result = getProbabilityValue(CopyVector, hiddenVariables, evidence, numVar);
			fileOut<<result<<endl;
			CopyVector.clear();

			//resetting table
			for(int i = 0; i < myInput.size(); i++){
				for(int j = 1; j <= pow(2,myInput.at(i).parents.size() + 1); j++){
					myInput.at(i).table[j][0] = 0;
				}
			}

		}
	}

	return 0;
}