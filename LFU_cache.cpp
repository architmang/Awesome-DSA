#include<bits/stdc++.h>
using namespace std;
class LFUCache{
    public:
        int capacity;
        int minFrequency;

        LFUCache(int cap){
            capacity = cap;
            minFrequency = 0;
        };

        struct node{
            int key, value, frequency;
        };

        map<int, list<node>> frequencytolist;
        map<int, list<node>::iterator> keytoIterator;

        // helper function
        void updateFreq(map<int, list<node>::iterator>::iterator mapIt)
        {
            auto listIt = mapIt->second;

            int key = listIt->key;
            int value = listIt->value;
            int frequency = listIt->frequency;

            // remove from the current frequency list
            frequencytolist[frequency].erase(listIt);
            
            // update freq
            int newfreq = frequency + 1;
            
            // update data structures
            frequencytolist[newfreq].push_front({key, value, newfreq});
            mapIt->second = frequencytolist[newfreq].begin();

            // update minFrequency
            if(frequencytolist[minFrequency].empty())
            {
                frequencytolist.erase(minFrequency);
                minFrequency++;
            }
        }

        int get(int key){
            if(capacity == 0)
                return -1;

            auto it = keytoIterator.find(key);
            
            if(it == keytoIterator.end())
                return -1;

            int value = it->second->value;
            updateFreq(it);
            return value;
        }

        void put(int key, int value){
            if(capacity == 0)
                return;

            // case1: key exists, update value and freq
            auto it = keytoIterator.find(key);
            if(it!=keytoIterator.end())
            {
                auto &listIt = it->second;
                listIt->value = value;
                updateFreq(it);
                return;
            }
            // case 2: capacity full
            if(keytoIterator.size() == capacity)
            {
                // evict LFU
                auto &minFreqList = frequencytolist[minFrequency];

                // update both data structures
                auto &node_to_remove = minFreqList.back();
                int key_to_remove = node_to_remove.key;
                minFreqList.pop_back();
                keytoIterator.erase(key_to_remove);

                if(minFreqList.empty())
                {
                    frequencytolist.erase(minFrequency);
                }
            }
            
            // case 3: insert completely new node
            minFrequency = 1;
            frequencytolist[minFrequency].push_front({key, value, 1});
            keytoIterator[key] = frequencytolist[minFrequency].begin();
        }
};

int main()
{
    LFUCache cache(2);
    
    cache.put(1, 1);
    cache.put(2, 2);
    cout << "get(1): " << cache.get(1) << endl; // Returns 1
    
    cache.put(3, 3);                             // Evicts key 2 
    cout << "get(2): " << cache.get(2) << endl; // Returns -1 (not found)
    cout << "get(3): " << cache.get(3) << endl; // Returns 3
    
    cache.put(4, 4);                             // Evicts key 1 
    cout << "get(1): " << cache.get(1) << endl; // Returns -1 (not found)
    cout << "get(3): " << cache.get(3) << endl; // Returns 3
    cout << "get(4): " << cache.get(4) << endl; // Returns 4
   
    return 0;
}