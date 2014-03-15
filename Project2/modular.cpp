#include <iostream>
#include "movie.h"
using namespace std;
struct CF_candidate_t
{
	int mid;
	int score;
	int date;
};
struct movie_candidate_t
{
	int mid;
	float score;
	int year;
};

struct output_list_t
{
	int mid;
	float score;
};

int sqr(int num)
{
	return num*num;
}

int min(int a ,int b)
{
	if (a<b) 
		return a; 
	else 
		return b;
}

int cmp(const void *a, const void *b)
{
	CF_candidate_t *x,*y;
	x = (CF_candidate_t *)a;
	y = (CF_candidate_t *)b;
	
	if(x->score!=y->score)
		return x->score - y->score;
	else 
		return x->date - y->date;
}

CF_list_t * CF(int uid,int &return_size)
{
	int i,j;
	int inner_product,len_a,len_b;
	int common_movie[total_user];
	CF_candidate_t candidate_list[total_movie];
	int c_index = 0;
	int size= 500;
	int index=0;
	
	CF_list_t * output_list;
	output_list = (CF_list_t *)malloc(sizeof(CF_list_t)*size);
	for(i=0;i<total_user;i++)
	{
		if(i==uid) continue;
		inner_product = 0;
		len_a = 0;
		len_b = 0;
		common_movie[i]=0;

		for(j=0;j<total_movie;j++)
		{
			if(ratingMatrix[i][j]!=-1&&ratingMatrix[uid][j]!=-1)
			{
				inner_product += ratingMatrix[i][j]*ratingMatrix[uid][j];
				len_a += sqr(ratingMatrix[uid][j]);
				len_b += sqr(ratingMatrix[i][j]);
				common_movie[i]++;
			}
		}
		float similarity = inner_product/(sqrt(len_a)*sqrt(len_b));
		
		if(similarity>0.5)
		{
			c_index = 0;
			for(j=0;j<total_movie;j++)
			{				
				if(ratingMatrix[i][j]!=-1){
					
					candidate_list[c_index].mid = j;
					candidate_list[c_index].score = ratingMatrix[i][j];
					candidate_list[c_index].date = timeStampMatrix[i][j];
					c_index++;
				}		
			}
			
			qsort(candidate_list,c_index,sizeof(CF_candidate_t),cmp);
			for(j=0;j<min(c_index,10);j++)
			{
				if(index==size){
					size+=100;
					output_list = (CF_list_t *)realloc(output_list,sizeof(CF_list_t)*size);
				}
				output_list[index].mid = candidate_list[j].mid;
				output_list[index].uid = i;
				output_list[index].score = candidate_list[j].score;
				output_list[index].similarity = similarity;
				output_list[index].common = common_movie[i];
				index++;
			}
		}					

	}
	return_size = index;
	return output_list;
}

bool movie_cmp(movie_candidate_t a,movie_candidate_t b)
{
	return a.score<b.score;
}
void scan_and_pick(int pid,content_list_t* list, int &index, int quota,int uid)
{
	int i,j;
	vector<movie_candidate_t> movie_list;
	for(i=0;i<total_movie;i++)
	{
		for(j=0;j<movieArr[i].genre.size();j++)
			if(movieArr[i].genre[j]==pid){
				movie_candidate_t tmp;
				tmp.mid = i;
				tmp.score = movieArr[i].rating;
				if(ratingMatrix[uid][i]==-1)
				movie_list.push_back(tmp);
				break;
			}
	}	
	sort(movie_list.begin(),movie_list.end(),movie_cmp);
	for(j=0;j<min(quota,movie_list.size());j++)
		{
			if(index>=output_size*5){

				break;
			}
			list[index].mid = movie_list[j].mid;
			list[index].score = movie_list[j].score;
			list[index].preference = movieArr[j].genre;
			index++;
		}
		
	
}

content_list_t * content_filter(int uid,int &size)
{
	content_list_t* output_list;
	output_list = (content_list_t *)malloc(sizeof(content_list_t)*5*output_size);
	int index=0;
	vector<double> preference = userArr[uid].preference;

	int i;
	int quota;
	for(i=0;i<preference.size();i++){

		quota = ceil(5*output_size*preference[i]);
		scan_and_pick(i,output_list,index,quota,uid);
	}
	size = index;
	return output_list;
}

bool compOutput(output_list_t a,output_list_t b)
{
	return a.score<b.score;
}


void filter(CF_list_t * CF_list, int CF_len, content_list_t * content_list, int content_len, int n)
{
	output_list_t * output_list;
	output_list = (output_list_t *)malloc(sizeof(output_list_t)*output_size);
	int rec_count[total_movie];
	memset(rec_count,0,sizeof(rec_count));
		
	float lambda = 0.8;
	map<int,float> movieScore;
	int i;

	int mid;
	CF_list_t tmp;
	for(i=0;i<CF_len;i++)
	{
		mid = CF_list[i].mid;
		tmp = CF_list[i];
		if(movieScore.count(mid)==0)
		{
			cout<<userArr[n].total<<endl;
			cout<<userArr[tmp.uid].variant<<endl;
			rec_count[mid]++;
			movieScore[mid]=lambda*tmp.similarity*(tmp.common/userArr[n].total)*(tmp.score-userArr[tmp.uid].average/sqrt(userArr[tmp.uid].variant))*(1.0/rec_count[mid]);
		}
		else
		{
			cout<<userArr[n].total<<endl;
			cout<<userArr[tmp.uid].variant<<endl;
			rec_count[mid]++;
			movieScore[mid]+=lambda*tmp.similarity*(tmp.common/userArr[n].total)*(tmp.score-userArr[tmp.uid].average/sqrt(userArr[tmp.uid].variant))*(1.0/rec_count[mid]);
		}
	}
	
	float preference;
	for(i=0;i<content_len;i++)
	{
		preference = 0;
		mid = content_list[i].mid;
		int j;
		int pid;

		for(j=0;j<content_list[i].preference.size();j++)
		{
			pid = content_list[i].preference[j];
			preference += userArr[n].preference[pid];	
		}
		if(movieScore.count(mid)==0)
		{
			movieScore[mid]=(1-lambda)*content_list[i].score*preference;
		}
	}

	map<int,float>::iterator it;
	vector<output_list_t> output;
	for(it=movieScore.begin();it!=movieScore.end();i++)
	{
		output_list_t outputNode;
		outputNode.mid = it->first;
		outputNode.score = it->second;

		output.push_back(outputNode);
	}
	sort(output.begin(),output.end(),compOutput);

	cout<<n<<endl;
	for(i=0;i<output_size;i++) cout<<output[i].mid<<' ';
	cout<<endl;	
	

	return;
}
