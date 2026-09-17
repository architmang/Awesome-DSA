#include<bits/stdc++.h>
using namespace std;

static int calls = 0;

int expensive(int x){
    calls++;
    return x*x;
}

class LRU_cache
{
    public:
        int capacity;
        list<pair<int, int>> List;
        unordered_map<int, list<pair<int, int>>::iterator> cacheMap;

        LRU_cache(int cap){
            capacity = cap;
        }
        
        int get(int key){
            if(cacheMap.find(key) == cacheMap.end())
                return -1;
            auto it = cacheMap[key];
            int value = it->second;
            // update priority
            List.erase(it);
            List.push_front({key, value});
            cacheMap[key] = List.begin();
            return value;
        }

        void put(int key, int value){
            if(cacheMap.find(key) != cacheMap.end())
            {
                auto it = cacheMap[key];
                it->second =  value;
                return;       
            }
            // not present, put now
            List.push_front({key, value});
            cacheMap[key] = List.begin();

            // check for capacity
            if(cacheMap.size() > capacity)
            {
                int key_to_remove = List.back().first;
                List.pop_back();
                cacheMap.erase(key_to_remove);
            }
        }
};
LRU_cache cache(1);

int memoization(int input)
{
    int cache_status = cache.get(input);
    if( cache_status != -1)
        return cache_status;

    int output = expensive(input);
    // update data structures by insertin new item
    cache.put(input, output);
    return output;
}

int main()
{
    assert(memoization(4) == 16);
    assert(memoization(4) == 16);
    assert(calls == 1);
    assert(memoization(3) == 9);
    assert(memoization(4) == 16);
    cout << calls << endl;
    return 0;
}