//LUOGU 3376
#include<cstdio>
#include<queue>
using namespace std;
const int maxn=10000,inf=0x3f3f3f3f;
struct edge{
	int to,cap,flow,rev;
	edge(int to,int cap,int flow,int rev):
		to(to),cap(cap),flow(flow),rev(rev){}
};
vector<edge> g[maxn+10];
int n,m;
void addedge(int from,int to,int cap){
	g[from].push_back(edge(to,cap,0,g[to].size()));
	g[to].push_back(edge(from,0,0,g[from].size()-1));
}
namespace doisap{
	int s,t,dis[maxn+10],gap[maxn+10],pn[maxn+10],pe[maxn+10],cur[maxn+10];
	queue<int> que;
	void bfs(){
		fill(dis+1,dis+n+1,inf); dis[t]=0; que.push(t);
		while(!que.empty()){
			int x=que.front(); que.pop();
			++gap[dis[x]];
			for(int i=0;i<g[x].size();++i){
				edge e=g[x][i];
				if(g[e.to][e.rev].cap>g[e.to][e.rev].flow&&dis[e.to]==inf){
					dis[e.to]=dis[x]+1; que.push(e.to);
				}
			}
		}
	}
	int trace(){
		int now=inf;
		for(int i=t;i!=s;i=pn[i])
			now=min(now,g[pn[i]][pe[i]].cap-g[pn[i]][pe[i]].flow);
		for(int i=t;i!=s;i=pn[i]){
			g[pn[i]][pe[i]].flow+=now;
			g[i][g[pn[i]][pe[i]].rev].flow-=now;
		}
		return now;
	}
	int isap(){
		bfs(); int x=s,ans=0;
		while(dis[s]<inf){
			if(x==t){
				ans+=trace(); x=s;
			}
			bool suc=0;
			for(int &i=cur[x];i<g[x].size();++i){
				edge e=g[x][i];
				if(e.cap>e.flow&&dis[x]==dis[e.to]+1){
					pn[e.to]=x; pe[e.to]=i; x=e.to;
					suc=1; break;
				}
			}
			if(!suc){
				if(!--gap[dis[x]]) break;
				dis[x]=inf;
				for(int i=0;i<g[x].size();++i){
					edge e=g[x][i];
					if(e.cap>e.flow) dis[x]=min(dis[x],dis[e.to]+1);
				}
				if(dis[x]<inf) ++gap[dis[x]];
				cur[x]=0; if(x!=s) x=pn[x];
			}
		}
		return ans;
	}
}
int main(){
	scanf("%d%d%d%d",&n,&m,&doisap::s,&doisap::t);
	for(int i=1;i<=m;++i){
		int from,to,cap; scanf("%d%d%d",&from,&to,&cap);
		addedge(from,to,cap);
	}
	printf("%d",doisap::isap()); return 0;
}
