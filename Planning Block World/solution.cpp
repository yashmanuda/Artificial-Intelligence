#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <sstream>
#include <stack> 
#include <stdlib.h>
#include <time.h>  
#include <fstream>

using namespace std;


struct state{
	float hv;
	vector<int> onTable;
	vector<int> clear;
	vector<pair<int,int> > on;
	bool empty;
	int hand;
	vector<string> listActions;
	int noActions;

	bool operator()(struct state a, struct state b) const{
		return a.hv > b.hv;
    }
};



struct stateGoalStack{
	int iden;	//1 for conjunct, 2 for predicate, 3 for action
	
	//use for conjunct
	vector<int> onTable;
	vector<int> clear;
	vector<pair<int,int> > on;
	bool empty;
	int hand;

	//use for single predicate 1 for on table, 2 for clear, 3 for on, 4 for hold, 5 for empty
	int which;

	//use for action 1 for pick block, 2 for unstack, 3 for release block, 4 for stack 
	int actionNo;
	//for 1,3 action
	int rpickBlockNo;
	//for 2,4 action
	vector <pair<int , int> > ustack;

};


bool sortOn (pair<int, int> a, pair<int, int> b){
	if(a.first < b.first){
		return true;
	}
	else if(a.first > b.first){
		return false;
	}
	else{
		return a.second <= b.second;
	}
}


void getSuccessors(struct state currentState, vector<struct state>* children, int numberBlocks){
	

	//unstack action
	for(int i = 0 ; i < currentState.on.size(); i++){
		for(int j = 0; j < currentState.clear.size(); j++){
			if(currentState.on.at(i).first == currentState.clear.at(j)){
				if(currentState.empty == true){
					struct state create;
					create.empty = false;
					create.hand = currentState.on.at(i).first;
					create.clear = currentState.clear;
					create.on = currentState.on;
					create.clear.erase(create.clear.begin() + j);
					create.clear.push_back(currentState.on.at(i).second);
					create.on.erase(create.on.begin() + i);
					create.onTable = currentState.onTable;
					create.listActions = currentState.listActions;
					create.noActions = currentState.noActions + 1;

					string ac = "action(unstack ";
					int num = currentState.clear.at(j);
					ostringstream oss, oss_2;
					oss << num;
					string firstHalf = ac + oss.str() + " ";
					num = currentState.on.at(i).second;
					oss_2 << num;
					string result = firstHalf + oss_2.str() + ")";
					create.listActions.push_back(result);
					create.hv = currentState.hv;
					children->push_back(create);
				}
			}
		}
	}

	//stack action
	for(int i = 0; i < currentState.clear.size(); i++){
		if(currentState.empty == false){
			struct state create;
			create.empty = true;
			create.onTable = currentState.onTable;
			create.clear = currentState.clear;
			create.on = currentState.on;
			create.clear.erase(create.clear.begin() + i);
			create.clear.push_back(currentState.hand);
			create.hand = 0;
			create.on.push_back(make_pair(currentState.hand, currentState.clear.at(i)));
			create.listActions = currentState.listActions;
			create.noActions = currentState.noActions + 1;
			
			int num = currentState.hand;
			string ac = "action(stack ";
			ostringstream oss,oss_2;
			oss << num;
			string firstHalf = ac + oss.str() + " ";
			num = currentState.clear.at(i);
			oss_2 << num;
			string result = firstHalf + oss_2.str() + ")";
			create.listActions.push_back(result);
			create.hv = currentState.hv;
			children->push_back(create);
		}
	}


	// release block
	if(currentState.empty == false){
		struct state create;
		int num = currentState.hand;
		create.on = currentState.on;
		create.onTable = currentState.onTable;
		create.onTable.push_back(currentState.hand);
		create.clear = currentState.clear;
		create.clear.push_back(currentState.hand);
		create.empty = true;
		create.hand = 0;
		create.listActions = currentState.listActions;
		create.noActions = currentState.noActions + 1;

		string ac = "action(release ";
		string end = ")";
		ostringstream oss;
		oss << num;
		string result = ac + oss.str() + end;
		create.listActions.push_back(result);
		create.hv = currentState.hv;
		children->push_back(create);
	}

	//pick action
	for(int i = 0; i < currentState.onTable.size(); i++){
		for(int j = 0; j < currentState.clear.size(); j++){
			// cout<<i<<" "<<j<<endl;
			if(currentState.onTable.at(i) == currentState.clear.at(j)){
				// cout<<"got it!";
				if(currentState.empty == true){
					struct state create;
					int num = currentState.onTable.at(i);
					create.on = currentState.on;
					create.onTable = currentState.onTable;
					create.onTable.erase(create.onTable.begin() + i);
					create.clear = currentState.clear;
					create.clear.erase(create.clear.begin() + j);
					create.empty = false;
					create.hand = currentState.onTable.at(i);
					create.listActions = currentState.listActions;
					create.noActions = currentState.noActions + 1;

					ostringstream oss;
					oss << num;
					string ac = "action(pick ";
					string end = ")";
					string result = ac + oss.str() + end;
					create.listActions.push_back(result);
					create.hv = currentState.hv;
					children->push_back(create);

				}
			}
		}
	}

	

	
}

