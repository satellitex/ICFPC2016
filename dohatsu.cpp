#include<bits/stdc++.h>
#include"bun.cpp"
using namespace std;

typedef long long ll;


typedef complex<double> P;
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

double eps=1e-8;

P reflect(P a,P b,P c){
  b-=a;c-=a;
  return a+conj(c/b)*b;
}

P intersect(P a,P b,P c,P d){
  a-=d;b-=d;c-=d;
  return d+a+(b-a)*imag(a/c)/imag(a/c-b/c);
}

int ccw(P a,P b,P c, double EPS = eps){
  b-=a,c-=a,a=c*conj(b);
  if(a.imag()>EPS)return 1;//ccw
  if(a.imag()<-EPS)return -1;//cw
  if(a.real()<-EPS)return 2;//ob
  if(abs(b)+EPS<abs(c))return -2;//of
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

void printPolygon( polygon pg ,int id){
  int n=pg.size();
  for(int i=0;i<n;i++){
    P a=pg[i];
    P b=pg[ (i+1)%n ]-a;
    printf("%.8f %.8f %d ", real(a) , imag(a) ,id);
    printf("%.8f %.8f 0.0\n", real(b) , imag(b) );
  }
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
    //  printPolygon(data.pg,k);
  }

  return res;
}


typedef pair<double,P> Pair;
polygon ConvexHull(polygon t){
  vector<Pair> u;
  polygon R;
  int N=t.size(),K=0,C=0;
  for(int i=1;i<N;i++)if(t[i]<t[K])K=i;
  for(int i=0;i<N;i++)u.push_back(Pair(arg(t[i]-t[K]),t[i]));
  R.push_back(t[K]),C++;
  sort(u.begin(),u.end());
  for(int i=0;i<N;i++){
    P p=u[i].second;
    if(2<=C&&ccw(R[C-2],R[C-1],p)==0)continue;
    if(2<=C&&ccw(R[C-2],R[C-1],p)==2)continue;
    while(2<=C&&ccw(R[C-2],R[C-1],p)!=1)R.pop_back(),C--;
    R.push_back(p),C++;    
  }
  return R;
}

double stoD( string s ){
  if( s.find( "/" ) != string::npos ) {
    replace( s.begin(), s.end(), '/', ' ' );
    int b,c;
    stringstream sm(s);    
    sm >> b >> c;
    return b / (double)c;
  }

  stringstream ss;
  ss<<s;
  double res;
  ss>>res;
  return res;
}

P stoP( string s ){
  replace( s.begin(), s.end(), ',', ' ' );
  stringstream ss(s);
  string a,b;
  ss >> a >> b;
  double x = stoD( a ), y = stoD( b );
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

vector<P> input() {
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



  polygon pg, lineA,lineB;;
  pg.push_back( P(0,0) );
  pg.push_back( P(1,0) );
  pg.push_back( P(1,1) );
  pg.push_back( P(0,1) );

  vector< Data > ma ;
  ma.push_back( Data( pg, vector<S> () ) );

  int size=ps.size();

  bool update=true;
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
    }
  }

  vector< P > v;
  vector< vector<P> > mat = printPaper(ma);
  vector< vector<int> > id;
  id.resize( mat.size() );
  for(int i=0;i<(int)mat.size();i++){
    id[i].resize(mat[i].size());
    for(int j=0;j<(int)mat[i].size();j++){
           
      bool flg=false;
      for(int k=0;k<(int)v.size();k++){
        if( abs(v[k]-mat[i][j]) < eps ){
          id[i][j]=k;
          flg=true;
          break;
        }
      }

      if(!flg){
        id[i][j]=v.size();
        v.push_back(mat[i][j]);
      }
    }
  }

  cout<<v.size()<<endl;
  for(int i=0;i<(int)v.size();i++){
    cout<< Bun( v[i].real() ).str() <<',';
    cout<< Bun( v[i].imag() ).str() <<endl;
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
    cout<< Bun( target.real() ).str() <<',';
    cout<< Bun( target.imag() ).str() <<endl;
  }
  
  return ps;
}

int main(){
  input();
  return 0;
}
