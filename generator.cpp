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

#define TURN 15  

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
int cur;
unordered_map<int, int>v;
string str;
int ev;
bool dot;
int length;    
};
int max_ev=-1;

int vvvv=0;

int k=10;

void g_str2(int cur,int depth,unordered_map<int, int>v,string str,int ev){
if(depth>=15){return;}    
vvvv++;
if(vvvv%1000000==0){cout<<"max_ev="<<(max_ev)<<",output="<<output<<endl;}    
if((v)[cur]>=rnd(1,1)){
    return;
}
(v)[cur]++;
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
    ev+=10000;
    if(ev>max_ev){
        output=str+".";
        max_ev=ev;
    }
    continue;
}
if(s[dot]=='.'){
    ev+=10000;
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

int dot_value=0;

node BEAM_SEARCH(node n) {
    if(n.dot){return n;}
	vector<node>dque;
	dque.push_back(n);

	node bestAction=n;
    
    int maxvalue=0;

	//2手目以降をビームサーチで探索
	for (int i = 0; i < TURN; i++) {
		int ks = (int)dque.size();
		vector<node>vn;
		for (int k = 0; k < ks; k++) {
			node temp = dque[k];
			(temp.v)[temp.cur]++;
			if((temp.v)[temp.cur]>=2){continue;}
			auto p = words.equal_range(temp.cur);
			for (auto it = p.first; it != p.second; ++it) {
				node cand = temp;
				string s=unword[it->second];
				if(s==""||s==" "||(it->second==0)){cand.ev=0;}
				if(s=="."){cand.dot=true;}
				if(s[(int)s.size()-1]=='.'){cand.dot=true;}
				cand.cur=it->second;
				cand.str=cand.str+" "+unword[it->second];
				cand.ev=cand.ev+prob[it->second];
				if(cand.dot){cand.ev+=dot_value;}
				vn.push_back(cand);
			}
		}
		//printf("depth=%d/%d\n",i+1,MAX_TRN);
		dque.clear();
		vector<pair<int,int> >vec;
		bool congrats=false;
		for (int j = 0; j < (int)vn.size(); j++) {
		vec.push_back(make_pair(-vn[j].ev,j));
		}
		sort(vec.begin(),vec.end());
		int push_node=0;
		for (int j = 0; push_node < 100 ;j++) {
			if(j>=(int)vec.size()){break;}
			int x=vec[j].second;
			node temp = vn[x];
			if(temp.ev>maxvalue){
			maxvalue=temp.ev;
			bestAction=temp;
			}
			if (i < TURN-1&&!temp.dot) {
				dque.push_back(temp);
				push_node++;
			}
		}
	}
	return bestAction;
}

node BEAM_SEARCH2(node n) {
	
	vector<node>dque;
	dque.push_back(n);

	node bestAction=n;
	
	int maxvalue=0;

	//2手目以降をビームサーチで探索
	for (int i = 0;i<TURN; i++) {
		int ks = (int)dque.size();
		vector<node>vn;
		for (int k = 0; k < ks; k++) {
			node temp = dque[k];
			(temp.v)[temp.cur]++;
			if((temp.v)[temp.cur]>=2){continue;}
			auto p = words.equal_range(temp.cur);
			for (auto it = p.first; it != p.second; ++it) {
			node cand = temp;
			string s=unword[it->second];
			if(s==""||s==" "||(it->second==0)){cand.ev=0;}
			if(s=="."){cand.dot=true;}
			if(s[(int)s.size()-1]=='.'){cand.dot=true;}
			cand.cur=it->second;
			cand.str=cand.str+" "+unword[it->second];
			node n2=BEAM_SEARCH(cand);
			cand.ev=n2.ev;
			if(cand.dot){cand.ev+=100000;}
			vn.push_back(cand);
			}
		}
		printf("depth=%d/%d\n",i+1,TURN);
		dque.clear();
		vector<pair<int,int> >vec;
		for (int j = 0; j < (int)vn.size(); j++) {
		vec.push_back(make_pair(-vn[j].ev,j));
		}
		sort(vec.begin(),vec.end());
		int push_node=0;
		for (int j = 0; push_node < 10 ;j++) {
			if(j>=(int)vec.size()){break;}
			int x=vec[j].second;
			node temp = vn[x];
			if(temp.ev>maxvalue&&temp.dot){
				maxvalue=temp.ev;
				bestAction=temp;
			}
			if (i < TURN-1&&!temp.dot) {
				dque.push_back(temp);
				push_node++;
			}
		}
	}
	return bestAction;
}




int main(){
	
	string line;
	vector<string> t_path;
	ifstream myfile ("paper.txt");
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
        bool isword=true;    
        for(int z=0;z<(int)www.size();z++){
            if(www[z]==' '){isword=false;}
        }  
        if(isword){    
        word[www]=counter;
        unword[counter]=www;    
        }    
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
        if(prob[word[www]]>dot_value){dot_value=prob[word[www]]+1;}    
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
        if(!find){words.emplace(cur,nexthash);}
        }
	}
    
	string rrr;
	int cur;
	cin>>rrr;
	cur=word[rrr];
	unordered_map<int, int>v2;
	//g_str2(cur,0,v2,rrr,0);
	node n;
	n.cur=cur;
	n.v=v2;
	n.str=rrr;
	n.ev=0;
	n.dot=false;
	n=BEAM_SEARCH2(n);
	cout<<"ev="<<n.ev<<",output="<<n.str<<endl;
	return 0;
}
