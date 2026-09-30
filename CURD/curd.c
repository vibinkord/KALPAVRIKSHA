#include <stdio.h>
#include <string.h>

void CreateUser();
void DisplayUser();
void UpdateUser();
void DeleteUser();

struct User{
    int id;
    char name[50];
    int age;
};

int main(){
    int choice=0;
    while(choice!=5){
        printf("Enter Choice\n1. Add User\n2. Display User\n3. Update User\n4. Delete User\n5. Exit\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
            CreateUser();
            break;
        case 2:
            DisplayUser();
            break;
        case 3:
            UpdateUser();
            break;
        case 4:
            DeleteUser();
            break;
        case 5:
            printf("Exit\n");

            break;
        default:
            printf("Invalid choice\n");
            break;
    }
}
    return 0;
}
void CreateUser(){
            struct User user;
            FILE*fp=fopen("user.txt","a");
            if(fp==NULL){
                printf("Not opening \n");
                return;
            }
            printf("Add User\nEnter ID, Name, Age:\n");
            scanf("%d",&user.id);
            scanf("%s",user.name);
            scanf("%d",&user.age);
            // printf("User Added: ID=%d, Name=%s, Age=%d\n",user.id,user.name,user.age);
            fprintf(fp,"%d %s %d\n",user.id,user.name,user.age);
            fclose(fp);
            printf("User added successfully.\n");

}

void DisplayUser(){
    struct User user;
    FILE*fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Not opening \n");
        return;
    }
    while (fscanf(fp, "%d %s %d", &user.id, user.name, &user.age) == 3) {
    printf("User: ID=%d, Name=%s, Age=%d\n", user.id, user.name, user.age);
}
fclose(fp);
}
void DeleteUser(){
    struct User user;
    int id,found=0;
    printf("Enter ID to delete:\n");
    scanf("%d",&id);
    FILE *fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Not opening \n");
        return;
    }
    FILE *temp=fopen("temp.txt","w");
    if(temp==NULL){
        printf("Not opening\n");
        return;
    }
    while(fscanf(fp,"%d %s %d",&user.id,user.name,&user.age)==3){
        if(id==user.id){
            found=1;
        }
        else{
            fprintf(temp,"%d %s %d\n",user.id,user.name,user.age);
        }
    }
    fclose(fp);
    fclose(temp);
    remove("user.txt");
    rename("temp.txt","user.txt");
    if(found){
        printf("User deleted successfully.\n");
    } else {
        printf("User not found.\n");
    }
}
void UpdateUser(){
    struct User user;
    int id,found=0;
    printf("Enter Id to update:\n");
    scanf("%d",&id);
    FILE *fp=fopen("user.txt","r");
    if(fp==NULL){
        printf("Not opening \n");
        return;
    }
    FILE *temp=fopen("temp.txt","w");
    if(temp==NULL){
        printf("Not opening\n");
        return;
    }
    while(fscanf(fp,"%d %s %d",&user.id,user.name,&user.age)==3){
        if(id==user.id){
            found=1;
            int age;
            char name[50];
            printf("Enter new Name and Age:\n");
            scanf("%s",name);
            scanf("%d",&age);
            user.age=age;
            strcpy(user.name,name);
            fprintf(temp,"%d %s %d\n",user.id,user.name,user.age);
        }
        else{
            fprintf(temp,"%d %s %d\n",user.id,user.name,user.age);
        }
        fclose(fp);
    }
        fclose(temp);
        remove("user.txt");
        rename("temp.txt","user.txt");
        if(found){
            printf("User updated successfully.\n");
        } else {
            printf("User not found.\n");
        }


}