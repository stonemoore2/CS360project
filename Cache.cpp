#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <map>
#include <bitset>
#include <utility>

using namespace std;

class Cache {

int sets;
int ways;
int block_size;
int address_size;

int accesses;
int misses;
int replacements;

/*vector<vector<string> > cache(sets);
  for (int i = 0; i < sets; i++) {
    cache[i].resize(ways);
    for (int j = 0; j < ways; j++) {
      cache[i][j] = string(block_size,'0');
    }
    }*/

map<string, vector<pair<string,int> > > cache;


public:
  Cache(int s,int w,int bs,int as);
  string getIndex(string address);
  string getTag(string address);
  string cacheSearch(string address);
  void displayCache();
  double missRate();
  double replacementRate();
};

Cache::Cache (int s, int w, int bs, int as) {
  sets = s;
  ways = w;
  block_size = bs;
  address_size = as;

  accesses = 0;
  misses = 0;
  replacements = 0;

  for (int i = 0; i < sets; i++) {
    string key =  bitset<64>(i).to_string();
    key = key.substr(key.length()-(floor(log2(sets))),floor(log2(sets)));
    cache[key].resize(ways);
      for (int j = 0; j < ways; j++) {
      cache[key][j].first = "";
      cache[key][j].second = 0;
      }
  }
}
  
string Cache::getIndex(string address) {
  if (address.length() != address_size)
    return "N/A";
  //  return address.substr(log2(block_size),log2(sets));
  return address.substr(address_size - (log2(block_size) + log2(sets)),log2(sets));
}

string Cache::getTag(string address) {
  if (address.length() != address_size)
    return "N/A";
  //return address.substr(log2(block_size)+log2(sets),block_size - log2(block_size)+log2(sets));
  return address.substr(0,address_size - (log2(block_size)+log2(sets)));
}

string Cache::cacheSearch(string address) {
  accesses++;
  string key = getIndex(address);
  if (cache.count(key) > 0) {
    int min = cache[key][0].second;
    int mindex = 0;
    for (int i = 0; i < ways; i++) {
      if (getTag(cache[key][i].first) == getTag(address))
	return "Cache Hit!";
      if (min > cache[key][i].second) {
	min = cache[key][i].second;
	mindex = i;
      }
    }
    misses++;
    if (cache[key][mindex].first == "") {
      cache[key][mindex].first = address;
      cache[key][mindex].second++;
      return "Cache Miss!";
    }
    else {
      replacements++;
      cache[key][mindex].first = address;
      cache[key][mindex].second++;
      return "Cache Miss + Replacement!";
    }
  }
  else
    return "Improper Input";
    }

void Cache::displayCache() {
  for (int i = 0; i < sets; i++) {
    string key =  bitset<64>(i).to_string();
    key = key.substr(key.length()-(floor(log2(sets))),floor(log2(sets)));
    for (int j = 0; j < ways; j++) {
      //cache[key][j].first = "";
      //cache[key][j].second = 0;
      cout << "Index: " << key << " Tag: " << getTag(cache[key][j].first) << " Block #: " << (ways*i) + j << endl;
    }
  }
}

double Cache::missRate() {
  return ((double)(misses))/((double)(accesses));
}

double Cache::replacementRate() {
  return ((double)(replacements))/((double)(accesses));
}
/*
int main() {

  /*for (int i = 0; i < sets; i++) {
    string key =  bitset<64>(i).to_string();
    key = key.substr(key.length()-(floor(log2(sets))),floor(log2(sets)));
    cache[key].resize(ways);
      for (int j = 0; j < ways; j++) {
      cache[key][j].first = "";
      cache[key][j].second = 0;
      }
  }
  

  Cache assoc = Cache(16,2,4,11);
  
  cout << assoc.cacheSearch("00000000000") <<endl;
  cout << assoc.cacheSearch("10000000000") <<endl;
  cout << assoc.cacheSearch("10100000000") <<endl;
  cout << assoc.cacheSearch("01000000000") <<endl;
  cout << assoc.cacheSearch("01000000000") <<endl;
  cout << assoc.cacheSearch("10100000000") <<endl;
  
  assoc.displayCache();

  cout << assoc.missRate() << endl;
  cout << assoc.replacementRate() << endl;
}
*/
  
