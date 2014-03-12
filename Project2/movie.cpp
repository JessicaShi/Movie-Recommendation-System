#include<iostream>
#include <fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<unordered_map>
using namespace std;



struct ratings{
	ratings(){}
	ratings(int m_userID, int m_movieID, int m_rating, int m_timestamp):userID(m_userID), movieID(m_movieID), rating(m_rating), timeStamp(m_timestamp){}
	int userID;
	int movieID;
	int rating;
	int timeStamp;
};

unordered_map<string, int> MovieGenreMap;


struct userData{
	userData(){};
	userData(int m_userID, int m_age, int m_gender, vector<double>& m_perference, double m_average, double m_variant) :userID(m_userID), age(m_age), gender(m_gender), perference(m_perference),
	average(m_average), variant(m_variant){}
	       
	int userID;
	int age;
	int gender;        //0 is male, 1 is female;
	vector<double> perference;
	double average;
	double variant;
};

struct movieData{
	movieData(){};
	movieData(int m_movieID, string m_title, int m_year, vector<int>& m_genre, int m_rating) :movieID(m_movieID), genre(m_genre), title(m_title), year(m_year), rating(m_rating){}
	int movieID;
	vector<int> genre;
	string title;
	int year;
	int rating;
};
void buildMovieGenereMap(){
	int count = 0;
	MovieGenreMap.insert(pair<string, int>("Action", count++));
	MovieGenreMap.insert(pair<string, int>("Adventure", count++));
	MovieGenreMap.insert(pair<string, int>("Animation", count++));
	MovieGenreMap.insert(pair<string, int>("Children's", count++));
	MovieGenreMap.insert(pair<string, int>("Comedy", count++));
	MovieGenreMap.insert(pair<string, int>("Crime", count++));
	MovieGenreMap.insert(pair<string, int>("Documentary", count++));
	MovieGenreMap.insert(pair<string, int>("Drama", count++));
	MovieGenreMap.insert(pair<string, int>("Fantasy", count++));
	MovieGenreMap.insert(pair<string, int>("Film_Noir", count++));
	MovieGenreMap.insert(pair<string, int>("Horror", count++));
	MovieGenreMap.insert(pair<string, int>("Musical", count++));
	MovieGenreMap.insert(pair<string, int>("Mystery", count++));
	MovieGenreMap.insert(pair<string, int>("Romance", count++));
	MovieGenreMap.insert(pair<string, int>("Sci_Fi", count++));
	MovieGenreMap.insert(pair<string, int>("Thriller", count++));
	MovieGenreMap.insert(pair<string, int>("War", count++));
	MovieGenreMap.insert(pair<string, int>("Western", count++));

}

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

