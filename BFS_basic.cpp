// Q: How do you find the shortest path distance from a source to a destination in a 2D grid where 0 is empty and 1 is an obstacle?

#include<bits/stdc++.h>
using namespace std;
struct Point{
    Point(int a, int b)
    {
        x = a;
        y = b;
    };
    int x;
    int y;
};
static int ROWS, COLS;

int dx[4] = {0, 1, -1, 0};
int dy[4] = {1, 0, 0, -1};

int isPointValid(Point demo, const vector<vector<int>> &grid)
{
    if(demo.x >= 0 && demo.x < ROWS && demo.y >= 0 && demo.y < COLS && grid[demo.x][demo.y] == 0)
        return 1;
    return -1;
}
int BFS_solver(const vector<vector<int>> &grid, Point source, Point destination )
{
    ROWS = grid.size();
    COLS = grid[0].size();
    
    if(isPointValid(source, grid) == -1 || isPointValid(destination, grid) == -1)
        return -1;
    
    vector<vector<int>> dist(ROWS, vector<int>(COLS, -1));
    dist[source.x][source.y] = 0;

    queue<Point> q;
    q.push(source);

    while(!q.empty())
    {
        Point top_point = q.front();
        q.pop();
        int top_x = top_point.x;
        int top_y = top_point.y;

        if(top_x == destination.x && top_y == destination.y)
            break;
        
        int distance = dist[top_x][top_y];

        for(int dir=0; dir<4; dir++)
        {
            int neighbor_x = top_x + dx[dir];
            int neighbor_y = top_y + dy[dir];

            Point neighbor_point(neighbor_x, neighbor_y);

            if(isPointValid(neighbor_point, grid) == 1 && dist[neighbor_x][neighbor_y] == -1)
            {
                dist[neighbor_x][neighbor_y] = distance + 1;
                q.push(neighbor_point);
            }
        }
    }
    return dist[destination.x][destination.y];
}
int main()
{
    // 0 1 0 1 0 1
    // 0 0 0 0 0 0
    // 0 0 1 1 0 1
    // 0 1 0 0 0 1
    // 0 1 0 1 0 0
    vector<vector<int>> grid = {
        {0, 1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 1},
        {0, 1, 0, 0, 0, 1},
        {0, 1, 0, 1, 0, 0},
    };
    cout << " answer is " << BFS_solver(grid, Point(0, 0), Point(4, 5)) << endl;
    return 0;
}