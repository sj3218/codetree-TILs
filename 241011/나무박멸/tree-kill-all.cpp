#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int N, M, K, C;
vector<vector<int>> map;
vector<vector<int>> tree_killer_years;
vector<vector<int>> near_empty_count;
int total_killing_tree = 0;

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};
int cross_dx[4] = {-1,-1,1,1};
int cross_dy[4] = {-1,1,1,-1};

void Input()
{
    cin >> N >>M >>K>>C;
    map.assign(N, vector<int>(N, 0));
    tree_killer_years.assign(N, vector<int>(N, 0));
    near_empty_count.assign(N, vector<int>(N, 0));

    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j< N; ++j)
        {
            cin >> map[i][j];
        }
    }
}

bool IsValid(int x, int y)
{
    if(x < 0 || y < 0 || x >= N || y >= N)
    {
        //out of range
        return false;
    }
    return true;
}

void Growing()
{
    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j< N; ++j)
        {
            //there is no tree
            if(map[i][j] <= 0)
            {
                continue;
            }

            for(int k = 0; k< 4; ++k)
            {
                int nx = i + dx[k];
                int ny = j + dy[k];

                if(IsValid(nx, ny))
                {
                    if(map[nx][ny] > 0)
                    {
                        //there is tree;
                        map[i][j]++;
                    }
                    else if(map[nx][ny] == 0 && tree_killer_years[nx][ny] == 0)
                    {
                        //there is empty space
                        near_empty_count[i][j]++;
                    }
                }
            }
        }
    }
}

void Propagation()
{
    vector<vector<int>> map_after_growing = map;

    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j< N; ++j)
        {
            if(map[i][j] <= 0)
            {
                continue;
            }

            if(near_empty_count[i][j] == 0)
            {
                continue;
            }

            int value = map[i][j] / near_empty_count[i][j];
            for(int k = 0; k<4; ++k)
            {
                int nx = i + dx[k];
                int ny = j + dy[k];

                if(IsValid(nx,ny))
                {
                    if(map[nx][ny] == 0 && tree_killer_years[nx][ny] == 0)
                    {
                        map_after_growing[nx][ny] += value;
                    }
                }
            }
        }
    }

    //update map after growing
    map = map_after_growing;
}

int BFS(int x, int y, vector<pair<int,int>>& pos)
{
    queue<tuple<int,int,int,int>> q; // x, y, direction, size(for k)
    int nx, ny;
    int cnt = map[x][y];

    pos.push_back({x,y});
    for(int i = 0; i< 4; ++i)
    {
        nx = x + cross_dx[i];
        ny = y + cross_dy[i];

        q.push({nx, ny, i, 1});
    }

    int direction, k;
    while(!q.empty())
    {
        tie(nx,ny,direction,k) = q.front();
        q.pop();

        if(IsValid(nx,ny))
        {
            if(map[nx][ny] < 0)
            {
                continue;
            }
            
            cnt += map[nx][ny];
            pos.push_back({nx,ny});

            if(k < K && map[nx][ny] > 0)
            {
                q.push({nx + cross_dx[direction], ny + cross_dy[direction], direction, k + 1});
            }
        }
    }
    return cnt;
}

vector<pair<int,int>> FindKillerPosition()
{
    int x, y;
    int max_killing_tree_cnt = 0;
    vector<pair<int,int>> killing_tree_pos;

    vector<pair<int,int>> pos;
    int cnt = 0;

    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j< N; ++j)
        {
            if(map[i][j] <= 0)
            {
                continue;
            }
            
            pos.clear();
            cnt = BFS(i,j, pos);

            if(max_killing_tree_cnt < cnt)
            {
                max_killing_tree_cnt = cnt;
                x= i;
                y= j;
                killing_tree_pos = pos;
            }
        }
    }

    total_killing_tree += max_killing_tree_cnt;
    return killing_tree_pos;
}

void Killing()
{
    vector<pair<int,int>> pos = FindKillerPosition();
    
    int size = pos.size();
    int x,y;

    for(int i = 0; i< size; ++i)
    {
        x = pos[i].first;
        y = pos[i].second;

        tree_killer_years[x][y] = C + 1;
        map[x][y] = 0;
    }
}

void UpdateTreeKillerYears()
{
    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j< N; ++j)
        {
            if(tree_killer_years[i][j] == 0)
            {
                continue;
            }

            tree_killer_years[i][j]--;
        }
    }
}

void print_map()
{
    cout<<"\n";
    for(int i = 0; i< N; ++i)
    {
        for(int j = 0 ; j< N; ++j)
        {
            cout<<map[i][j]<<" ";
        }
    cout<<"\n";
    }

}
int main() {
    // 여기에 코드를 작성해주세요.
    Input();

    for(int i = 0; i< M; ++i)
    {
        Growing();
        Propagation();
        Killing();
        UpdateTreeKillerYears();
    }

    cout<<total_killing_tree;
    return 0;
}