bool isGoalState(struct state currentState, struct state goalState){
	vector<int> onTableGoal = goalState.onTable;
	vector<int> onTableState = currentState.onTable;

	vector<int> clearGoal = goalState.clear;
	vector<int> clearState = currentState.clear;

	vector<pair<int, int> > onGoal = goalState.on;
	vector<pair<int, int> > onState = currentState.on;

	sort (onTableGoal.begin(), onTableGoal.end());
	sort (onTableState.begin(), onTableState.end());

	if(onTableGoal != onTableState){
		return false;
	}

	sort (clearGoal.begin(), clearGoal.end());
	sort (clearState.begin(), clearState.end());

	if(clearGoal != clearState){
		return false;
	}

	sort (onGoal.begin(), onGoal.end(), sortOn);
	sort (onState.begin(), onState.end(), sortOn);

	if(onGoal != onState){
		return false;
	}

	if(goalState.empty != currentState.empty){
		return false;
	}

	return true;
}


bool isSubGoalState(struct state currentState, struct state goalState){
	vector<int> onTableGoal = goalState.onTable;
	vector<int> onTableState = currentState.onTable;

	vector<int> clearGoal = goalState.clear;
	vector<int> clearState = currentState.clear;

	vector<pair<int, int> > onGoal = goalState.on;
	vector<pair<int, int> > onState = currentState.on;

	sort (onTableGoal.begin(), onTableGoal.end());
	sort (onTableState.begin(), onTableState.end());
	int count = 0;

	for(int i = 0; i < onTableGoal.size(); i++){
		for(int j = 0 ; j < onTableState.size(); j++){
			if(onTableGoal.at(i) == onTableState.at(j)){
				count++;
			}
		}	
	}

	if(count != onTableGoal.size()){
		return false;
	}
	count = 0;

	sort (clearGoal.begin(), clearGoal.end());
	sort (clearState.begin(), clearState.end());

	for(int i = 0; i < clearGoal.size(); i++){
		for(int j = 0 ; j < clearState.size(); j++){
			if(clearGoal.at(i) == clearState.at(j)){
				count++;
			}
		}	
	}

	if(count != clearGoal.size()){
		return false;
	}
	count = 0;

	sort (onGoal.begin(), onGoal.end(), sortOn);
	sort (onState.begin(), onState.end(), sortOn);

	for(int i = 0; i < onGoal.size(); i++){
		for(int j = 0 ; j < onState.size(); j++){
			if(onGoal.at(i) == onState.at(j)){
				count++;
			}
		}	
	}


	if(count != onGoal.size()){
		return false;
	}

	if(goalState.empty != currentState.empty){
		return false;
	}

	return true;
}


