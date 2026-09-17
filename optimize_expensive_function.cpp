#include<bits/stdc++.h>
using namespace std;

static int calls = 0;

int expensive(int x){
    calls++;
    return x*x;
}

unordered_map<int, int> cache;

int memoization(int input)
{
    if(cache.find(input) != cache.end())
        return cache[input];

    int output = expensive(input);

    return cache[input] = output;
}

int main()
{
    assert(memoization(4) == 16);
    assert(memoization(4) == 16);
    assert(calls == 1);
    return 0;
}