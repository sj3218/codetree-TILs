#include <iostream>
#include <vector>
using namespace std;

int N;
vector<vector<int>> grid;

int group_n;
vector<vector<bool>> is_visited;
vector<vector<int>> groups;
vector<int> group_cnt;

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

void Input()
{
    cin >> N;

    int value = 0;
    grid.assign(N, vector<int>(N, 0));

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            cin >> value;
            grid[i][j] = value;
        }
    }
}

bool IsValid(int x, int y)
{
    if (x < 0 || y < 0 || x >= N || y >= N)
        return false;

    return true;
}

void DFS(int x, int y)
{
    int value = grid[x][y];

    for (int i = 0; i < 4; ++i)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (IsValid(nx, ny) && !is_visited[nx][ny] && grid[nx][ny] == value)
        {
            is_visited[nx][ny] = true;
            groups[nx][ny] = group_n;
            ++group_cnt[group_n - 1];

            DFS(nx, ny);
        }
    }
}

void BuildGroup()
{
    group_n = 0;
    is_visited.assign(N, vector<bool>(N, false));
    groups.assign(N, vector<int>(N, 0));
    group_cnt.clear();
    
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (!is_visited[i][j])
            {
                ++group_n;
                is_visited[i][j] = true;
                groups[i][j] = group_n;
                group_cnt.push_back(1);
                DFS(i, j);
            }
        }
    }
}

int GetScore()
{
    int score = 0;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                int nx = i + dx[k];
                int ny = j + dy[k];

                if (IsValid(nx, ny) && grid[nx][ny] != grid[i][j])
                {
                    score += (group_cnt[groups[i][j] - 1] + group_cnt[groups[nx][ny] - 1]) * grid[i][j] * grid[nx][ny];
                }
            }
        }
    }
    return score/2;
}

void RotateSquare(int x, int y, int mid, vector<vector<int>>& next_g)
{
    for (int i = x; i < x + mid; ++i)
    {
        for (int j = y; j < y + mid; ++j)
        {
            int ox = i-x , oy = j-y;
            int rx = oy, ry = mid - ox - 1;
            next_g[rx + x][ry + y] = grid[i][j];
        }
    }
}

void Rotate()
{
    vector<vector<int>> next_grid(N, vector<int>(N, 0));
    int mid_index = N / 2;

    //rotate cross - couter clock wise
    for (int i = 0; i < N; ++i)
    {
        next_grid[mid_index][i] = grid[i][mid_index];
        next_grid[i][mid_index] = grid[mid_index][N - i - 1];
    }

    //rotate the others - clock wise
    RotateSquare(0, 0, mid_index, next_grid);
    RotateSquare(0, mid_index + 1, mid_index, next_grid);
    RotateSquare(mid_index+1, 0, mid_index, next_grid);
    RotateSquare(mid_index+1, mid_index + 1, mid_index, next_grid);

    grid = next_grid;
}

void print_map()
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    Input();
    int total_score = 0;

    for (int i = 0; i < 4; ++i)
    {
        BuildGroup();
        total_score += GetScore();
        Rotate();
    }
    cout << total_score;
    return 0;
}

/*
548
1293
2208
3703
3703
*/