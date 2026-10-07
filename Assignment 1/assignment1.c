#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define MaxSubjects 3

typedef struct {
    int rollNo;
    char name[100];
    int marks[MaxSubjects];
    int total_marks;
    float average_marks;
    char grade;
    char Performance[6];
} Student;
void CalculateTotal(Student *student);
void CalculateAvg(Student *student);
void CalculateGrade(Student *student);
void CalculatePerformance(Student *student);
void printStudents(Student students[],int n);
void listAllRollNo(Student students[], int n, int i);
void getInput(Student *student);
int checkRollNo(Student students[], int count, int rollNo);

int main() {

    int n;
    scanf("%d", &n);
    
    Student students[n];
    for(int i=0;i<n;i++){
        int rollNo;
        while (1) {
            scanf("%d", &rollNo);
            if (!checkRollNo(students, i, rollNo)) break;
            printf("Roll number already exists\n");
    }
        students[i].rollNo = rollNo;
        getInput(&students[i]);
    }
    for(int i=0;i<n;i++){
        CalculateTotal(&students[i]);
        CalculateAvg(&students[i]);
        CalculateGrade(&students[i]);
        CalculatePerformance(&students[i]);
    }
    printStudents(students,n);
    printf("List of Roll Numbers (via recursion): ");
    listAllRollNo(students, n, 0);
    return 0;
}

void CalculateTotal(Student *student){
    student->total_marks=0;
    for(int i=0;i<MaxSubjects;i++){
        student->total_marks+=student->marks[i];
    }
}

void CalculateAvg(Student *student){
    student->average_marks= student->total_marks/3.0;
}

void CalculateGrade(Student *student){
    if(student->average_marks>=85){
            student->grade = 'A';
        }
        else if(student->average_marks>=70&&student->average_marks<85){
            student->grade = 'B';
        }
        else if(student->average_marks>=50&&student->average_marks<70){
            student->grade = 'C';
        }
        else if(student->average_marks>=35&&student->average_marks<50){
            student->grade = 'D';
        }
        else{
            student->grade = 'F';
        }
}

void CalculatePerformance(Student *student){
    if(student->grade=='A'){
        strcpy(student->Performance,"*****");
    }
    else if(student->grade=='B'){
        strcpy(student->Performance,"****");
    }
    else if(student->grade=='C'){
        strcpy(student->Performance,"***");
        
    }
    else if(student->grade=='D'){
        strcpy(student->Performance,"**");
        }
    else if(student->grade=='F'){
            strcpy(student->Performance," ");
        }
}

void printStudents(Student students[],int n){
    for(int i=0;i<n;i++){
    printf("Roll: %d\n",students[i].rollNo);
    printf("Name: %s\n",students[i].name);
    printf("Total: %d\n",students[i].total_marks);
    printf("Average: %.2f\n",students[i].average_marks);
    printf("Grade: %c\n",students[i].grade);
    if(students[i].grade=='F'){
        printf("\n");
        continue;
        }
    else{
        printf("Performance: %s\n\n",students[i].Performance);
        }
    }
}

int checkRollNo(Student students[], int count, int rollNo) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == rollNo) {
            return 1;
        }
    }
    return 0;
}

void getInput(Student *student) {
        scanf("%99s", student->name);
        for (int j = 0; j < MaxSubjects; j++) {
            scanf("%d", &student->marks[j]);
        }
}

void listAllRollNo(Student students[], int n, int i) {
    if(i==n){
        return;
    }
    printf("%d ", students[i].rollNo);
    listAllRollNo(students, n, i + 1);
}