#include<bits/stdc++.h>
using namespace std;

#include <cmath>
#include <complex>
#include <vector>
using namespace std;

typedef complex<double> P; //Point
typedef pair<P,P> L; //Line, Segment

const double EPS = 1e-8;
const double PI = 6.0 * asin(0.5);

namespace std {
  bool operator < (const P& a, const P& b){
    return fabs(real(a)-real(b)) < EPS ? imag(a) < imag(b) : real(a) < real(b);
  }
  bool operator == (const P& a, const P& b) {
    return abs( a - b ) < EPS;
  }
}

/*
  ベクトルの内積
  a.real() * b.real() + a.imag() * b.imag()
*/
double dot(P a, P b){ return a.real() * b.real() + a.imag() * b.imag(); }


/*
  ベクトルの外積
  a.real() * b.imag() - a.imag() * b.real()
*/
double cross(P a, P b){ return   a.real() * b.imag() - a.imag() * b.real(); }


/*
  多角形の面積
  v : それぞれの頂点の座標
  プラマイが逆になる？
*/
double area(vector<P> v){
  double sum = 0.0;
  int n = v.size();
  for(int i=0;i<n;i++) sum += (real(v[i]) - real(v[(i+1)%n])) * (imag(v[i]) + imag(v[(i+1)%n]));
  return fabs(sum) / 2;
};



// へロンの公式（三角形の3辺の長さから面積を求める）
double heron(double a, double b, double c){
  double s = (a + b + c) / 2;
  return sqrt(s * (s-a) * (s-b) * (s-c));
}


// 回転行列
P rotate(P p, double theta){
  theta = theta * M_PI / 180.0;
  double x = real(p) * cos(theta) - imag(p) * sin(theta);
  double y = real(p) * sin(theta) + imag(p) * cos(theta);
  return P(x,y);
}


// 射影
P proj(P p, L l){
    return l.first + dot(p - l.first, l.second - l.first) / norm(l.second - l.first) * (l.second - l.first);
}

// 反射（線対称な点）
P reflect(P p, L l){
  return p + (proj(p,l) - p) * 2.0;
}


//  a -> b -> c に進む時　の b -> c の向き
int ccw(P a, P b, P c){
  b -= a;
  c -= a;
  if(cross(b,c) > EPS) return 1; //反時計回り
  if(cross(b,c) < -EPS) return -1; // 時計回り
  if(dot(b,c) < -EPS) return 2; // c -- a -- b の一直線
  if(norm(b) < norm(c)) return -2; // a -- b -- c の一直線
  return 0; // a -- c -- b の一直線?
}


// 線分と線分の交差判定（交差していたらtrue）
bool isIntersect(L s1, L s2){
  
  //ccw による判定だけだと, 二線分が同一直線上にあるときに間違うため、追加
  if(max(real(s1.first), real(s1.second)) + EPS < min(real(s2.first), real(s2.second)) ||
     max(imag(s1.first), imag(s1.second)) + EPS < min(imag(s2.first), imag(s2.second)) ||
     max(real(s2.first), real(s2.second)) + EPS < min(real(s1.first), real(s1.second)) ||
     max(imag(s2.first), imag(s2.second)) + EPS < min(imag(s1.first), imag(s1.second))) return false;

  return ( ccw(s1.first,s1.second,s2.first) * ccw(s1.first,s1.second,s2.second) <= 0 &&
           ccw(s2.first,s2.second,s1.first) * ccw(s2.first,s2.second,s1.second) <= 0 );
}

// 直線と線分の交差判定
bool isIntersectLS(L l, L s){
  return cross(l.second-l.first, s.first-l.first) * cross(l.second-l.first, s.second-l.first) < EPS;
}


// 線分と点の交差判定
bool isIntersectSP(L s, P p){ 
  return abs(s.first-p) + abs(s.second-p) - abs(s.second-s.first) < EPS; 
}


// 線分と点の距離
double getDistanceSP(L s, P p){
  if(dot(s.second - s.first, p - s.first) < -EPS) return abs(p - s.first);
  if(dot(s.first - s.second, p - s.second) < -EPS) return abs(p - s.second);
  return abs(cross(s.second - s.first, p - s.first) / abs(s.second - s.first));  //直線と点の距離
}


// 線分と線分の距離
double getDistanceSS(L s1, L s2){
  if(isIntersect(s1,s2)) return 0.0;
  return min( min(getDistanceSP(s1, s2.first), getDistanceSP(s1, s2.second)),
              min(getDistanceSP(s2, s1.first), getDistanceSP(s2, s1.second)));
}

// ２直線間の交点
P crossPoint(L l, L m){
  double A = cross(l.second - l.first, m.second - m.first);
  double B = cross(l.second - l.first, l.second - m.first);
  if(fabs(A) < EPS && fabs(B) < EPS) return m.first;
  else if(fabs(A) >= EPS) return m.first + B / A * (m.second - m.first);
}


