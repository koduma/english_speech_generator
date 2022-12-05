#include <vector>
#include <cfloat>
#include <cstdio>
#include <cstring>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <random>
#include <queue>
#include <deque>
#include <list>
#include <map>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <sstream>

using namespace std;

unordered_map<string,int>word;
unordered_map<int,string>unword;
unordered_map<int,int>prob;
multimap<int,int>words;
unordered_map<string,bool>history;

string output;

double d_rnd() {
	static mt19937 mt((int)time(0));
	uniform_real_distribution<double> dice(0.0, 1.0);
	return dice(mt);
}
int rnd(int mini, int maxi) {
	static mt19937 mt((int)time(0));
	uniform_int_distribution<int> dice(mini, maxi);
	return dice(mt);
}

struct node{
int score;
int hash;    
bool operator < (const node& n)const {//スコアが高い方が優先される
		return score < n.score;
	}
};

/*
void generate_str(int cur,int depth,unordered_map<int, bool>*v,string str){
if((*v)[cur]){output=str.erase(str.size()-1,1);return;}
(*v)[cur]=true;
auto p = words.equal_range(cur);
int counter=0;    
int sum=1;
    
vector<pair<double,int> >vec;
    
for (auto it = p.first; it != p.second; ++it) {sum+=prob[it->second];counter++;}
    
double true_sum=0;    
    
for (auto it = p.first; it != p.second; ++it) {true_sum+=(double)prob[it->second];vec.push_back(make_pair(true_sum/(double)sum,it->second));}
    
double r=d_rnd();
    
int next;
    
for(int i=0;i<counter-1;i++){
//cout<<"a="<<vec[i].first<<",b="<<vec[i+1].first<<",next="<<vec[i].second<<",r="<<r<<endl;
if(vec[i].first<r&&r<vec[i+1].first){next=vec[i].second;break;}
}
    
string add=" ";
if((*v)[next]||unword[next]==""){output=str.erase(str.size()-1,1);}
else{generate_str(next,depth+1,v,str+unword[next]+add);}
if(counter==0){output=str.erase(str.size()-1,1);}
return;
}
*/

int max_ev=-1;

void g_str2(int cur,int depth,unordered_map<int, int>*v,string str,int ev){
if((*v)[cur]>=rnd(1,1)){
    return;
}
(*v)[cur]++;
auto p = words.equal_range(cur);
int counter=0;
for (auto it = p.first; it != p.second; ++it) {
//cout<<"depth="<<depth<<endl;
//cout<<"it->second="<<unword[it->second]<<endl;
string s=unword[it->second];
int dot=(int)s.size()-1;
if(s[dot]==','){
s[dot]='.';
}    
if(s==""){
    continue;
}
if(s=="."){
    if(ev>=INT_MAX/2){
    ev=INT_MAX;
    }
    else{
    ev*=2;
    }
    if(ev>max_ev){
        output=str+".";
        max_ev=ev;
    }
    continue;
}
if(s[dot]=='.'){
    if(ev>=INT_MAX/2){
    ev=INT_MAX;
    }
    else{
    ev*=2;
    }
    if(ev>max_ev){
        output=str+" "+s;
        max_ev=ev;
    }
    continue;
}    
g_str2(it->second,depth+1,v,str+" "+unword[it->second],ev+prob[it->second]);
//else{g_str2(it->second,depth+1,v,str+" "+unword[it->second],depth);}    
}
return;
}

int main(){
	
	string line;
	vector<string> t_path;
	ifstream myfile ("paper3.txt");
	while(getline(myfile,line)){

	t_path.push_back(line);

	}
	myfile.close();
	
	int counter=0;

	for(int i=0;i<(int)t_path.size();i++){
	
        bool space=false;
        string www="";
        for(int j=0;j<(int)t_path[i].size();j++){
        
        char w=t_path[i][j];
            
        if(w==' '||w=='\n'){
        counter++;
        if(www!=" "&&word[www]==0){
        word[www]=counter;
        unword[counter]=www;
        }
        www="";
        }
        else{
        www+=w;
        }
            
        }
	}
	
	for(int i=0;i<(int)t_path.size();i++){
	
        bool space=false;
        string www="";
        vector<int>vec;
            
        for(int j=0;j<(int)t_path[i].size();j++){
            
        char w=t_path[i][j];
            
        if(w==' '||w=='\n'){
        vec.push_back(word[www]);
        prob[word[www]]++;
        www="";
        }
        else{
        www+=w;
        }
           
        }
        for(int r=0;r<(int)vec.size()-1;r++){    
        int cur=vec[r];
		int nexthash=vec[r+1];
		bool find=false;
		auto p = words.equal_range(cur);
		for (auto it = p.first; it != p.second; ++it) {
		if(it->second==nexthash){find=true;break;}
		}
		if(!find){
		words.emplace(cur,nexthash);
		}
        }
        
	}
	
	string rrr;
	int cur;
	cin>>rrr;
	cur=word[rrr];
	unordered_map<int, int>v2;
	g_str2(cur,0,&v2,rrr,0);
	cout<<"ex:"<<output<<endl;
	return 0;
}
