#include<bits/stdc++.h>
#include<boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
typedef mp::cpp_int ll;
using namespace std;


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
    ll c = gcd( abs(x), abs(y) );
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
  
  Bun operator + (const Bun& b) const {
    ll t = lcm( y, b.y );    
    ll a = x * ( t / y );
    ll c = b.x * ( t / b.y );  
    return Bun( a + c , t );
  }
  Bun& operator += (const Bun& b ) {
    *this = *this + b;
    return *this;
  }
  
  Bun operator - (const Bun& b) const {
    return *this + Bun( -b.x, b.y );
  }
  Bun& operator -= (const Bun& b) {
    *this = *this - b;
    return *this;
  }
  
  Bun operator * (const Bun& b) const {
    return Bun( x * b.x , y * b.y );
  }
  Bun& operator *= (const Bun& b) {
    *this = *this * b;
    return *this;
  }
  
  Bun operator / (const Bun& b) const {
    return Bun( x * b.y , y * b.x );
  }
  Bun& operator /= (const Bun& b) {
    *this = *this / b;
    return *this;
  }

};

ostream &operator<<(ostream &out, const Bun &tgt)
{
  out << tgt.x << "/" << tgt.y;
  return out;
}

