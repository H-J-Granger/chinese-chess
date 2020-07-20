/*
 * @Author: BilyHurington
 * @Date: 2020-07-16 19:06:01
 * @LastEditors: BilyHurington
 * @LastEditTime: 2020-07-20 13:21:17
 */ 
#include<vector>
#include<algorithm>
#include<cmath>
#include<iostream>
#include "chessboard.h"
using namespace std;
int Count_Red[HEIGHT][WIDTH],Count_Green[HEIGHT][WIDTH];
vector<pair<int,int> > S[HEIGHT][WIDTH];
int expected_value(Map &mp){
    if(mp.status()==1) return -2.1e9;
    if(mp.status()==-1) return 2.1e9;
    memset(Count_Red,0,sizeof(Count_Red));
    memset(Count_Green,0,sizeof(Count_Green));
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            S[i][j]=mp.able_positions(i,j);
            for(auto pos:S[i][j]){
                if(mp.data_color[i][j]==red) Count_Red[pos.first][pos.second]++;
                if(mp.data_color[i][j]==green) Count_Green[pos.first][pos.second]++;
            }
        }
    }
    int ans=0;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            int val=mp.data_color[i][j]==green?-1:1;
            switch(mp.data_type[i][j]){
                case null: {val*=0;break;}
                case ju: {val*=350;break;}
                case ma: {val*=200;break;}
                case pao: {val*=210;break;}
                case shi: {val*=30;break;}
                case jiang: {val*=1000000000;break;}
                case xiang: {val*=50;break;}
                case zu: {
                    val*=35;
                    if((mp.data_color[i][j]==red&&i<=HEIGHT/2)||(mp.data_color[i][j]==green&&i>HEIGHT/2)) val*=2;
                    break;
                }
            }
            if(mp.data_color[i][j]==red){
                if(Count_Green[i][j]) val*=-1;
                if(mp.data_type[i][j]!=jiang&&Count_Red[i][j]){val*=2;val=abs(val);if(mp.data_color[i][j]==green) val*=-1;}
            }
            else{
                if(Count_Red[i][j]) val*=-1;
                if(mp.data_type[i][j]!=jiang&&Count_Green[i][j]){val*=2;val=abs(val);if(mp.data_color[i][j]==green) val*=-1;}
            }
            if(mp.data_type[i][j]==jiang){ans+=val;continue;}
            val*=(100-(mp.data_color[i][j]==red?HEIGHT-1-i:i));
            ans+=val;
        }
    }
    return ans;
}
const int max_dep=4;
struct Step{Map::Move mov;int val;};
bool operator < (Step x,Step y){return x.val<y.val;}
int Min_Max_Search(int dep=0){
    if(map.status()==-1) return -2.1e9;
    if(map.status()==1) return 2.1e9;
    if(dep>=max_dep) return expected_value(map);
    vector<Step> V;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            if((dep&1)?map.data_color[i][j]!=red:map.data_color[i][j]!=green) continue;
            vector<pair<int,int> > S=map.able_positions(i,j);
            for(auto k:S){
                map.move(i,j,k.first,k.second);
                int val=expected_value(map);
                map.undo();
                V.push_back((Step){Map::Move(i,j,k.first,k.second,map.data_type[k.first][k.second],map.data_color[k.first][k.second]),val});
            }
        }
    }
    sort(V.begin(),V.end());
    int m=min((int)V.size(),max_dep-dep);
    if(!(dep&1)) V.erase(V.begin(),V.end()-m);
    else V.erase(V.begin()+m,V.end());
    int maxn=-2.1e9,minn=2.1e9;
    for(auto step:V){
	    map.move(step.mov);
        int val=Min_Max_Search(dep+1);
	    map.undo();
        maxn=max(maxn,val);
        minn=min(minn,val);
    }
    return (!(dep&1))?minn:maxn;
}
Map::Move robot_choose(){
    vector<Step> V;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            if(map.data_color[i][j]!=red) continue;
            vector<pair<int,int> > S=map.able_positions(i,j);
            for(auto k:S){
                map.move(i,j,k.first,k.second);
                int val=expected_value(map);
                map.undo();
                V.push_back((Step){Map::Move(i,j,k.first,k.second,map.data_type[k.first][k.second],map.data_color[k.first][k.second]),val});
            }
        }
    }
    // puts("Debug");
    // printf("%d\n",map.data_color[0][7]);
    sort(V.begin(),V.end());
    int maxn=-2.1e9,val;
    Map::Move max_step;
    for(int i=0;i<V.size();i++){
        // printf("%d %d %d %d %d\n",V[i].mov.fromx,V[i].mov.fromy,V[i].mov.tox,V[i].mov.toy,map.data_color[0][7]);
        map.move(V[i].mov);
        // puts("ok");
        if((val=Min_Max_Search())>maxn){
            maxn=val;
            max_step=V[i].mov;
        } 
        map.undo();
    }   
    return max_step;
}
