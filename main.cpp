#include<bits/stdc++.h>
#include "bun.h"
using namespace std;


typedef complex< Bun > P;
typedef vector< P > polygon;

namespace std{
  bool operator < (const P &a,const P &b){
    return (a.imag()==b.imag()?
            a.real()<b.real():
            a.imag()<b.imag());
  }
};


struct S{
  P p0,p1;
  S( P a , P b ){
    p0=a;
    p1=b;
  }
};


struct Data{
  polygon pg;
  vector< S > re;
  Data( polygon a, vector< S > b){
    pg=a;
    re=b;
  }
};

P reflect(P a,P b,P c){
  b=b-a;
  c=c-a;
  return a+conj(c/b)*b;
}

P intersect(P a,P b,P c,P d){
  a-=d;b-=d;c-=d;
  return d+a+(b-a)*imag(a/c)/imag(a/c-b/c);
}

int ccw(P a,P b,P c){
  b-=a,c-=a,a=c*conj(b);
  if(a.imag().x > 0)return 1;//ccw
  if(a.imag().x < 0)return -1;//cw
  if(a.real().x < 0)return 2;//ob
  if(norm(b)<norm(c))return -2;//of
  return 0;//os
}

polygon cutConvex(polygon &t,P a,P b){
  int N=t.size();
  polygon res;
  for(int i=0;i<N;i++){
    P c=t[i],d=t[(i+1)%N];
    int C=ccw(a,b,c),D=ccw(a,b,d);
    if(C!=-1)res.push_back(c);
    if(C+D==0&&max(C,D)==1)res.push_back(intersect(a,b,c,d));
  }
  if(res.size()<3)return polygon();
  return res;
}

vector< Data > cutPaper( vector<Data> d, S line ){
  vector< Data > res;
  int n=d.size();
  for(int i=0;i<n;i++){
    Data data=d[i];
    polygon a= cutConvex( data.pg , line.p0, line.p1 );
    polygon b= cutConvex( data.pg , line.p1, line.p0 );
    if( b.size() > 0 ){
      res.push_back( Data(b,data.re) );
    }
    if( a.size() == 0 )continue;
    for(int j=0;j<(int)a.size();j++){
      a[j]=reflect(line.p0,line.p1,a[j]);
    }
    
    reverse(a.begin(),a.end());
    data.pg=a;
    data.re.push_back(line);
    res.push_back(data);
  }
  return res;
}

vector< vector< P > >  printPaper( vector<Data> d ){
  vector< vector< P > > res;
  for(int k=0;k<(int)d.size();k++){
    Data data=d[k];
    for(int i=(int)data.re.size()-1;i>=0;i--){
      S line=data.re[i];
      for(int j=0;j<(int)data.pg.size();j++){
        data.pg[j]=reflect( line.p0, line.p1 ,data.pg[j]);
      }
      reverse(data.pg.begin(),data.pg.end());
    }
    res.push_back( data.pg );
  }

  return res;
}

polygon andrewScan(polygon &t){
  int N=t.size(),C=0;
  polygon R(N);
  for(int i=0;i<N;i++){
    while(2<=C&&ccw(R[C-2],R[C-1],t[i])==-1)C--;
    R[C++]=t[i];
  }
  polygon res(C);
  for(int i=0;i<C;i++)res[i]=R[i];
  return res;
}
 
polygon ConvexHull(polygon &t){
  sort(t.begin(),t.end());
  polygon u=andrewScan(t);
  reverse(t.begin(),t.end());
  polygon l=andrewScan(t);
  for(int i=1;i+1<(int)l.size();i++)u.push_back(l[i]);
  return u;
}

Bun stoB( string s ){
  if( s.find( "/" ) != string::npos ) {
    replace( s.begin(), s.end(), '/', ' ' );
    ll b,c;
    stringstream sm(s);    
    sm >> b >> c;
    return  Bun(b,c);
  }

  stringstream ss;
  ss<<s;
  ll res;
  ss>>res;
  return Bun(res,1);
}

P stoP( string s ){
  replace( s.begin(), s.end(), ',', ' ' );
  stringstream ss(s);
  string a,b;
  ss >> a >> b;
  Bun x = stoB( a ), y = stoB( b );
  return P(x,y);
}


