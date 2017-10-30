//LUOGU 3381
#include<cstdio>
#include<queue>
using namespace std;
const int maxn=5000,inf=0x3f3f3f3f;
int n,m,s,t;
struct edge{
	int to,cap,flow,cost,rev;
	edge(int to,int cap,int flow,int cost,int rev):
		to(to),cap(cap),flow(flow),cost(cost),rev(rev){}
};
vector<edge> g[maxn+10];
void addedge(int from,int to,int cap,int cost){
	g[from].push_back(edge(to,cap,0,cost,g[to].size()));
	g[to].push_back(edge(from,0,0,-cost,g[from].size()-1));
}
int h[maxn+10],dis[maxn+10]; bool inq[maxn+10];
queue<int> que;
struct heapnode{
	int id,v;
	heapnode(int id,int v):
		id(id),v(v){}
	bool operator<(const heapnode &t)const{
		return v>t.v;
	}
};
priority_queue<heapnode> heap; int pn[maxn+10],pe[maxn+10];
void bellman_ford(){
	fill(h+1,h+n+1,inf); h[s]=0; inq[s]=1;
	que.push(s);
	while(!que.empty()){
		int x=que.front(); que.pop(); inq[x]=0;
		for(int i=0;i<g[x].size();++i){
			edge e=g[x][i];
			if(e.flow<e.cap&&h[x]+e.cost<h[e.to]){
				h[e.to]=h[x]+e.cost;
				if(!inq[e.to]){
					inq[e.to]=1; que.push(e.to);
				}
			}
		}
	}
}
bool dijkstra(){
	fill(dis+1,dis+n+1,inf); dis[s]=0;
	heap.push(heapnode(s,0));
	while(!heap.empty()){
		heapnode x=heap.top(); heap.pop();
		if(x.v>dis[x.id]) continue;
		for(int i=0;i<g[x.id].size();++i){
			edge e=g[x.id][i];
			if(e.flow<e.cap&&x.v+h[x.id]+e.cost-h[e.to]<dis[e.to]){
				dis[e.to]=x.v+h[x.id]+e.cost-h[e.to];
				pn[e.to]=x.id; pe[e.to]=i;
				heap.push(heapnode(e.to,dis[e.to]));
			}
		}
	}
	return dis[t]!=inf;
}
void mincostmaxflow(){
	bellman_ford(); int maxflow=0,mincost=0;
	while(dijkstra()){
		int now=inf;
		for(int i=t;i!=s;i=pn[i])
			now=min(now,g[pn[i]][pe[i]].cap-g[pn[i]][pe[i]].flow);
		maxflow+=now; mincost+=now*(dis[t]+h[t]-h[s]);
		for(int i=t;i!=s;i=pn[i]){
			g[pn[i]][pe[i]].flow+=now;
			g[i][g[pn[i]][pe[i]].rev].flow-=now;
		}
		for(int i=1;i<=n;++i) h[i]=min(h[i]+dis[i],inf);
	}
	printf("%d %d",maxflow,mincost);
}
int main(){
	scanf("%d%d%d%d",&n,&m,&s,&t);
	for(int i=1;i<=m;++i){
		int from,to,cap,cost; scanf("%d%d%d%d",&from,&to,&cap,&cost);
		addedge(from,to,cap,cost);
	}
	mincostmaxflow(); return 0;
}
