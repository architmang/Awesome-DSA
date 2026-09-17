// Q: Given a sorted array, how do you efficiently find the index of a target element?
#include<bits/stdc++.h>
using namespace std;
int main()
{
    vector<int> arr = {12, 5, -3, 99, 120, 17, 15};
    sort(arr.begin(), arr.end());
    int low = 0;
    int high = arr.size() -1;
    int val = 17;
    while(low <= high)
    {
        int mid = low + (high - low)/2;
        if(arr[mid] == val){
            cout << "found " << val << " at " << mid << endl;
            break;
        }
        else if(arr[mid] > val){
            high = mid-1;
        }
        else{
            low = mid+1;
        }
    }
    return 0;
}