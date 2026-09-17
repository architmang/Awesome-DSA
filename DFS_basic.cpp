#include<bits/stdc++.h>
using namespace std;
struct TreeNode{
    int val;
    TreeNode *left, *right;
    TreeNode(int x){
        val = x;
        left = nullptr;
        right = nullptr;
    }
};
int DFS_solver(TreeNode* root)
{
    if(root == nullptr)
        return 0;
    int lmax = DFS_solver(root->left);
    int rmax = DFS_solver(root->right);
    return max(lmax, rmax) + 1;

}
int main()
{
    TreeNode *x = new TreeNode(0);
    TreeNode *y = new TreeNode(1);
    x->left = y;
    cout << "ans is " << DFS_solver(x) << endl;
    return 0;
}