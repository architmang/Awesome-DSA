#include<bits/stdc++.h>
using namespace std;
constexpr int MAX_TREE_NODES = 128;
struct CompactTree{
    int values[MAX_TREE_NODES];
    bool exists[MAX_TREE_NODES];
    CompactTree(){
        fill(&exists[0], &exists[0] + MAX_TREE_NODES, false);
        fill(&values[0], &values[0] + MAX_TREE_NODES, -1);
    };
};
int DFS_solver(CompactTree *tree, int index)
{
    if(index >= MAX_TREE_NODES || index <0 || tree->exists[index] == false)
        return 0;

    int lmax = DFS_solver(tree, 2*index + 1);
    int rmax = DFS_solver(tree, 2*index + 2);
    
    return max(lmax, rmax) + 1;

}
int main()
{
    CompactTree *tree = new CompactTree;
    tree->exists[0] = true;
    tree->exists[1] = true;
    tree->exists[2] = true;
    tree->exists[3] = true;

    cout << "ans is " << DFS_solver(tree, 0) << endl;
    return 0;
}