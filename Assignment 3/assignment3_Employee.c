#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_EMP 100
#define MAX_REPORTS 10

typedef struct{
    int empId;
    char empName[100];
    char designation[100];
    int reportingEmpId[MAX_REPORTS];
    int reportStored;
    int reportingManagerId;
}Employee;

void addEmployee(Employee *emp,int *count);
void storeEmployee(Employee *emp, int *count, int id, char *name, char *designation, int managerId);
void initialEmployee(Employee *emp, int *count);
void displayEmployeeHierarchy(Employee *emp, int count, Employee *current,int level);
void displayCompleteHierarchy(Employee *emp, int count);
void removeFrom(Employee *mgr, int id);
void removeEmployee(Employee *emp,int *count);
int countEmploye(Employee *emp, int count, Employee *current);

Employee *findEmployee(Employee *emp,int empId,int count){
    for(int i=0;i<count;i++){
        if(emp[i].empId==empId){
            return &emp[i];
        }
    }
    return NULL;
}


int main(){
    printf("==== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
    int choice;
    int id;

    int count=0;
    Employee emp[MAX_EMP];
    // initialEmployee(emp, &count);
    while(1){
        printf("\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        switch(choice){
                case 1:
                    addEmployee(emp,&count);
                    break;
                case 2:
                    removeEmployee(emp,&count);
                    break;
                case 3:
                    printf("Enter Emp id to find hierarchy: ");
                    scanf("%d", &id);
                    Employee *target = findEmployee(emp, id, count);
                    if(target == NULL){
                        printf("Employee not found\n");
                    }else{
                        displayEmployeeHierarchy(emp, count, target,0);
                        printf("Total Employees Under %d: %d\n", id, countEmploye(emp, count, target));
                    }
                    break;
                case 4:
                        displayCompleteHierarchy(emp, count);
                        break;
                case 5:
                    exit(0);
                default:
                    printf("Invalid choice\n");
                    break;
        }
    }
    return 0;
}

void removeEmployee(Employee *emp,int *count){
    int id;
    printf("Enter Employee ID to remove\n");
    scanf("%d",&id);
    Employee *target=findEmployee(emp,id,*count);
    if(target==NULL){
        printf("Employee not found\n");
        return;
    }
    if(target->reportingManagerId==0&&target->reportStored>0){
        printf("Cannot remove the CEO with direct reports\n");
        return;
    }
    Employee *manager=NULL;
    if(target->reportingManagerId!=0){
        manager=findEmployee(emp,target->reportingManagerId,*count);
        if(manager->reportStored-1+target->reportStored>MAX_REPORTS){
            printf("Cannot remove employee\n");
            return;
        }
    }
    if(manager!=NULL){
        removeFrom(manager,target->empId);
    }
    for(int i=0;i<target->reportStored;i++){
        int reportId=target->reportingEmpId[i];
        Employee *report=findEmployee(emp,reportId,*count);
        report->reportingManagerId=manager->empId;
        manager->reportingEmpId[manager->reportStored]=reportId;
        manager->reportStored++;
    }
    *target = emp[*count - 1];
    (*count)--;
}

void removeFrom(Employee *mgr, int id){
    if(mgr==NULL){
        return;
    }
    for(int i=0;i<mgr->reportStored;i++){
        if(mgr->reportingEmpId[i]==id){
            for(int j=i;j<mgr->reportStored-1;j++){
                mgr->reportingEmpId[j]=mgr->reportingEmpId[j+1];
            }
            mgr->reportStored--;
            return;
        }
    }
}

void displayEmployeeHierarchy(Employee *emp, int count, Employee *current,int level){
    if(current==NULL){
        return;
    }
    for(int s = 0; s < level * 4; s++){
    printf(" ");
    }
    printf("%d - %s - %s\n",current->empId,current->empName,current->designation);
    for(int i=0;i<current->reportStored;i++){
        Employee *report=findEmployee(emp,current->reportingEmpId[i],count);
        displayEmployeeHierarchy(emp,count,report,level+1);
    }
}

void displayCompleteHierarchy(Employee *emp, int count){
    printf("\n===== ORGANIZATION HIERARCHY =====\n");
    for(int i=0;i<count;i++){
        if(emp[i].reportingManagerId==0){
            displayEmployeeHierarchy(emp,count,&emp[i],0);
        }

    }
    printf("Total Employees: %d\n",count);
}

int countEmploye(Employee *emp, int count, Employee *current){
    if(current==NULL){
        return 0;
    }
    int total=0;
    for(int i=0;i<current->reportStored;i++){
        Employee *report=findEmployee(emp,current->reportingEmpId[i],count);
        total+=1+countEmploye(emp,count,report);
    }
    return total;
}

void addEmployee(Employee *emp,int *count){
    if(*count>=MAX_EMP){
        printf("Employee limit reached\n");
        return;
    }
    int id;
    char name[100],designation[100];
    int reportingManager;
    printf("Enter Employee ID: ");
    scanf("%d",&id);
    if(id<=0){
        printf("Employee ID must be greater than 0\n");
        return;
    }
    if(findEmployee(emp,id,*count)!=NULL){
        printf("Employee ID already exists\n");
        return;
    }
    getchar();
    printf("Enter Name: ");
    fgets(name,100,stdin);
    name[strcspn(name, "\n")] = '\0';
    printf("Enter Designation: ");
    fgets(designation,100,stdin);
    designation[strcspn(designation, "\n")] = '\0';
    printf("Enter Reporting Manager ID (0 for none): ");
    scanf("%d",&reportingManager);
    getchar();

    Employee *mgr=NULL;
    if(reportingManager!=0){
        mgr=findEmployee(emp,reportingManager,*count);
        if(mgr==NULL){
            printf("Reporting Manager ID does not exist\n");
            return;
        }
        if(mgr->reportStored>=MAX_REPORTS){
            printf("Reporting Manager has reached maximum direct reports\n");
            return;
        }
    }
    if(reportingManager==0 && *count>0){
    printf("Only the first employee can have no manager\n");
    return;
}
    storeEmployee(emp, count, id, name, designation, reportingManager);

}
void storeEmployee(Employee *emp, int *count, int id, char *name, char *designation, int managerId){
    emp[*count].empId=id;
    strcpy(emp[*count].empName,name);
    strcpy(emp[*count].designation,designation);
    emp[*count].reportingManagerId=managerId;
    emp[*count].reportStored=0;
    Employee *mgr=findEmployee(emp,managerId,*count);
    if(mgr!=NULL){
        mgr->reportingEmpId[mgr->reportStored]=id;
        mgr->reportStored++;
    }
    (*count)++;
}

void initialEmployee(Employee *emp,int *count){
    storeEmployee(emp, count, 101, "Rahul", "CEO", 0);
    storeEmployee(emp, count, 102, "Amit", "Engineering Manager", 101);
    storeEmployee(emp, count, 103, "Priya", "HR Manager", 101);
    storeEmployee(emp, count, 104, "Neha", "Developer", 102);
    storeEmployee(emp, count, 105, "Raj", "Developer", 102);
    storeEmployee(emp, count, 106, "Karan", "HR Executive", 103);
}