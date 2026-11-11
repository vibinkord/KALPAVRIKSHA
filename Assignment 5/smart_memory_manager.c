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

int ensure_capacity(){
    if(block_count<block_capacity){
        return 0;
    }
    struct Memory *tmp=realloc(blocks,block_capacity*2*sizeof(struct Memory));
    if(tmp==NULL){
        printf("Memory allocation failed\n");
        return -1;
    }
    blocks=tmp;
    block_capacity*=2;
    return 0;
}

void insert_free_block(size_t pos,size_t start,size_t size){
    for(size_t i=block_count;i>pos;i--){
        blocks[i]=blocks[i-1];
    }
    blocks[pos].start_offset=start;
    blocks[pos].size=size;
    blocks[pos].allocationId=0;
    blocks[pos].status=0;
    block_count++;
}

void remove_block(size_t pos){
    for(size_t i=pos;i+1<block_count;i++){
        blocks[i]=blocks[i+1];
    }
    block_count--;
}

void display_stats(){
    size_t allocated_bytes=0, free_bytes=0;
    size_t allocated_blocks=0, free_blocks=0;
    size_t largest_free=0;
    for(size_t i=0;i<block_count;i++){
        if(blocks[i].status==1){
            allocated_bytes+=blocks[i].size;
            allocated_blocks++;
        } else {
            free_bytes+=blocks[i].size;
            free_blocks++;
            if(blocks[i].size>largest_free){
                largest_free=blocks[i].size;
            }
        }
    }
    printf("Total memory: %zu bytes\n",pool_size);
    printf("Allocated: %zu bytes\n",allocated_bytes);
    printf("Free: %zu bytes\n",free_bytes);
    printf("Allocated blocks: %zu\n",allocated_blocks);
    printf("Free blocks: %zu\n",free_blocks);
    printf("Largest free block: %zu bytes\n",largest_free);
}

void report_fragmentation(size_t size){
    size_t total_free=0;
    for(size_t i=0;i<block_count;i++){
        if(blocks[i].status==0){
            total_free+=blocks[i].size;
        }
    }
    if(total_free>=size){
        printf("Note: The request for %zu bytes failed even though %zu bytes are free, because the free memory is fragmented.\n",size,total_free);
        printf("Free blocks:");
        for(size_t i=0;i<block_count;i++){
            if(blocks[i].status==0){
                printf(" [Start: %zu, Size: %zu]",blocks[i].start_offset,blocks[i].size);
            }
        }
        printf("\n");
    }
}

int allocate_memory(size_t size){
    if(size==0||size>pool_size){
        printf("Invalid size\n");
        return -1;
    }
    long idx=find_block(size);
    if(idx==-1){
        printf("Allocation failed: no contiguous free block is large enough.\n");
        report_fragmentation(size);
        return -1;
    }
    if(blocks[idx].size>size){
        if(ensure_capacity()==-1){
            return -1;
        }
        insert_free_block((size_t)idx+1,blocks[idx].start_offset+size,blocks[idx].size-size);
        blocks[idx].size=size;
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


int resize_memory(int id,size_t new_size){
    if(id<=0||id>=next_id){
        printf("Unknown allocation ID\n");
        return -1;
    }
    if(new_size==0||new_size>pool_size){
        printf("Invalid size\n");
        return -1;
    }
    long found=-1;
    for(size_t i=0;i<block_count;i++){
        if(blocks[i].status==1 && blocks[i].allocationId==id){
            found=(long)i;
            break;
        }
    }
    if(found==-1){
        printf("Allocation already released\n");
        return -1;
    }
    size_t idx=(size_t)found;
    size_t old_size=blocks[idx].size;

    if(new_size==old_size){
        printf("Allocation already has that size.\n");
        return 0;
    }

    /* shrink */
    if(new_size<old_size){
        size_t diff=old_size-new_size;
        if(idx+1<block_count && blocks[idx+1].status==0){
            blocks[idx+1].start_offset-=diff;
            blocks[idx+1].size+=diff;
        } else {
            if(ensure_capacity()==-1){
                printf("Resize failed. Allocation unchanged.\n");
                return -1;
            }
            insert_free_block(idx+1,blocks[idx].start_offset+new_size,diff);
        }
        blocks[idx].size=new_size;
        merge_free_blocks();
        printf("Allocation %d shrunk to %zu bytes.\n",id,new_size);
        return 0;
    }

    /* grow in place */
    size_t need=new_size-old_size;
    if(idx+1<block_count && blocks[idx+1].status==0 && blocks[idx+1].size>=need){
        blocks[idx].size=new_size;
        blocks[idx+1].start_offset+=need;
        blocks[idx+1].size-=need;
        if(blocks[idx+1].size==0){
            remove_block(idx+1);
        }
        printf("Allocation %d expanded in place to %zu bytes.\n",id,new_size);
        return 0;
    }

    /* move to another free block */
    long target=find_block(new_size);
    if(target==-1){
        printf("Resize failed: no contiguous free block is large enough. Allocation unchanged.\n");
        report_fragmentation(new_size);
        return -1;
    }
    size_t t=(size_t)target;
    if(blocks[t].size>new_size){
        if(ensure_capacity()==-1){
            printf("Resize failed. Allocation unchanged.\n");
            return -1;
        }
        insert_free_block(t+1,blocks[t].start_offset+new_size,blocks[t].size-new_size);
        blocks[t].size=new_size;
        if(idx>t){
            idx++;
        }
    }
    for(size_t i=0;i<old_size;i++){
        pool[blocks[t].start_offset+i]=pool[blocks[idx].start_offset+i];
    }
    blocks[t].status=1;
    blocks[t].allocationId=id;
    blocks[idx].status=0;
    blocks[idx].allocationId=0;
    printf("Allocation %d moved to Start: %zu, Size: %zu\n",id,blocks[t].start_offset,new_size);
    merge_free_blocks();
    return 0;
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
            case 3: {
                printf("Allocation ID: ");
                int rid=(int)read_number();
                printf("New size: ");
                resize_memory(rid,(size_t)read_number());
                break;
            }
            case 4:
                display_map();
                break;
            case 5:
                display_stats();
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