//spaghetti source版 凸包
vector<P> convex_hull(vector<P> ps) {
  int n = ps.size(), k = 0;
  sort(ps.begin(), ps.end());
  vector<P> ch(2*n);
  for (int i = 0; i < n; ch[k++] = ps[i++]) // lower-hull
    while (k >= 2 && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
  for (int i = n-2, t = k+1; i >= 0; ch[k++] = ps[i--]) // upper-hull
    while (k >= t && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
  ch.resize(k-1);
  return ch;
}


// 凸多角形かどうか
bool isConvex(vector<P> v){
  int n = v.size();
  for(int i=0;i<n;i++)
    if(ccw(v[(i+n-1)%n], v[i], v[(i+1)%n]) > 0) return false;
  return true;
}



//すべて反時計で持つ（そのほうが都合がいいため
typedef vector<int> Region; //表面(半時計回りで座標のindexを持つ)
class Regions{
  //多角形[index] = 座標
  vector<P> pos;//元の座標  
  vector<P> tos;//折ったあとの座標
  list<Region> R;

  
  int addPostion( Region &r, int now, int nex, P p ){
    int a = r[now], b = r[nex];
    if( tos[a] == p ) return a;
    if( tos[b] == p ) return b;
    int id = (int)pos.size();
    tos.push_back( p );
    P v = pos[b]-pos[a]; double d = abs( tos[a]-p );
    v /= abs(v);
    pos.push_back( pos[a] + v * d );
    r.insert( r.begin() + nex, id );
    return id;
  }

  // a->b
  vector<Region> cutRegion(Region &r, int a, int b ){
    int f = 0;
    vector<Region> ret;
    Region sr;
    for(int i=0;ret.size()<2;i++){
      int id = (i%(int)r.size());
      if( r[id] == a || r[id] == b ){
        if( f == 0 ) f = 1;
        else if( f == 1 ) {
          sr.push_back( r[id] ); ret.push_back( sr );
          sr.clear();
        }
      }
      if( f == 1 ) sr.push_back( r[id] );
    }
    return ret;
  }

public:

  Regions() {
    int dx[]={0,1,1,0};
    int dy[]={0,0,1,1};
    Region r;
    for(int i=0;i<4;i++){
      tos.push_back( P( dx[i], dy[i] ) );
      pos.push_back( P( dx[i], dy[i] ) );
      r.push_back( i );
    }
    R.push_back( r );
  }
  
  // l.first -> l.second に対し左側をそのままにして折る。
  void chipPaper( L l ){
    set<P> s;
    vector<Region> nwr;
    vector< list<Region>::iterator > rmvs;
    for( auto it = R.begin(); it != R.end(); it ++ ){
      vector<int> vs;
      Region &r = *it;
      for(int now =0;now<(int)it->size();now++) {
        int nex = (now+1)%(int)r.size();
        L s = L( tos[r[now]], tos[r[nex]] );
        if( isIntersectLS( l, s ) ) {//直線lと線分sの交差判定
          P p = crossPoint( l, s );
          vs.push_back( addPostion( r, now, nex, p ) );
        }
      }
      vs.erase( unique( vs.begin(), vs.end() ), vs.end() );
      if( vs.size() == 2 )  {
        vector<Region> vr = cutRegion( r, vs[0], vs[1] );
        nwr.insert( nwr.begin(), vr.begin(), vr.end() );
        rmvs.push_back( it );
      }
      assert( vs.size() <= 2 );
    }

    //分割した猟奇を削除
    for( auto rm : rmvs )
      R.erase( rm );
    //新たな領域を追加
    for( Region r : nwr )
      R.push_back( r );

    //右側の点を反射
    for( P &p : tos )
      if( ccw( l.first, l.second, p ) == -1 )
        p = reflect( p, l );
  }

  // l.first->l.second
  bool isCut( L l ){
    for( P p : tos ){
      if( ccw( l.first, l.second, p ) == -1 )
        return true;
    }
    return false;
  }

  void output(){
    cout << tos.size() << endl;
    for(P t : tos ) cout << t << endl;
    cout << R.size() << endl;
    for(Region r : R ){
      for( int x : r ) cout << x << " ";
      cout << endl;
    }
    for(P p : pos ) cout << p << endl;
  }
 
};


double stoD( string s ){
  if( s.find( "/" ) != string::npos ) {
    replace( s.begin(), s.end(), '/', ' ' );
    int b,c;
    stringstream sm(s);    
    sm >> b >> c;
    return b / (double)c;
  }
  return stoi(s);
}

P stoP( string s ){
  replace( s.begin(), s.end(), ',', ' ' );
  stringstream ss(s);
  string a,b;
  ss >> a >> b;
  double x = stoD( a ), y = stoD( b );
  return P(x,y);
}

vector<P> input() {
  int T,N;
  vector<P> ps;
  for(int t=0;t<T;t++){
    cin >> N;
    for(int i=0;i<N;i++){
      string s;
      cin >> s;
      ps.push_back( stoP( s ) );
    }
    cin >> N;
    for(int i=0;i<N;i++){
      string s;
      cin >> s;
      ps.push_back( stoP( s ) );
      cin >> s;
      ps.push_back( stoP( s ) );
    }    
  }

  sort( ps.begin(), ps.end() );
  ps.erase( unique( ps.begin(), ps.end() ), ps.end() );

 
  ps = convex_hull( ps );

  for( P p : ps ) cout << p << endl;
  cout << "ok" << endl;
  return ps;
}

int main(){

  /*
  vector<P> ps = input();
  Regions regions;
  bool f = true;
  while( f ){
    f = false;
    for(int i=0;i<(int)ps.size();i++){
      P now = ps[i], nex = ps[(i+1)%ps.size()];
      L l = L( now, nex );
      if( regions.isCut( l ) ){
        regions.chipPaper( l ); f = true;
        cout << "cut " << l.first << " -> " << l.second << endl;
        regions.output();
      }            
    }
  }
  */

  int Q; cin >> Q;
  Regions regions;
  for(int i=0;i<Q;i++){    
    double a,b,c,d; cin >> a >> b >> c >> d;    
    L l = L( P(a,b), P(c,d) );
    cout << "cut :" << l.second << "->" << l.first<< endl;
    regions.chipPaper( l );
    regions.output();
  }
  

            
}
