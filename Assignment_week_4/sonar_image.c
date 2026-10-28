#include<stdio.h>
#include <stdlib.h>
#include<time.h>
void PrintMatrix(int n,int arr[n][n]);
void rotateMatrix(int n,int arr[n][n]);
void smoothingImage(int n,int arr[n][n]);

int main(){
    srand(time(NULL));
    int min=0,max=255;
    int n;
    printf("Enter the size of the array(2-10): ");
    if(scanf("%d",&n)!=1||n<2||n>10){
        printf("Invalid size\n");
        return 0;
    }
    int arr[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
       *(*(arr+i)+j)=rand()%(max-min+1)+min;
        }
    }
    printf("Original Matrix:\n");
    PrintMatrix(n,arr);
    rotateMatrix(n,arr);
    printf("Matrix after 90 Clockwise Rotation:\n");
    PrintMatrix(n,arr);
    smoothingImage(n,arr);
    printf("Matrix after Applying 3×3 Smoothing Filter:\n");
    PrintMatrix(n,arr);
}
void PrintMatrix(int n,int arr[n][n]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",*(*(arr+i)+j));
        }
        printf("\n");
    }
    printf("\n");
}

void rotateMatrix(int n,int arr[n][n]){
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            int temp=*(*(arr+i)+j);
            *(*(arr+i)+j)=*(*(arr+j)+i);
            *(*(arr+j)+i)=temp;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n/2;j++){
            int temp=*(*(arr+i)+j);
            *(*(arr+i)+j)=*(*(arr+i)+n-1-j);
            *(*(arr+i)+n-1-j)=temp;
        }
    }
}
void smoothingImage(int n,int arr[n][n]){
    int curr[n],prev[n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            *(curr+j)=*(*(arr+i)+j);
        }
        for(int j=0;j<n;j++){
            int sum=0,count=0;
            for(int k=-1;k<=1;k++){
                for(int l=-1;l<=1;l++){
                    int r=i+k,c=j+l;
                    if(r<0||r>=n||c<0||c>=n){
                        continue;
                    }
                    if(k==-1)sum+=*(prev+c);
                    else if(k==0) sum+=*(curr+c);
                    else sum+=*(*(arr+r)+c);
                    count++;
                }
            }
            *(*(arr+i)+j)=sum/count;
        }
        for(int j=0;j<n;j++){
            *(prev+j)=*(curr+j);
        }
    }
}