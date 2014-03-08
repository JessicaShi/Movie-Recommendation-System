#include<iostream>
#include <fstream>
#include<string>
using namespace std;

const int ROW = 3900;
const int COL = 6040;
int ratingMatrix[ROW][COL];
int timeStampMatrix[ROW][COL];


int getUserID(string& s){
	int result;
	int size = s.size();
	int i = 0;
	while(i < size && s[i]!=':'){
		i++;
	}
	result = atoi(s.substr(0, i).c_str());
	s = s.substr(i + 2, size - i - 1);
	return result;
}

int getMovieID(string& s){
	int result;
	int size = s.size();
	int i = 0;
	while (i < size && s[i] != ':'){
		i++;
	}
	result = atoi(s.substr(0, i).c_str());
	s = s.substr(i + 2, size - i - 1);
	return result;
}
int getRating(string& s){
	int result;
	int size = s.size();
	int i = 0;
	while (i < size && s[i] != ':'){
		i++;
	}
	result = atoi(s.substr(0, i).c_str());
	s = s.substr(i + 2, size - i - 1);
	return result;
}
int getTimeStamp(string& s){
	int size = s.size();
	int i = 0;
	while (i < size && s[i] != ' '){
		i++;
	}
	return atoi(s.substr(0, i).c_str());
}

int main()
{
	int userIDArr[6040];
	ifstream fin;
	fin.open("ratings.dat");
	string line;
	if (fin.is_open()){
		cout << "opened" << endl;
	}
	getline(fin, line);
	int userID=getUserID(line);
	int movieID;
	int count = 1;
	while (getline(fin, line)){

		//cout << userID;
		
		int newID = getUserID(line);
		if (userID == newID){
			count++;
		}
		else{
			userIDArr[userID-1] = count;
			
			count = 1;
			userID = newID;
			
		}
	}
	userIDArr[6039] = count;
	fin.close();
	cout << "userIDArr filled" << endl;
	fin.close();
	fin.open("ratings.dat");

	if (fin.is_open()){
		cout << "file opened" << endl;
	}
	for (int i = 0; i <ROW; i++){
		int j = 0;
		for (; j < userIDArr[i] * 0.8; j++){
			getline(fin, line);
			userID = getUserID(line);
			movieID = getMovieID(line);
			ratingMatrix[userID - 1][movieID - 1] = getRating(line);
			cout << "userID: "<<userID<<"rating: "<<ratingMatrix[userID - 1][movieID - 1];
			timeStampMatrix[userID - 1][movieID - 1] = getTimeStamp(line);
			cout << " time stamp:" << timeStampMatrix[userID - 1][movieID - 1]<< endl;
		}
		for (; j < userIDArr[i]; j++){
			getline(fin, line);
		}
	}
	
	cin.get();
}
