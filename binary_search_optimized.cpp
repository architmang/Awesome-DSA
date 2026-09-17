// Q: Given a sorted array, how do you efficiently find the index of a target element?
#include<bits/stdc++.h>
using namespace std;
int main()
{
    vector<int> arr = {12, 5, -3, 99, 120, 17, 15};
    sort(arr.begin(), arr.end());
    int base = 0;
    int size = arr.size();
    int val = 17;
    while(size > 1)
    {
        int half = size >> 1;
        base = val > arr[base+half-1] ? base + half: base;
        // ternary operator compiles to cmov
        // computes both values simulatenously
        // no branch misprediction penalties
        size -= half;
    }
    if(arr[base] == val)
        cout << "found " << val << " at index " << base << endl;
    return 0;
}