int isGoalStateHeuristic(struct state currentState, struct state goalState){
	vector<int> onTableGoal = goalState.onTable;
	vector<int> onTableState = currentState.onTable;
	
	vector<int> clearGoal = goalState.clear;
	vector<int> clearState = currentState.clear;

	vector<pair<int, int> > onGoal = goalState.on;
	vector<pair<int, int> > onState = currentState.on;

	sort (onTableGoal.begin(), onTableGoal.end());
	sort (onTableState.begin(), onTableState.end());


	int stateConj = 0;
	int finalValue = 0 ;


	if(onTableGoal != onTableState){
		for(int i = 0; i < onTableState.size(); i++){
			for(int j = 0; j < onTableGoal.size(); j++){
				if(onTableState.at(i) == onTableGoal.at(j)){
					stateConj++;
				}
			}
		}
	}
	else{
		stateConj += onTableGoal.size();
	}

	finalValue += (onTableGoal.size() - stateConj)*3;
	stateConj = 0;

	sort (clearGoal.begin(), clearGoal.end());
	sort (clearState.begin(), clearState.end());

	if(clearGoal != clearState){
		for(int i = 0; i < clearState.size(); i++){
			for(int j = 0; j < clearGoal.size(); j++){
				if(clearState.at(i) == clearGoal.at(j)){
					stateConj++;
				}
			}
		}
	}
	else{
		stateConj += clearGoal.size();
	}

	finalValue += (clearGoal.size() - stateConj)*1.5;
	stateConj = 0;

	sort (onGoal.begin(), onGoal.end(), sortOn);
	sort (onState.begin(), onState.end(), sortOn);

	if(onGoal != onState){
		for(int i = 0; i < onState.size(); i++){
			for(int j = 0; j < onGoal.size(); j++){
				if(onState.at(i) == onGoal.at(j)){
					stateConj++;
					int middle = onState.at(i).second;
					for(int p = 0; p < onState.size(); p++){
						for(int q = 0; q < onGoal.size(); q++){
							if(onState.at(p).first == middle && onGoal.at(q).first == middle ){
								if(onState.at(p).second != onGoal.at(q).second){
									stateConj++;
									// cout<<"a";
								}
							}
						}
					}
				}
			}
		}
	}
	else{
		stateConj += onGoal.size();
	}

	finalValue += (onGoal.size() - stateConj)*1.5;
	if(currentState.empty != goalState.empty){
		finalValue--;
	}

	return finalValue;

}

void printState(struct state currentState){
	for(int i = 0 ; i < currentState.onTable.size(); i++){
		cout<<"Ontable "<<currentState.onTable.at(i)<<endl;
	}

	for(int i = 0 ; i < currentState.clear.size(); i++){
		cout<<"Clear "<<currentState.clear.at(i)<<endl;
	}

	for(int i = 0 ; i < currentState.on.size(); i++){
		cout<<"On "<<currentState.on.at(i).first<<" "<<currentState.on.at(i).second<<endl;
	}
}


