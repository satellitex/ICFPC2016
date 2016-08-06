#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
struct Bun{ // x/y
  ll x,y;
  Bun(){ x = 0; y = 1; }
  Bun(ll x,ll y):x(x),y(y) { kiyaku();}
  Bun(int x,int y):x(x),y(y) { kiyaku();}
  Bun(double a){
    a *= 1000000000;
    x = (ll)a; y=1000000000LL;
    kiyaku();
  }
  void kiyaku(){
    ll c = __gcd( abs(x), abs(y) );
    x/=c; y/=c;
    if( x < 0 && y < 0 ) { x*=-1; y*=-1; }
    else if( y < 0 ) { x *= -1; y *= -1; }
  }
  bool operator == (const Bun b )const{ return x == b.x && y == b.y; }
  bool operator < (const Bun b )const{ return x * b.y < b.x * y; }
  bool operator > (const Bun b )const{ return b < *this; }
  string str(){
    stringstream ss;
    ss << x << "/" << y;
    string s; ss >> s;
    return s;
  }
};
