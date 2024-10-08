#include <iostream>
#include <vector>
using namespace std;

int n;
vector<long long> customers;
int leader;
int member;

void input()
{
    cin >>n;
    int customer;
    for(int i = 0; i< n; ++i)
    {
        cin >> customer;
        customers.push_back(customer);
    }

    cin >> leader;
    cin >>member;
}

int main() {
    // 여기에 코드를 작성해주세요.
    input();

    long long ans = 0;
    long long customer = 0;

    for(int i = 0; i< n; ++i)
    {
        customer = customers[i];

        customer -= leader;
        ++ans;

        if(customer > 0)
        {
            ans += customer / member;
            if(customer % member != 0)
            {
                ++ans;
            }
        }
    }
    
    cout << ans;
    return 0;
}