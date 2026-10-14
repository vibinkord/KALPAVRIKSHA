#include<stdio.h>
#include<string.h>

const int StandardTicketPrice=150;
const int PremiumTicketPrice=250;
const int ReclinerTicketPrice=400;

int main(){
    char name[100];
    int TicketCount;
    int TicketCategory;
    char TicketCategoryName[20];
    float price=0.0;
    if (scanf("%99s", name) != 1){
        printf("Invalid Customer Name\n");
        return 0;
    }
    if((scanf("%d",&TicketCount)!=1)||(scanf("%d",&TicketCategory) !=1)){
        printf("Invalid Input\n");
        return 0;
    }
    
    if(TicketCount<=0||TicketCount>20){
        printf("Invalid Ticket Count\n");
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
            printf("Invalid Ticket Category\n");
            return 0;
    }
    float discount=0;
    if(TicketCount>=5){
        discount=price/10.0f;
    }
    printf("Customer: %s\n",name);
    printf("Ticket Category: %s\n",TicketCategoryName);
    printf("Tickets: %d\n",TicketCount);
    printf("Subtotal: %.2f\n",price);
    printf("Discount: %.2f\n",discount);
    printf("Final Bill: %.2f\n",price-discount);

    return 0;
}