int main(int argc, char* argv[]){
	struct state initialState;
	struct state goalState;
	ifstream fileRead;
	fileRead.open(argv[1]);
	ofstream myfile;
	myfile.open(argv[2]);
	string x;
	fileRead >> x;
	int numberBlocks = atoi(x.c_str());
	srand (time(NULL));


	fileRead>>x;
	// cout<<x;
	string ch = x.c_str();
	// cout<<ch;
	fileRead>>x;


	while( fileRead >> x && x.compare("goal") != 0 ){
		if(x.compare("(ontable") == 0){
			int num;
			char t;
			fileRead >> num >> t;
			initialState.onTable.push_back(num);
		}
		else if(x.compare("(clear") == 0){
			int num;
			char t;
			fileRead >> num >> t;
			initialState.clear.push_back(num);
		}
		else if(x.compare("(on") == 0){
			int numA, numB;
			char t;
			fileRead>>numA>>numB>>t;
			initialState.on.push_back(make_pair(numA, numB)); 
		}
		else if(x.compare("(empty)") == 0){
			initialState.empty = true;
			initialState.hand = 0;
		}
	}

	while(fileRead >> x){
		// cout<<x<<endl;
		if(x.compare("(ontable") == 0){
			int num;
			char t;
			fileRead >> num >> t;
			goalState.onTable.push_back(num);
		}
		else if(x.compare("(clear") == 0){
			int num;
			char t;
			fileRead >> num >> t;
			goalState.clear.push_back(num);
		}
		else if(x.compare("(on") == 0){
			int numA, numB;
			char t;
			fileRead>>numA>>numB>>t;
			goalState.on.push_back(make_pair(numA, numB)); 
		}
		else if(x.compare("(empty)") == 0){
			goalState.empty = true;
			goalState.hand = 0;
		}
	}

	queue <struct state > myQueue;
	myQueue.push(initialState);

	vector<struct state> temp;
	struct state result;

	// cout<<*c;
	if(ch == "f"){
		int noExpanded = 0;
		while(!myQueue.empty()){
			getSuccessors( (myQueue.front()), &temp, numberBlocks);
			if(isGoalState( (myQueue.front()) , goalState)){
				result = myQueue.front();
				break;
			}
			myQueue.pop();
			noExpanded++;

			for(int i = 0; i < temp.size(); i++){
				myQueue.push(temp.at(i));
			}
			temp.clear();
		}


		// cout<<"Expanded Nodes: "<<noExpanded<<endl;
		myfile<<result.listActions.size()<<endl;
		for(int i = 0; i < result.listActions.size(); i++){
			myfile<<result.listActions.at(i)<<endl;
		}
	}
	else if(ch == "a"){
		initialState.hv = isGoalStateHeuristic(initialState, goalState);
		initialState.noActions = 0;
		priority_queue < struct state , vector<struct state> , state > myPriorityQueue;
		myPriorityQueue.push(initialState);
		vector<struct state > tempPriority;

		int tempValue;
		int tempActions;
		int noExpanded = 0;

		while(!myPriorityQueue.empty()){
			getSuccessors( myPriorityQueue.top(), &tempPriority, numberBlocks) ;
			if( isGoalState( myPriorityQueue.top(), goalState) ){
				result = myPriorityQueue.top();
				break;
			}

			// cout<<myPriorityQueue.size()<<" ";
			tempActions = myPriorityQueue.top().noActions;
			tempValue = myPriorityQueue.top().hv;
			myPriorityQueue.pop();
			noExpanded++;
			for(int i = 0; i < tempPriority.size(); i++){
				tempPriority.at(i).noActions = tempActions + 1;
				tempPriority.at(i).hv = (tempActions + 1) + 3*isGoalStateHeuristic(tempPriority.at(i) , goalState);
				myPriorityQueue.push( tempPriority.at(i) );
			}
			tempPriority.clear();
		}

		// cout<<"Expanded Nodes: "<<noExpanded<<endl;
		myfile<<result.listActions.size()<<endl;
		for(int i = 0; i < result.listActions.size(); i++){
			myfile<<result.listActions.at(i)<<endl;
		}

	}
	else{
		int Expanded = 0;
		struct stateGoalStack initialNode;
		initialNode.onTable = goalState.onTable;
		initialNode.clear = goalState.clear;
		initialNode.on = goalState.on;
		initialNode.empty = goalState.empty;
		initialNode.hand = goalState.hand;
		initialNode.iden = 1;


		stack <stateGoalStack> myStack;
		myStack.push(initialNode);
		// cout<<myStack.size();
		vector<string> listActions;
		int u  = 0;
		
		while(!myStack.empty()){
			int idenVar = myStack.top().iden;
			u++;
			if(u > 1000){
				break;
			}
			// cout<<myStack.size()<<endl;
			if(idenVar == 1){
				// cout<<"Inside 1"<<endl;
				struct stateGoalStack tempItem;
				tempItem = myStack.top();
				struct state subGoal;
				subGoal.onTable = tempItem.onTable;
				subGoal.clear = tempItem.clear;
				subGoal.on = tempItem.on;
				subGoal.hand = tempItem.hand;
				subGoal.empty = tempItem.empty; 
				// cout<<myStack.size()<<" ";
				if(isGoalState(subGoal, goalState)){
					if(isGoalState(initialState, goalState)){
						break;
					}
				}
				if(isSubGoalState(initialState, subGoal) == true){
					myStack.pop();
				}
				else{
					int onTableNo = tempItem.onTable.size();
					int clearNo = tempItem.clear.size();
					int onNo = tempItem.on.size();
					// cout<<"Inside 1 - "<<onTableNo<<" "<<clearNo<<" "<<onNo<<" ";
					

					if(tempItem.empty == true){
						// cout<<" Also empty"<<endl;
						struct stateGoalStack create;
						create.iden = 2;
						create.which = 5;
						create.empty = true;
						myStack.push(create);
						Expanded++;
					}
					else{
						struct stateGoalStack create;
						create.iden = 2;
						create.which = 4;
						create.empty = false;
						create.hand = tempItem.hand;
						myStack.push(create);
						Expanded++;
					}

					for(int i = 0; i < clearNo; i++){
						struct stateGoalStack create;
						create.iden = 2;
						create.which = 2;
						create.clear.push_back(tempItem.clear.at(i));
						myStack.push(create);
						Expanded++;
					}


					for(int i = 0 ; i < onNo; i++){
						struct stateGoalStack create;
						create.iden = 2;
						create.which = 3;
						create.on.push_back(tempItem.on.at(onNo - 1 - i));
						myStack.push(create);
						Expanded++;
					}

					for(int i = 0; i < onTableNo; i++){
						struct stateGoalStack create;
						create.iden = 2;
						create.which = 1;
						create.onTable.push_back(tempItem.onTable.at(i));
						myStack.push(create); 
						Expanded++;
					}

				}

			}
			else if(idenVar == 2){
				// cout<<"Inside 2"<<endl;
				struct stateGoalStack tempItem;
				tempItem = myStack.top();
				int whichVar = tempItem.which;
				if(whichVar == 1){
					
					int flag = 0;
					for(int i = 0; i < initialState.onTable.size(); i++){
						if(initialState.onTable.at(i) == tempItem.onTable.at(0)){
							myStack.pop();

							flag = 1;
							break;
						}
					}
					if(flag == 0){
						// cout<<"Inside 2 1 not-popped"<<endl;
						struct stateGoalStack createRelease;
						createRelease.iden = 3;
						createRelease.actionNo = 3;
						createRelease.rpickBlockNo = tempItem.onTable.at(0);
						myStack.push(createRelease);
						Expanded++;

						struct stateGoalStack createHold;
						createHold.iden = 2;
						createHold.which = 4;
						createHold.hand = tempItem.onTable.at(0);
						myStack.push(createHold);
						Expanded++;
					}
				}
				else if(whichVar == 2){
					
					int flag = 0;
					for(int i = 0; i < initialState.clear.size(); i++){
						if(initialState.clear.at(i) == tempItem.clear.at(0)){
							myStack.pop();
							// cout<<"Inside 2 2 popped"<<endl;
							flag = 1;
							break;
						}
					}
					if(flag == 0){
						// cout<<"Inside 2 2 not-popped"<<endl;
						int tempToKnow;
						for(int j = 0; j < initialState.on.size(); j++){
							if(initialState.on.at(j).second == tempItem.clear.at(0)){
								tempToKnow = initialState.on.at(j).first;
								break;
							}
						}
						struct stateGoalStack createUnstack;
						createUnstack.iden = 3;
						createUnstack.actionNo = 2;
						createUnstack.ustack.push_back(make_pair(tempToKnow,tempItem.clear.at(0)));
						myStack.push(createUnstack);
						Expanded++;

						struct stateGoalStack precondi;
						precondi.iden = 1;
						precondi.clear.push_back(tempToKnow);
						precondi.on.push_back(make_pair(tempToKnow, tempItem.clear.at(0)));
						precondi.empty = true;
						myStack.push(precondi);
						Expanded++;
					}
				}
				else if(whichVar == 3){
					
					int flag = 0;
					for(int i = 0; i < initialState.on.size(); i++){
						if(initialState.on.at(i) == tempItem.on.at(0)){
							myStack.pop();
							// cout<<"Inside 2 3 popped"<<endl;
							flag = 1;
							break;
						}
					}
					if(flag == 0){
						// cout<<"Inside 2 3 not-popped"<<endl;
						struct stateGoalStack createStack;
						createStack.iden = 3;
						createStack.actionNo = 4;
						createStack.ustack.push_back(make_pair(tempItem.on.at(0).first, tempItem.on.at(0).second));
						myStack.push(createStack);
						Expanded++;

						struct stateGoalStack precondi;
						precondi.iden = 1;
						precondi.clear.push_back(tempItem.on.at(0).second);
						precondi.empty = false;
						//------------------------------------------------
						precondi.hand = tempItem.on.at(0).first;
						myStack.push(precondi);
						Expanded++;
					}
				}
				else if(whichVar == 4){
					
					if(initialState.hand == tempItem.hand){
						// cout<<"Inside 2 4 popped"<<endl;
						myStack.pop();
					}
					else{
						// cout<<"Inside 2 4 not-popped"<<endl;
						int trand = rand() % 100 + 1;
						if(trand > 0 && trand <= 50){
							struct stateGoalStack createPick;
							createPick.iden = 3;
							createPick.actionNo = 1;
							createPick.rpickBlockNo = tempItem.hand;
							myStack.push(createPick);
							Expanded++;

							struct stateGoalStack precondi;
							precondi.iden = 1;
							precondi.clear.push_back(tempItem.hand);
							precondi.onTable.push_back(tempItem.hand);
							precondi.empty = true;
							myStack.push(precondi);
							Expanded++;
						}
						else{

							for(int i = 0; i < initialState.on.size(); i++){
								if(initialState.on.at(i).first == tempItem.hand){
									struct stateGoalStack createUnstack;
									createUnstack.iden = 3;
									createUnstack.actionNo = 2;
									createUnstack.ustack.push_back(make_pair(tempItem.hand , initialState.on.at(i).second));
									myStack.push(createUnstack);
									Expanded++;

									struct stateGoalStack precondi;
									precondi.iden = 1;
									precondi.clear.push_back(tempItem.hand);
									precondi.on.push_back(make_pair(tempItem.hand, initialState.on.at(i).second));
									precondi.empty = true;
									myStack.push(precondi);
									Expanded++;
									break;
								}
							}
						}	
					}
				}
				else if(whichVar == 5){
					
					if(initialState.empty == true){
						// cout<<"Inside 2 5 popped"<<endl;
						myStack.pop();
					}
					else{
						// cout<<"Inside 2 5 not-popped"<<endl;
						int trand = rand() % 100 + 1;
						//release action at random
						if(trand > 0 && trand <= 50){
							struct stateGoalStack createRelease;
							createRelease.iden = 3;
							createRelease.actionNo = 3;
							createRelease.rpickBlockNo = initialState.hand;
							myStack.push(createRelease);
							Expanded++;

							struct stateGoalStack precondi;
							precondi.iden = 1;
							precondi.hand = initialState.hand;																																																																																																																																																																		
							precondi.empty = false;
							myStack.push(precondi);
							Expanded++;
						}
						//stack action at random
						else{
							int srand = rand() % initialState.clear.size();

							struct stateGoalStack createStack;
							createStack.iden = 3;
							createStack.actionNo = 4;
							createStack.ustack.push_back(make_pair(initialState.hand, initialState.clear.at(srand)));
							myStack.push(createStack);
							Expanded++;

							struct stateGoalStack precondi;
							precondi.iden = 1;
							precondi.clear.push_back(initialState.clear.at(srand));
							precondi.empty = false;
							precondi.hand = initialState.hand;
							myStack.push(precondi);
							Expanded++;
						}
					}
				}
			}
			else if(idenVar == 3){
				
				struct stateGoalStack tempItem = myStack.top();
				int actionNo = tempItem.actionNo;
				if(actionNo == 1){
					// cout<<"Inside 3 1"<<endl;
					int tLocation;
					for(int i = 0; i < initialState.onTable.size(); i++){
						if(initialState.onTable.at(i) == tempItem.rpickBlockNo){
							tLocation = i;
							break;
						}
					}

					initialState.hand = tempItem.rpickBlockNo;
					initialState.onTable.erase(initialState.onTable.begin() + tLocation);
					initialState.empty = false;

					//removing from clear
					for(int i = 0 ; i < initialState.clear.size(); i++){
						if(initialState.clear.at(i) == tempItem.rpickBlockNo){
							initialState.clear.erase(initialState.clear.begin() + i);
							break;
						}
					}

					string ac = "action(pick ";
					string end = ")";
					ostringstream oss;
					oss << tempItem.rpickBlockNo;
					string result = ac + oss.str() + end;

					listActions.push_back(result);
					myStack.pop();
				}
				else if(actionNo == 2){
					// cout<<"Inside 3 2"<<endl;

					initialState.hand = tempItem.ustack.at(0).first;
					initialState.empty = false;
					initialState.clear.push_back(tempItem.ustack.at(0).second);
					for(int i = 0; i < initialState.on.size(); i++){
						if(initialState.on.at(i) == tempItem.ustack.at(0)){
							initialState.on.erase(initialState.on.begin() + i);
							break;
						}
					}

					for(int i = 0; i < initialState.clear.size(); i++){
						if(initialState.clear.at(i) == tempItem.ustack.at(0).first){
							initialState.clear.erase(initialState.clear.begin() + i);
							break;
						}
					}

					string ac = "action(unstack ";
					string end = ")";
					ostringstream oss, oss_2;
					oss<< tempItem.ustack.at(0).first;
					oss_2<< tempItem.ustack.at(0).second;
					string result = ac + oss.str() + " " + oss_2.str() + end;
					listActions.push_back(result);

					myStack.pop();
				}
				else if(actionNo == 3){
					// cout<<"Inside 3 3"<<endl;
					initialState.onTable.push_back(initialState.hand);
					initialState.clear.push_back(initialState.hand);
					initialState.empty = true;
					initialState.hand = 0;

					string ac = "action(release ";
					string end = ")";
					ostringstream oss;
					oss << tempItem.rpickBlockNo;
					string result = ac + oss.str() + end;
					listActions.push_back(result);

					myStack.pop();
				}
				else if(actionNo == 4){
					// cout<<"Inside 3 4"<<endl;
					initialState.on.push_back(make_pair(tempItem.ustack.at(0).first,tempItem.ustack.at(0).second ));
					initialState.clear.push_back(tempItem.ustack.at(0).first);
					initialState.empty = true;
					initialState.hand = 0;

					for(int i = 0; i < initialState.clear.size(); i++){
						if(initialState.clear.at(i) == tempItem.ustack.at(0).second){
							initialState.clear.erase(initialState.clear.begin() + i);
							break;
						}
					}

					string ac = "action(stack ";
					string end = ")";
					ostringstream oss, oss_2;
					oss << tempItem.ustack.at(0).first;
					oss_2<< tempItem.ustack.at(0).second;
					string result = ac + oss.str() + " " + oss_2.str() + end;
					listActions.push_back(result);

					myStack.pop();
				}
			}
		}

		// cout<<endl;
		// cout<<"No. of expanded: "<<Expanded<<endl;
		myfile<<listActions.size()<<endl;
		for(int i = 0; i < listActions.size(); i++){
			myfile<<listActions.at(i)<<endl;
		}
	}
}