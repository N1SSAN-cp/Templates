#include <iostream>
using namespace std;

using ll = long long;

int dg()
{
    cout << "digit" << endl;
    cout.flush();
    int r; cin >> r;
    return r;
}

int add(int n)
{
    cout << "add " << n << endl;
    cout.flush();
    int r; cin >> r;
    return r;
}

int mul(int n)
{
    cout << "mul " << n << endl;
    cout.flush();
    int r; cin >> r;
    return r;
}

int div(int n)
{
    cout << "div " << n << endl;
    cout.flush();
    int r; cin >> r;
    return r;
}

int finish()
{
    cout << "!" << endl;
    cout.flush();
    int r; cin >> r;
    return r;
}

void solve()
{
    ll n; cin >> n;

    dg();
    dg();
    add(-8);
    add(-4);
    add(-2);
    add(-1);
    add(n - 1);
    finish();
}

int main()
{
    ll t; cin >> t;
    while (t--) { solve(); }

    return 0;
}