bool myCompare(ratings s1, ratings s2){
	return s1.timeStamp < s2.timeStamp;
}
int getMovieID2(string& s){
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
string getMovieTitle(string& s){
	int i = 0;
	int size = s.size();
	while (i < size && s[i] != '('){
		i++;
	}

	string result = s.substr(0, i);
	s = s.substr(i + 1, size - i);
	return result;
	
}
int getMovieYear(string& s){
	int i = 0;
	int size = s.size();
	while(i < size && s[i] != ')') {
		i++;
	}
	string resultString = s.substr(0, i);
	int result = atoi(resultString.c_str());
	return result;
}

vector<int> getMovieType(string& s){
	vector<int> result;
	int size = s.size();
	int i = 0;
	unordered_map<string, int> ::iterator it;
	while(i < size){
		string toBeInserted;
		if (s[i] != '|'){
			i++;
		}
		else{
			toBeInserted = s.substr(0, i);
			s = s.substr(i + 1, s.size() - i);
			it = MovieGenreMap.find(toBeInserted);
			result.push_back(it->second);
		}
	}
	it = MovieGenreMap.find(s);
	result.push_back(it->second);
	return result;


}

const int ROW = 6040;
const int COL = 3900;
int ratingMatrix[ROW][COL];  //sotores which user watches which movie and the movie rating from the user;
int timeStampMatrix[ROW][COL]; 
ratings ratingArr[1000209];  //helper array
movieData movieArr[3900];   //store all the 3900 movies information
userData userArr[6040];    
int userIDArr[6040];       //store the number of movies each user watched

void initializeUserIDArr(){
	ifstream fin;
	string line;


	fin.open("ratings.dat");

	if (fin.is_open()){
		cout << "opened" << endl;
	}
	getline(fin, line);
	int userID = getUserID(line);
	int movieID = getMovieID(line);
	int rating = getRating(line);
	int timeStamp = getTimeStamp(line);
	int count = 1;
	int i = 0;
	ratingArr[i++] = ratings(userID, movieID, rating, timeStamp);
	while (getline(fin, line)){

		//cout << userID;

		int newID = getUserID(line);
		if (userID == newID){
			count++;
		}
		else{
			userIDArr[userID - 1] = count;

			count = 1;
			userID = newID;

		}
		movieID = getMovieID(line);
		rating = getRating(line);
		timeStamp = getTimeStamp(line);
		ratingArr[i++] = ratings(newID, movieID, rating, timeStamp);

	}
	userIDArr[6039] = count;
	fin.close();
	//cout << "userIDArr filled" << endl;
}

void initializeRatingMatrix(){
	for (int i = 0; i < ROW; i++){
		for (int j = 0; j < COL; j++){
			ratingMatrix[i][j] = -1;
		}
		
	}
	initializeUserIDArr();
	ifstream fin;
	string line;
	fin.open("ratings.dat");

	if (fin.is_open()){
		cout << "file opened" << endl;
	}
	int startPoint = 0;
	int endPoint = 0;
	int k = 0;
	for (int i = 0; i <ROW; i++){
		int j = 0;
		startPoint = endPoint;
		endPoint += userIDArr[i];
		sort(ratingArr + startPoint, ratingArr + endPoint, myCompare);
		for (; j < userIDArr[i] * 0.8; j++){

			ratingMatrix[i][ratingArr[k].movieID - 1] = ratingArr[k].rating;
			cout << "userID: " << i << "rating: " << ratingMatrix[ratingArr[k].userID - 1][ratingArr[k].movieID - 1];
			timeStampMatrix[i][ratingArr[k].movieID - 1] = ratingArr[k].timeStamp;
			cout << " time stamp:" << timeStampMatrix[ratingArr[k].userID - 1][ratingArr[k].movieID - 1] << endl;
			k++;
		}
		for (; j < userIDArr[i]; j++){
			k++;
		}
	}

}

int getMovieRating(int movieID){
	int total = 0;
	int count = 0;
	for (int i = 0; i < 6040; i++){
		if (ratingMatrix[i][movieID] != -1){
			total += ratingMatrix[i][movieID];
			count++;
		}
		
	}
	return total / count;
}

void initializeMovieArr(){
	ifstream fin;
	string line;
	fin.open("movies.dat");
	int i = 0;
	while (getline(fin, line)){
		int movieID = getMovieID2(line);
		string movieTitle = getMovieTitle(line);
		vector<int> movieType = getMovieType(line);
		int year = getMovieYear(line);
		int averageRating = getMovieRating(movieID);
		movieArr[i++] = movieData(movieID, movieTitle, year, movieType, averageRating);

	}
	fin.close();
}

int getUserID2(string& s){
	int i = 0;
	int size = s.size();
	while (i < size && s[i] != ':') {
		i++;
	}
	string resultString = s.substr(0, i);
	int result = atoi(resultString.c_str());
	s = s.substr(i + 2, size - i - 1);
	return result;
}
int getGender(string& s){
	int result = atoi(s.substr(0, 1).c_str());
	s = s.substr(3,s.size()-2);
	return result;
}

int getAge(string& s){
	int i = 0;
	int size = s.size();
	while (i < size && s[i] != ':') {
		i++;
	}
	string resultString = s.substr(0, i);
	int result = atoi(resultString.c_str());
	s = s.substr(i + 2, size - i - 1);
	return result;

}

vector<double> getPreference(int userID){
	unordered_map<int, int> hash_map;
	unordered_map<int, int> ::iterator it;
	for (int i = 0; i < 3090; i++){
		if (ratingMatrix[userID][i] != -1){
			for (int j = 0l; j < movieArr[i].genre.size(); j++){
				it = hash_map.find(movieArr[i].genre[j]);
				if (it != hash_map.end()){
					it->second++;
				}
				else{
					hash_map.insert(pair<int, int>(movieArr[i].genre[j], 1));
				}
			}
		}
	}
	int total = 0;
	for (it = hash_map.begin(); it != hash_map.end(); it++){
		total += it->second;
	}
	vector<double> result;
	for (it = hash_map.begin(); it != hash_map.end(); it++){
		result.push_back(it->second / total);
	}
	return result;

}

void getAverage(int userID, double& average, double& variant){
	int total = 0;
	int count = 0;
	vector<int> ratings;
	for (int i = 0; i < 3900; i++){
		
		if (ratingMatrix[userID][i] != -1){
			total += ratingMatrix[userID][i];
			ratings.push_back(ratingMatrix[userID][i]);
			count++;

		}
	}
	average = total / count;
	for (int i = 0; i < ratings.size(); i++){
		variant = pow((ratings[i] - average), 2);
	}

}

void initializeUserDataArr(){
	ifstream fin;
	string line;
	fin.open("user.dat");
	int i = 0;
	while (getline(fin, line)){
		int userID = getUserID2(line);
		int gender = getGender(line);
		int age = getAge(line);
		vector<double> preference = getPreference(userID);
		double average;
		double variant;
		getAverage(userID, average, variant);
		userArr[i++] = userData(userID, gender, age, preference, average, variant);

	}

}


int main()
{
	initializeRatingMatrix();
	initializeMovieArr();
	initializeUserDataArr;


	
	
	cin.get();
}
