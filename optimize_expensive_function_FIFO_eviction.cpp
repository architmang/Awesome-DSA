#include<bits/stdc++.h>
using namespace std;

static int calls = 0;

int expensive(int x){
    calls++;
    return x*x;
}

unordered_map<int, int> cache;
queue<int> order;
// FIFO structure
int capacity = 1;

int memoization(int input)
{
    if(cache.find(input) != cache.end())
        return cache[input];

    int output = expensive(input);
    // update data structures by insertin new item
    cache[input] = output;
    order.push(input);
    // now check capacity
    if(cache.size() > capacity)
    {
        cache.erase(order.front());
        order.pop();
    }

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