bool isCut(vector<Data> &mp,P a,P b){
  for(Data d : mp ){
    for( P p : d.pg ){
      if( ccw( a, b, p ) == 1 )return true;
    }
  }
  return false;
}


Bun change(Bun a){
  if(a.x > 0){
    return Bun(a.x/a.y,1);
  }else{
    ll ax=-a.x;
    return Bun( -( (ax+a.y-1) /a.y  ) , 1 ) ;
  }
  return a;
}

int main(){

  int T,N;
  vector<P> ps;
  cin>>T;
  for(int t=0;t<T;t++){
    cin >> N;
    for(int i=0;i<N;i++){
      string s;
      cin >> s;
      ps.push_back( stoP( s ) );
    }
  }
  
  cin >> N;
  for(int i=0;i<N;i++){
    string s;
    cin >> s;
    ps.push_back( stoP( s ) );
    cin >> s;
    ps.push_back( stoP( s ) );
  }    
  

  
  sort( ps.begin(), ps.end() );
  ps.erase( unique( ps.begin(), ps.end() ), ps.end() );

  ps = ConvexHull( ps );

  Bun ax = ps[0].real();
  Bun ay = ps[0].imag();

  for(int i=0;i<(int)ps.size();i++){
    ax = min ( ax, ps[i].real() );
    ay = min ( ay, ps[i].imag() );
  }

  Bun nx = change(ax);
  Bun ny = change(ay);
  
  //  cerr<<nx<<' '<<ny<<endl;
  
  polygon pg, lineA,lineB;;
  vector<S> sre;
  pg.push_back( P( nx+Bun(0,1) , ny+Bun(0,1) ) );
  pg.push_back( P( nx+Bun(1,1) , ny+Bun(0,1) ) );
  pg.push_back( P( nx+Bun(1,1) , ny+Bun(1,1) ) );
  pg.push_back( P( nx+Bun(0,1) , ny+Bun(1,1) ) );


  /*
  if( nx.x %2 != 0 ){
    sre.push_back( S(pg[0],pg[3]) );
  }

  if( ny.y %2 != 0 ){
    sre.push_back( S(pg[1],pg[0]) );
  }
  */

  vector< Data > ma ;
  ma.push_back( Data( pg, sre ) );

  int size=ps.size();

  bool update=true;
  int cnt = 0;
  while(update){
    update=false;
    for(int i=0;i<size;i++){
      P b=ps[i];
      P a=ps[(i+1)%size];

      if( !isCut(ma,a,b) )continue;
      lineA.push_back(a);
      lineB.push_back(b);
      ma = cutPaper( ma, S(a,b) );
      update=true;
      if( ma.size() > 20 ) break;
    }
    if( ma.size() > 20 ) break;
    cnt++;
    if( cnt > 20 ) break;
  }


  int count=0;
  map< P , int > mp;
  vector< vector<P> > mat = printPaper(ma);
  vector< vector<int> > id;
  
  id.resize( mat.size() );
  
  for(int i=0;i<(int)mat.size();i++){
    
    id[i].resize(mat[i].size());
    
    for(int j=0;j<(int)mat[i].size();j++){
      
      if(mp.count( mat[i][j] ) == 0 ){
        id[i][j] = count;
        mp[ mat[i][j] ] = count++;
      }else{
        id[i][j] = mp[ mat[i][j] ];
      }
      
    }
  }

  cout<<mp.size()<<endl;
  vector< P > v(mp.size());
  for( pair< P , int > p : mp ){
    v[ p.second ] = p.first;
  }
  
  for(int i=0;i<(int)v.size();i++){

    P tmp=v[i];
    tmp -= P(nx,ny);
    cout<< tmp.real().str() <<',';
    cout<< tmp.imag().str() <<endl;
  }

  cout<<mat.size()<<endl;

  for(int i=0;i<mat.size();i++){
    cout<<mat[i].size();
    for(int j=0;j<mat[i].size();j++){
      cout<<' '<<id[i][j];
    }
    cout<<endl;
  }

  
  for(int i=0;i<(int)v.size();i++){
    P target=v[i];
    for(int j=0;j<(int)lineA.size();j++){
      P a=lineA[j];
      P b=lineB[j];
      if(ccw(a,b,target)==1)target=reflect(a,b,target);
    }
    cout<< target.real().str() <<',';
    cout<< target.imag().str() <<endl;
  }
  
  return 0;
}

