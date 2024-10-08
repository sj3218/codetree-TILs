#include <iostream>
#include <vector>
using namespace std;

int n;
vector<pair<int,int>> working;

void input()
{
    cin >> n;
    int time, pay;
    for(int i = 0; i< n; ++i)
    {
        cin >> time >> pay;
        working.push_back({time, pay});
    }
}

int recursive(int current_x, int total_time, int total_pay)
{
    int time = 0;
    int pay = 0;
    int ans = total_pay;

    if(current_x >= n)
    {
        return ans;
    }

    for(int i = current_x; i < n; ++i)
    {
        time = total_time + working[i].first;
        pay = total_pay + working[i].second;

        if(time > n)
        {
            continue;
        }

        ans = max(ans, recursive(i+ working[i].first, time, pay));
    }

    return ans;
}
int main() {
    // 여기에 코드를 작성해주세요.

    input();
    int ans = 0;

    for(int i = 0; i< n; ++i)
    {
        ans = max(ans, recursive(i +working[i].first, working[i].first, working[i].second));    
    }
    cout << ans;
    return 0;
}