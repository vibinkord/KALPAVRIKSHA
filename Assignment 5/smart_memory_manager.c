#include<stdio.h>
#include<stdlib.h>

struct Memory {
    size_t start_offset;
    size_t size;
    int allocationId;
    int status;
};

unsigned char *pool;
size_t pool_size;
struct Memory *blocks;
size_t block_count;
size_t block_capacity;
int next_id=1;
int strategy;


int init_pool(size_t size){
    pool=malloc(size);
    if(pool==NULL){
        return -1;
    }
    block_capacity=4;
    blocks=malloc(4*sizeof(struct Memory));
    if(blocks==NULL){
        free(pool);
        return -1;
    }
    pool_size=size;
    block_count=1;
    blocks[0].start_offset=0;
    blocks[0].size=size;
    blocks[0].allocationId=0;
    blocks[0].status=0;
    return 0;
}

void display_map(){
    printf("Memory Map:\n");
    for(size_t i=0; i<block_count; i++){
        if(blocks[i].status==0){
            printf("Block %zu: Offset=%zu, Size=%zu, Allocation ID=%d, Status=Free\n", i, blocks[i].start_offset, blocks[i].size, blocks[i].allocationId);
        } else {
            printf("Block %zu: Offset=%zu, Size=%zu, Allocation ID=%d, Status=Allocated\n", i, blocks[i].start_offset, blocks[i].size, blocks[i].allocationId);
        }
    }
}

long first_fit(size_t size){
    for(size_t i=0;i<block_count;i++){
        if(blocks[i].status==0 && blocks[i].size>=size){
            return (long)i;
        }
    }
    return -1;
}

long best_fit(size_t size){
    long best=-1;
    for(size_t i=0;i<block_count;i++){
        if(blocks[i].status==0 && blocks[i].size>=size){
            if(best==-1 || blocks[i].size<blocks[best].size){
                best=(long)i;
            }
        }
    }
    return best;
}

long worst_fit(size_t size){
    long worst=-1;
    for(size_t i=0;i<block_count;i++){
        if(blocks[i].status==0 && blocks[i].size>=size){
            if(worst==-1 || blocks[i].size>blocks[worst].size){
                worst=(long)i;
            }
        }
    }
    return worst;
}

long find_block(size_t size){
    if(strategy==1){
        return first_fit(size);
    }
    if(strategy==2){
        return best_fit(size);
    }
    return worst_fit(size);
}

int allocate_memory(size_t size){
    if(size==0||size>pool_size){
        printf("Invalid size\n");
        return -1;
    }
    long idx=find_block(size);
    if(idx==-1){
        printf("cannot allocate memory\n");
        return -1;
    }
    if(blocks[idx].size>size){
        if(block_count==block_capacity){
            struct Memory *tmp=realloc(blocks,block_capacity*2*sizeof(struct Memory));
            if(tmp==NULL){
                printf("Memory allocation failed\n");
                return -1;
            }
            blocks=tmp;
            block_capacity*=2;
        }
        for(size_t i=block_count;i>(size_t)idx+1;i--){
            blocks[i]=blocks[i-1];
        }
        blocks[idx+1].start_offset=blocks[idx].start_offset+size;
        blocks[idx+1].size=blocks[idx].size-size;
        blocks[idx+1].allocationId=0;
        blocks[idx+1].status=0;
        blocks[idx].size=size;
        block_count++;
    }
    blocks[idx].status=1;
    blocks[idx].allocationId=next_id++;
    printf("Allocation successful. ID: %d, Start: %zu, Size: %zu\n",
    blocks[idx].allocationId, blocks[idx].start_offset, blocks[idx].size);
    return 0;
}

void merge_free_blocks(){
    size_t i=0;
    while(i+1<block_count){
        if(blocks[i].status==0 && blocks[i+1].status==0){
            blocks[i].size+=blocks[i+1].size;
            for(size_t j=i+1;j+1<block_count;j++){
                blocks[j]=blocks[j+1];
            }
            block_count--;
        } else {
            i++;
        }
    }
}

int release_memory(int id){
    if(id<=0||id>=next_id){
        printf("Unknown allocation ID\n");
        return -1;
    }
    for(size_t i=0;i<block_count;i++){
        if(blocks[i].status==1 && blocks[i].allocationId==id){
            blocks[i].status=0;
            blocks[i].allocationId=0;
            merge_free_blocks();
            printf("Allocation released.\n");
            return 0;
        }
    }
    printf("Allocation already released\n");
    return -1;
}

long read_number(){
    long n;
    if(scanf("%ld",&n)!=1){
        int c;
        while((c=getchar())!='\n' && c!=EOF);
        return -1;
    }
    return n;
}

void choose_strategy(){
    long s;
    do{
        printf("Select allocation strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): ");
        s=read_number();
        if(s<1||s>3){
            printf("Invalid choice.\n");
        }
    }while(s<1||s>3);
    strategy=(int)s;
    if(strategy==1){
        printf("Strategy set to First Fit.\n");
    } else if(strategy==2){
        printf("Strategy set to Best Fit.\n");
    } else {
        printf("Strategy set to Worst Fit.\n");
    }
}

int main(){
    long n;
    do{
        printf("Enter memory pool size: ");
        n=read_number();
        if(n<1||n>1000000){
            printf("Invalid pool size. Must be between 1 and 1000000.\n");
        }
    }while(n<1||n>1000000);

    if(init_pool((size_t)n)==-1){
        printf("Failed to initialize memory pool\n");
        return -1;
    }
    choose_strategy();

    long choice;
    do{
        printf("\nMenu:\n");
        printf("1. Allocate memory\n");
        printf("2. Release memory\n");
        printf("3. Resize allocation\n");
        printf("4. Display memory map\n");
        printf("5. Display memory statistics\n");
        printf("6. Change allocation strategy\n");
        printf("7. Exit\n");
        printf("Choice: ");
        choice=read_number();

        switch(choice){
            case 1:
                printf("Requested size: ");
                allocate_memory((size_t)read_number());
                break;
            case 2:
                printf("Allocation ID: ");
                release_memory((int)read_number());
                break;
            case 3:
                printf("Not implemented yet\n");
                break;
            case 4:
                display_map();
                break;
            case 5:
                printf("Not implemented yet\n");
                break;
            case 6:
                choose_strategy();
                break;
            case 7:
                break;
            default:
                printf("Invalid choice\n");
        }
    }while(choice!=7);

    free(pool);
    free(blocks);
    return 0;
}