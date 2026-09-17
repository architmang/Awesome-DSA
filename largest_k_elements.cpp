// Q: How do you find the K largest elements in an unsorted stream or array efficiently?
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int k = 5;
    vector<int> large_fixed_tc = {
        10, 25, 45, 90, 12, 55, 67, 89, 91, 102,
        3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
        -5, -12, 0, 45, -99, 1000, 250, 450, 12, 0
    };
    priority_queue<int, vector<int>, greater<int>> pq;
    // make_heap(large_fixed_tc.begin(), large_fixed_tc.end());
    // converts vector into max heap
    for(int i=0;i<large_fixed_tc.size(); i++)
    {
        if(pq.size()<k) 
            pq.push(large_fixed_tc[i]);
        else{
            int top = pq.top();
            // if incoming is bigger than top, top is evicted
            if(large_fixed_tc[i] > top)
            {
                pq.pop();
                pq.push(large_fixed_tc[i]);
            }
        }
        // cout << large_fixed_tc.front() << endl;
        // pop_heap(large_fixed_tc.begin(), large_fixed_tc.end() -i);
    }
    for(int i=0;i<k; i++)
    {
        cout << pq.top() << endl;
        pq.pop();
    }
    return 0;
}