// Q: How do you find the shortest path distance from a source to a destination in a 2D grid where 0 is empty and 1 is an obstacle?

#include<bits/stdc++.h>
using namespace std;
struct Point{
    // default ctor
    Point() {};

    // custom ctor
    Point(int a, int b)
    {
        x = a;
        y = b;
    };

    int x;
    int y;
};
constexpr int ROWS = 128, COLS = 128;

int dx[4] = {0, 1, -1, 0};
int dy[4] = {1, 0, 0, -1};

int isPointValid(Point demo, const array<array<int, COLS>, ROWS> &grid)
{
    if(demo.x >= 0 && demo.x < ROWS && demo.y >= 0 && demo.y < COLS && grid[demo.x][demo.y] == 0)
        return 1;
    return -1;
}
int BFS_solver(const array<array<int, COLS>, ROWS> &grid, Point source, Point destination )
{
    if(isPointValid(source, grid) == -1 || isPointValid(destination, grid) == -1)
        return -1;
    
    int dist[ROWS][COLS];
    fill(&dist[0][0], &dist[0][0] + (ROWS*COLS), -1);
    dist[source.x][source.y] = 0;

    constexpr int MAX_QUEUE_SIZE = ROWS*COLS;
    Point queue_buffer[MAX_QUEUE_SIZE];
    int head = 0;
    int tail = 0;

    queue_buffer[tail++] = source;

    while(head < tail)
    {
        Point top_point = queue_buffer[head++];

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
                queue_buffer[tail++] = neighbor_point;
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
    // double braces else error
    array<array<int, COLS>, ROWS> grid = {{
        {0, 1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 1},
        {0, 1, 0, 0, 0, 1},
        {0, 1, 0, 1, 0, 0},
    }};
    cout << " answer is " << BFS_solver(grid, Point(0, 0), Point(4, 5)) << endl;
    return 0;
}