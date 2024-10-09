#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int k, m;
vector<vector<int>> map(5, vector<int>(5, 0));
vector<int> pieces;
int piece_index = 0;
int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

void input()
{
    cin >> k >> m;

    for(int i = 0; i< 5; ++i)
    {
        for(int j = 0; j< 5; ++j)
        {
            cin >> map[i][j];
        }
    }

    int piece;
    for(int i = 0; i< m; ++i)
    {
        cin >> piece;
        pieces.push_back(piece);
    }
}

void FillInPiece()
{
    for(int i = 0; i< 5; ++i)
    {
        for(int j = 4; j >= 0; --j)
        {
            if(map[j][i] == 0 && piece_index < m)
            {
                map[j][i] = pieces[piece_index];
                ++piece_index;
            }
        }
    }
}

int BFS(vector<vector<int>>& m, int x, int y, int piece_value, vector<pair<int,int>>& visited_position)
{
    m[x][y] = 0;
    int count = 1;
    visited_position.push_back({x,y});

    queue<pair<int,int>> q;
    q.push({x,y});

    int cx, cy;
    while(!q.empty())
    {
        tie(cx, cy) = q.front();
        q.pop();

        for(int i = 0; i< 4; ++i)
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            
            //out of range
            if(nx < 0 || ny < 0 || nx >= 5 || ny >= 5)
            {
                continue;
            }

            if(piece_value == m[nx][ny])
            {
                q.push({nx,ny});
                visited_position.push_back({nx, ny});
                m[nx][ny] = 0;
                ++count;
            }
        }
    }

    return count;
}

int FindPiece(vector<vector<int>>& m)
{
    int find_piece_count = 0;
    vector<pair<int,int>> visited_position;

    int x,y, count;
    int value = 0;

    for(int i = 0; i< 5; ++i)
    {
        for(int j = 0; j< 5; ++j)
        {
            visited_position.clear();
            value = m[i][j];
            if(value == 0)
            {
                //already visited
                continue;
            }
            count = BFS(m, i, j, value, visited_position);

            if(count >= 3)
            {
                find_piece_count += count;
            }
            else
            {
                int size= visited_position.size();
                for(int k = 0; k < size; ++k)
                {
                    x = visited_position[k].first;
                    y = visited_position[k].second;

                    m[x][y] = value;
                }
            }
        }
    }
    return find_piece_count;
}

int GetPiece()
{
    FillInPiece();
    int total_piece = 0;

    while(true)
    {
        int cnt = FindPiece(map);
        if(cnt == 0)
            break;
        
        total_piece += cnt;
        FillInPiece();
    }
    return total_piece;
}

void RotationPiece(vector<vector<int>>& m, int cx, int cy)
{
    int tmp1= m[cx-1][cy-1];
    int tmp2= m[cx-1][cy];
    int tmp3= m[cx-1][cy+1];
    int tmp4= m[cx][cy+1];
    int tmp5= m[cx + 1][cy+1];
    int tmp6= m[cx+1][cy];
    int tmp7= m[cx+1][cy-1];
    int tmp8= m[cx][cy-1];

    m[cx-1][cy-1] = tmp7;
    m[cx-1][cy] = tmp8;
    m[cx-1][cy+1] = tmp1;
    m[cx][cy+1] = tmp2;
    m[cx + 1][cy+1]= tmp3;
    m[cx+1][cy]= tmp4;
    m[cx+1][cy-1]= tmp5;
    m[cx][cy-1]= tmp6;
}

bool IsBetterChoice(int cnt, int x, int y, int rotation_degree, int current_cnt, int cx, int cy, int current_degree)
{
    if(current_cnt == 0)
        return false;

    if(cnt < current_cnt)
    {
        return true;
    }

    if(cnt == current_cnt)
    {
        if(rotation_degree > current_degree)
        {
            return true;
        }

        if(rotation_degree == current_degree)
        {
            if(y > cy)
            {
                return true;
            }

            if(y == cy && x > cx)
            {
                return true;
            }
        }
    }

    return false;
}

int ExplorationProcess()
{
    vector<vector<int>> m = map;

    vector<vector<int>> new_m = m;
    vector<vector<int>> rotate_m = m;

    int degree = 0;//rotation degree 0,90 1,180 2,270
    int cx = 5;
    int cy = 5;//center x center y

    int find_piece_count = FindPiece(new_m);
    int cnt = 0;

    //select center point for rotation
    for(int i = 1; i < 4; ++i)
    {
        for(int j = 1; j< 4; ++j)
        {
            m = map;
            //rotation clockcounter
            for(int k = 0; k<3; ++k)
            {
                RotationPiece(m, i, j);
                rotate_m = m;
                cnt = FindPiece(rotate_m);

                if(IsBetterChoice(find_piece_count, cx, cy, degree, cnt, i, j, k))
                {
                    find_piece_count = cnt;
                    cx = i;
                    cy = j;
                    degree = k;
                    new_m = rotate_m;
                }
            }
        }
    }

    map = new_m;
    return find_piece_count;
}

int main() {
    // 여기에 코드를 작성해주세요.
    input();

    int total_piece = 0;

    for(int i = 0; i< k; ++i)
    {
        total_piece = ExplorationProcess();
        if(total_piece == 0)
        {
            break;
        }

        total_piece += GetPiece();
        cout <<total_piece<<" ";
    }

    return 0;
}