// cpp code
// #include <vector>
// #include <utility>
#include <bits/stdc++.h>
using namespace std;

int astar(int m, int n)
{
   vector<pair<int g,int h, int f>> algo;
   auto right=algo,left=algo,front=algo;
   int fmin=0;
   for (auto it : algo){
      (*it).g()=fmod(m-start.x)+fmod(n-start.y);
      (*it).h()=fmod(finish.x-m)+fmod(finish.y-n);
      (*it).f()=(*it).g()+(*it).h();
      if ((*it).f()<fmin){
         fmin=(*it).f();
         dir=(*it);
      }
      

   }
   
}
   int main()
   {
      vector<pair<byte x, byte y>> path;
      cout << "Enter end point cordinates:";
      auto finish = path;
      cin >> finish.x() >> finish.y();
      cout << "Enter start point cordinates:";
      auto start = path;
      cin >> start.x() >> start.y();
      cout << "Enter the dimension of nodes:";
      byte n;
      cin >> n;
      auto pos=path;
      cout << "Enter the current cordinates of bot:";
      cin >> pos.x() >> pos.y();
      astar(pos.x)
   }
}