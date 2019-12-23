#pragma once
#include <vector>
#include <queue>
#include <utility>
using namespace std;
typedef pair<String, byte> psb;
typedef struct userinfo {
  int id, hp, boost, bullet, armor;
}userinfo;

class Query {
  private:
  queue<vector<psb>> Q;
  const String mPath, mHost;
  const int mPort;
  String makeUrl();

  public:
  Query(const String&, const int, const String&);
  void sendQuery(const char* const, const char* const);
  void pushData(const userinfo);
};
