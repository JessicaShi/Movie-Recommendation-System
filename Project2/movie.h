#ifndef _MOVIE_H
#define _MOVIE_H

#include <iostream>
#include <fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<unordered_map>
#include<cmath>
#include<cstdlib>
#include <map>
#include <cstring>
using namespace std;


struct ratings{
        ratings(){}
        ratings(int m_userID, int m_movieID, int m_rating, int m_timestamp):userID(m_userID), movieID(m_movieID), rating(m_rating), timeStamp(m_timestamp){}
        int userID;
        int movieID;
        int rating;
        int timeStamp;
};

extern unordered_map<string, int> MovieGenreMap;


struct userData{
        userData(){};
        userData(int m_userID, int m_age, int m_gender, vector<double>& m_preference, double m_average, double m_variant) :userID(m_userID), age(m_age), gender(m_gender), preference(m_preference),
        average(m_average), variant(m_variant){}

        int userID;
        int age;
        int gender;        //0 is male, 1 is female;
        vector<double> preference;
        double average;
        double variant;
	int total;
};

struct movieData{
        movieData(){};
        movieData(int m_movieID, string m_title, int m_year, vector<int>& m_genre, double m_rating) :movieID(m_movieID), genre(m_genre), title(m_title), year(m_year), rating(m_rating){}
        int movieID;
        vector<int> genre;
        string title;
        int year;
        double rating;
};

struct CF_list_t
{
        int mid;
        int uid;
        int score;
        float similarity;
        int common;
};

struct content_list_t
{
        int mid;
        int score;
        vector<int> preference;
};


extern const int ROW;
extern const int COL;
extern int ratingMatrix[6040][3900];  //sotores which user watches which movie and the movie rating from the user;
extern int timeStampMatrix[6040][3900];
extern ratings ratingArr[1000209];  //helper array
extern movieData movieArr[3900];   //store all the 3900 movies information
extern userData userArr[6040];
extern int userIDArr[6040];       //store the number of movies each user watched
extern int ratingMatrixPrediction[6040][3900]; 

const int total_user		= 6040;
const int total_movie		= 3900;
const int total_rating		= 1000209;
const int total_preference	= 18;
const int output_size		= 10;

content_list_t * content_filter(int uid,int &size);
CF_list_t * CF(int uid,int &return_size);
void filter(CF_list_t * CF_list, int CF_len, content_list_t * content_list, int content_len, int n);



#endif
