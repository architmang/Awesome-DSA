#include<bits/stdc++.h>
using namespace std;
class LRU_cache
{
    public:
        int capacity;
        list<pair<int, int>> List;
        // doubly linked list. highest prioerity at head[front], lowest at tail[back]
        // this is actually our cache which stores key value pairs
        unordered_map<int, list<pair<int, int>>::iterator> cacheMap;
        // maps key to the iterator in our list
    
        LRU_cache(int cap)
        {
            capacity = cap;
        };
        int get(int key)
        {
            auto it = cacheMap.find(key);
            if(it == cacheMap.end())
                return -1;
            // found in cachemap
            int value = it->second->second;
            auto it_list = it->second;

            // update our cache
            List.erase(it_list);
            List.push_front({key, value});

            // update the it against it
            cacheMap[key] = List.begin();

            return value;
        }

        int put(int key, int value)
        {
            // first check if already exists
            auto it = cacheMap.find(key);
            if(it != cacheMap.end())
            {
                // remove old mentions
                cacheMap.erase(key);
                List.erase(it->second);
            }   
            // update cache and cacheMap
            List.push_front({key, value});
            cacheMap[key] = List.begin();
            // size exceeds K, remove LRU
            if(cacheMap.size() > capacity)
            {
                auto it = List.back();
                int key_LRU = it.first;
                cacheMap.erase(key_LRU);
                List.pop_back();
            }
        }
};
int main()
{
    LRU_cache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << endl;
    cache.put(3, 3);
    cout << cache.get(2) << endl;
    cache.put(4, 4);
    cout << cache.get(1) << endl;
    cout << cache.get(3) << endl;
    cout << cache.get(4) << endl;
    return 0;
}