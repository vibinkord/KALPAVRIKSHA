#include <stdio.h>
#include <string.h>

#define userFile "users.txt"

void CreateFile();
void CreateUser();
void DisplayUser();
void UpdateUser();
void DeleteUser();

struct User{
    int userId;
    char userName[50];
    int userAge;
};

int main(){
    CreateFile();
    int Choice=0;
    while(Choice!=5){
        printf("Enter Choice\n1. Add User\n2. Display User\n3. Update User\n4. Delete User\n5. Exit\n");
        scanf("%d",&Choice);
        switch(Choice){
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

void CreateFile(){
    FILE *userFileHandle = fopen(userFile, "a");
    if (userFileHandle != NULL) fclose(userFileHandle);
}

void CreateUser(){
    struct User user;
    FILE *userFileHandle=fopen(userFile,"a");
    if(userFileHandle==NULL){
        printf("Not opening \n");
        return;
    }
    printf("Add User\nEnter ID, Name, Age:\n");
    scanf("%d",&user.userId);
    scanf("%s",user.userName);
    scanf("%d",&user.userAge);
    fprintf(userFileHandle,"%d %s %d\n",user.userId,user.userName,user.userAge);
    fclose(userFileHandle);
    printf("User added successfully.\n");
}

void DisplayUser(){
    struct User user;
    FILE *userFileHandle=fopen(userFile,"r");
    if(userFileHandle==NULL){
        printf("Not opening \n");
        return;
    }
    while(fscanf(userFileHandle,"%d %s %d",&user.userId,user.userName,&user.userAge)==3){
        printf("User: ID=%d, Name=%s, Age=%d\n",user.userId,user.userName,user.userAge);
    }
    fclose(userFileHandle);
}

void DeleteUser(){
    struct User user;
    int targetUserId;
    int isUserFound=0;
    printf("Enter ID to delete:\n");
    scanf("%d",&targetUserId);
    FILE *userFileHandle=fopen(userFile,"r");
    if(userFileHandle==NULL){
        printf("Not opening \n");
        return;
    }
    FILE *tempFileHandle=fopen("temp.txt","w");
    if(tempFileHandle==NULL){
        printf("Not opening\n");
        return;
    }
    while(fscanf(userFileHandle,"%d %s %d",&user.userId,user.userName,&user.userAge)==3){
        if(targetUserId==user.userId){
            isUserFound=1;
        }
        else{
            fprintf(tempFileHandle,"%d %s %d\n",user.userId,user.userName,user.userAge);
        }
    }
    fclose(userFileHandle);
    fclose(tempFileHandle);
    remove(userFile);
    rename("temp.txt",userFile);
    if(isUserFound){
        printf("User deleted successfully.\n");
    } else {
        printf("User not found.\n");
    }
}

void UpdateUser(){
    struct User user;
    int targetUserId;
    int isUserFound=0;
    printf("Enter Id to update:\n");
    scanf("%d",&targetUserId);
    FILE *userFileHandle=fopen(userFile,"r");
    if(userFileHandle==NULL){
        printf("Not opening \n");
        return;
    }
    FILE *tempFileHandle=fopen("temp.txt","w");
    if(tempFileHandle==NULL){
        printf("Not opening\n");
        return;
    }
    while(fscanf(userFileHandle,"%d %s %d",&user.userId,user.userName,&user.userAge)==3){
        if(targetUserId==user.userId){
            isUserFound=1;
            int newAge;
            char newName[50];
            printf("Enter new Name and Age:\n");
            scanf("%s",newName);
            scanf("%d",&newAge);
            user.userAge=newAge;
            strcpy(user.userName,newName);
            fprintf(tempFileHandle,"%d %s %d\n",user.userId,user.userName,user.userAge);
        }
        else{
            fprintf(tempFileHandle,"%d %s %d\n",user.userId,user.userName,user.userAge);
        }
    }
    fclose(userFileHandle);
    fclose(tempFileHandle);
    remove(userFile);
    rename("temp.txt",userFile);
    if(isUserFound){
        printf("User updated successfully.\n");
    } else {
        printf("User not found.\n");
    }
}