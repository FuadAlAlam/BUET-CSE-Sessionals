#include <stdio.h>

int main () {

    int income;
    double tax;
    scanf("%d", &income);

    if(income <= 350000){
        printf("0");
    }
    else if(income > 350000 && income <= 450000){
        tax = (double) {(income - 350000)*0.05};
        printf("%.2lf", tax);
    }
    else if(income > 450000 && income <= 750000){
        tax = (double) {5000 + (income - 450000)*0.1};
        printf("%.2lf", tax);
    }
    else if(income > 750000 && income <= 1150000){
        tax = (double) {35000 + (income - 750000)*0.15};
        printf("%.2lf", tax);
    }
    else if(income > 1150000 && income <= 1650000){
        tax = (double) {95000 + (income - 1150000)*0.2};
        printf("%.2lf", tax);
    }
    else{
        tax = (double) {195000 + (income - 1650000)*0.25};
        printf("%.2lf", tax);
    }
}
