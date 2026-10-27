#include<iostream>
#include<climits>

using namespace std;

pair<pair<int, int>, int> shortestCrossingStreak(int arr[], int lo, int hi, int mid){
    int maxLeft = INT_MIN, maxRight = INT_MIN;

    int sum = 0;
    int left, right;
    for(int i=mid; i>=lo; i--){
        sum += arr[i];
        if(maxLeft < sum){
            maxLeft = sum;
            left = i;
        }
    }

    sum = 0;
    for(int i=mid; i<=hi; i++){
        sum += arr[i];
        if(maxRight < sum){
            maxRight = sum;
            right = i;
        }
    }

    int ans = maxLeft + maxRight - arr[mid];
    if(ans <= maxLeft){
        ans = maxLeft;
        right = mid;
    }
    if(ans <= maxRight){
        ans = maxRight;
        left = mid;
    }

    return {{left, right}, ans};
}

pair<pair<int, int>, int> shortestStreak(int arr[], int lo, int hi){
    if(lo == hi) return {{lo, hi}, arr[lo]};

    int mid = (lo+hi)/2;

    auto left = shortestStreak(arr, lo, mid);
    auto right = shortestStreak(arr, mid+1, hi);
    auto cross = shortestCrossingStreak(arr, lo, hi, mid);

    auto ans = cross;
    int l = left.first.second - left.first.first + 1;
    int shortest = ans.first.second - ans.first.first + 1;
    if((ans.second < left.second) || (ans.second == left.second && l < shortest)){
        ans = left;
    }
    int r = right.first.second - right.first.first + 1;
    shortest = ans.first.second - ans.first.first + 1;
    if((ans.second < right.second) || (ans.second == right.second && r < shortest)){
        ans = right;
    }

    return ans;
}

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n;  cin>>n;
    int arr[n];
    for(int i=0; i<n; i++){
        cin>>arr[i];
    }
    auto ans = shortestStreak(arr, 0, n-1);
    
    if(ans.second <= 0){
        cout<<"[] with a sum of "<<0<<".";
        return 0;
    }

    cout<<"[";
    for(int i=ans.first.first; i<=ans.first.second; i++){
        cout<<arr[i];
        if(i != ans.first.second) cout<<", ";
    }
    cout<<"] with a sum of "<<ans.second<<".";
}