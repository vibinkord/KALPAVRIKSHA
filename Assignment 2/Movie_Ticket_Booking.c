#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int StandardTicketPrice=150;
const int PremiumTicketPrice=250;
const int ReclinerTicketPrice=400;

float price=0.0;

int main(){
    char name[100];
    int TicketCount;
    int TicketCategory;
    char TicketCategoryName[20];
    scanf("%s",name);
    scanf("%d",&TicketCount);
    scanf("%d",&TicketCategory);
    if(TicketCount<=0||TicketCount>20){
        printf("Invalid Ticket Count");
        return 0;
    }

    switch(TicketCategory){
        case 1:
            price=(float) TicketCount*StandardTicketPrice;
            strcpy(TicketCategoryName,"Standard");
            break;
        case 2:
            price=(float)TicketCount*PremiumTicketPrice;
            strcpy(TicketCategoryName,"Premium");
            break;
        case 3:
            price=(float)TicketCount*ReclinerTicketPrice;
            strcpy(TicketCategoryName,"Recliner");
            break;
        default:
            printf("Invalid Ticket Category");
            return 0;
    }
    float discountPrice=0;
    float discount=0;
    if(TicketCount>=5){
        discount=(float)price*10/100;
        // discountPrice=price-discount;
    }
    printf("Customer: %s\n",name);
    printf("Ticket Category: %s\n",TicketCategoryName);
    printf("Tickets: %d\n",TicketCount);
    printf("Subtotal: %.2f\n",price);
    // if(discountPrice>0){
        
        printf("Discount: %.2f\n",discount);
        printf("Final Bill: %.2f\n",price-discount);
    // }
    // else{
        // printf("Discount: %.2f\n",discount);
        // printf("Final Bill: %.2f\n",discountPrice);
    // }


    return 0;
}