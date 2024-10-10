#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

int N, M, K;
vector<vector<int>> map;
vector<vector<int>> attack_stack;
int attack_tower_x, attack_tower_y;
int destroied_tower_cnt = 0;
vector<vector<bool>> cannot_repairing_tower;

void Input()
{
    cin >> N >> M >> K;

    map.assign(N, vector<int>(M, 0));
    attack_stack.assign(N, vector<int>(M, 0));
    cannot_repairing_tower.assign(N, vector<bool>(M, false));
    attack_tower_x = INT_MAX;
    int power;

    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j< M; ++j)
        {
            cin >> power;
            map[i][j] = power;
            if(power == 0)
            {
                ++destroied_tower_cnt;
            }
        }
    }
}

bool CurrentTowerIsWeaker(int x, int y, int cx, int cy)
{
    if(map[x][y] > map[cx][cy])
        return true;

    if(map[x][y] == map[cx][cy])
    {
        if(attack_stack[x][y] < attack_stack[cx][cy])
        {
            //attack at latest
            return true;
        }

        if(attack_stack[x][y] == attack_stack[cx][cy])
        {
            if(x + y < cx + cy)
            {
                //sum of the N and M is bigger one
                return true;
            }
            else if(x + y == cx + cy)
            {
                if(y < cy)
                {
                    //column bigger one
                    return true;
                }
            }
        }
    }

    return false;
}

void SelectAttackTower()
{
    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j < M; ++j)
        {
            if(map[i][j] == 0)
            {
                continue;
            }

            if(attack_tower_x == INT_MAX)
            {
                attack_tower_x = i;
                attack_tower_y = j;
                continue;
            }

            if(CurrentTowerIsWeaker(attack_tower_x, attack_tower_y, i, j))
            {
                attack_tower_x =i;
                attack_tower_y =j;
            }
        }
    }

    attack_stack[attack_tower_x][attack_tower_y]++;
}

bool CurrentTowerIsPowerful(int x, int y, int cx, int cy)
{
    if(attack_stack[x][y] > attack_stack[cx][cy])
        {
            return true;
        }

        if(attack_stack[x][y] == attack_stack[cx][cy])
        {
            if(x + y > cx + cy)
            {
                return true;
            }
            else if(x + y == cx + cy)
            {
                if(y > cy)
                {
                    return true;
                }
            }
        }
    return false;
}

pair<int,int> FindPowerfulTower()
{
    int x, y;
    int power = 0;
    int current_power = 0;
    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j<M; ++j)
        {
            current_power = map[i][j];
            if(map[i][j] == 0)
                continue;
            
            if(current_power > power)
            {
                x = i;
                y = j;
                power = current_power;
                continue;
            }
            
            if(current_power == power)
            {
                if(CurrentTowerIsPowerful(x, y, i, j))
                {
                    x = i;
                    y = j;
                    power = current_power;
                }
            }
        }
    }
    return {x,y};
}

bool bfs(int target_x, int target_y, vector<vector<pair<int,int>>>& parent_nodes)
{
    int dx[4] = {0,1,0,-1};
    int dy[4] = {1,0,-1,0};

    bool is_possible = false;
    queue<pair<int,int>> q;
    q.push({attack_tower_x, attack_tower_y});

    int x, y;

    while(!q.empty())
    {
        x = q.front().first;
        y = q.front().second;
        q.pop();

        if(x == target_x && y == target_y)
        {
            is_possible = true;
            break;
        }

        for(int i =0;i< 4; ++i)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(nx < 0)
                nx = N -1;
            else if(nx >= N)
                nx = 0;

            if(ny < 0)
                ny= M -1;
            else if(ny >= M)
                ny = 0;

        
            if(map[nx][ny] == 0)
            {
                continue;
            }

            if(parent_nodes[nx][ny].first != -1 && parent_nodes[nx][ny].second != -1)
            {
                //is already visited
                continue;
            }

            q.push({nx,ny});
            parent_nodes[nx][ny] = {x,y};
        }
    }
    return is_possible;
}

bool Reizer(int target_x, int target_y)
{
    vector<vector<pair<int,int>>> parent_nodes(N, vector<pair<int,int>>(M, {-1,-1}));
    
    if(bfs(target_x, target_y, parent_nodes))
    {
        //attack tower following the parent node
        int parent_x = parent_nodes[target_x][target_y].first;
        int parent_y = parent_nodes[target_x][target_y].second;
        int power = map[attack_tower_x][attack_tower_y];
        int less_power = power /2;

        map[target_x][target_y] -= power;
        if(map[target_x][target_y] <= 0)
        {
            map[target_x][target_y] = 0;
            destroied_tower_cnt++;
        }

        while(true)
        {
            if(parent_x == attack_tower_x && parent_y == attack_tower_y)
            {
                break;
            }
            
            map[parent_x][parent_y] -= less_power;
            cannot_repairing_tower[parent_x][parent_y] = true;
            
            if(map[parent_x][parent_y] <= 0)
            {
                map[parent_x][parent_y] = 0;
                ++destroied_tower_cnt;
            }

            int tmp = parent_nodes[parent_x][parent_y].first;
            parent_y = parent_nodes[parent_x][parent_y].second;
            parent_x = tmp;
        }

        return true;
    }
    return false;
}

void Bomb(int target_x, int target_y)
{
    int dx[8] = {-1,-1,-1,0,0,1,1,1};
    int dy[8] = {-1,0,1,-1,1,-1,0,1};

    int power = map[attack_tower_x][attack_tower_y];
    int less_power = power/2;

    map[target_x][target_y] -= power;

    if(map[target_x][target_y] <= 0)
    {
        map[target_x][target_y] = 0;
        ++destroied_tower_cnt;
    }

    for(int i = 0; i < 8; ++i)
    {
        int nx = target_x + dx[i];
        int ny = target_y + dy[i];

        if(nx < 0)
            nx = N-1;
        if(ny < 0)
            ny = M -1;
        if(nx >= N)
            nx = 0;
        if(ny >= M)
            ny = 0;

        map[nx][ny] -= less_power;
        cannot_repairing_tower[nx][ny] = true;

        if(map[nx][ny] <= 0)
        {
            map[nx][ny] = 0;
            ++destroied_tower_cnt;
        }
    }
}

void Attack()
{
    pair<int,int> target = FindPowerfulTower();
    
    cannot_repairing_tower[target.first][target.second] = true;
    cannot_repairing_tower[attack_tower_x][attack_tower_y] = true;

    if(!Reizer(target.first, target.second))
    {
        //attack using bomb
        Bomb(target.first, target.second);
    }
}

void RepairTower()
{
    for(int i = 0; i< N; ++i)
    {
        for(int j = 0; j< M; ++j)
        {
            if(map[i][j] == 0)
            {
                continue;
            }

            if(cannot_repairing_tower[i][j] == true)
            {
                continue;
            }

            map[i][j] += 1;
        }
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    Input();

    for(int i = 0; i< K; ++i)
    {
        if(destroied_tower_cnt - 1 == N*M)
        {
            break;
        }

        cannot_repairing_tower.assign(N, vector<bool>(M, false));
        
        SelectAttackTower();
        map[attack_tower_x][attack_tower_y] += (N + M);

        Attack();
        RepairTower();
    }

    pair<int,int> powerful_tower = FindPowerfulTower();
    cout<<map[powerful_tower.first][powerful_tower.second];
    return 0;
}