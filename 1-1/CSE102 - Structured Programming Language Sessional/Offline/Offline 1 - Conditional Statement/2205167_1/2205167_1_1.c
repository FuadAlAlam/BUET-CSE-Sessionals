#include <stdio.h>

int main () {

    int a, b, ans;
    double ans1;
    char sign;
    scanf("%d %c %d", &a, &sign, &b);

    if(sign == '+'){
        ans = a + b;
        printf("%d", ans);
    }
    else if(sign == '-'){
        ans = a - b;
        printf("%d", ans);
    }
    else if(sign == '*'){
        ans = a * b;
        printf("%d", ans);
    }
    else if(sign == '/'){
        if(b == 0){
            printf("YOU CANNOT DIVIDE BY 0");
        }
        else{
            if(a%b == 0){
                ans = a / b;
                printf("%d", ans);
            }
            else{
                ans1 = (double) a / b;
                printf("%lf", ans1);
            }
        }
    }
    else{
        printf("SIGN MUST BE +, -, * OR /");
    }

}