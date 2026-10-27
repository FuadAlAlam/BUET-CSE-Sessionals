#include<stdio.h>

int main () {

    int n;
    scanf("%d", &n);
    
    for(int i = 1; i <= n; i++){
        printf("Case %d\n", i);

        long long int k, k_cng, power;
        int m, k1 = 0;
        scanf("%lld %d", &k, &m);

        power = 4294967296;
        k_cng = k;
        for(int l = 1; l <= 32; l++){
            power = power/2;
            if(k_cng >= power){
                k1++;
                k_cng = k_cng - power;
            }
        }

        long long int min1 = 4294967296, max1 = 0;
        int min_diff = 32, max_diff = 0;

        for(int j = 1; j <= m; j++){
            long long int a, a_cng;
            int a1 = 0;
            scanf("%lld", &a);

            power = 4294967296;
            a_cng = a;
            for(int l = 1; l <= 32; l++){
                power = power/2;
                if(a_cng >= power){
                    printf("1");
                    a1++;
                    a_cng = a_cng - power;
                }
                else{
                    printf("0");
                }
            }
            printf("\n");

            int diff = (k1>a1) ? k1-a1 : a1-k1;
            if(diff < min_diff){
                min1 = a;
                min_diff = diff;
            }
            else if(diff == min_diff){
                min1 = (a < min1) ? a : min1;
            }
            if(diff > max_diff){
                max1 = a;
                max_diff = diff;
            }
            else if(diff == max_diff){
                max1 = (a > max1) ? a : max1;
            }
        }
        
        power = 4294967296;
        k_cng = k;
        for(int l = 1; l <= 32; l++){
            power = power/2;
            if(k_cng >= power){
                printf("1");
                k_cng = k_cng - power;
            }
            else{
                printf("0");
            }
        }
        printf("\n");

        printf("Number with Minimum Difference: %lld\nNumber with Maximum Difference: %lld\n\n", min1, max1);
    }
    
}