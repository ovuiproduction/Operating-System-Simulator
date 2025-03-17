
//structure for process info
struct proc{
    int pid;
    int at;
    int bt; 
    int wt;
    int tat;
    int ct;
}ganchart[programCount];

// structure for ready queue
struct Node{
    struct proc node;
    struct Node* next;
};

struct proc process_arr[programCount];
struct Node *head = NULL;
struct Node* temp = NULL;
int rare = 0;

//insert function 
void insert(struct proc node){

    struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
    newnode->node = node;
    newnode->next = NULL;
    
    if(head == NULL){
        head = newnode;
    }
    else{
        temp = head;
        while(temp->next!= NULL){
            temp = temp->next;
        }
        temp->next = newnode;
    }
    rare++;
}


void deleteData(int pid){
    temp = head;
    while(temp!=NULL){
        if(temp->node.pid == pid){
            if(temp == head){
                head = head->next;
                free(temp);
                break;
            }
            else if(temp->next == NULL){
                struct Node* temp2 = head;
                struct Node* prev = temp2;
                while(temp2->next!=NULL){
                    prev = temp2;
                    temp2 = temp2->next;
                }
                prev->next = NULL;
                free(temp);
                break;
            }
            else{
                struct Node* temp2 = head;
                struct Node* prev = temp2;
                while(temp2->node.pid != pid){
                    prev = temp2;
                    temp2 = temp2->next;
                }
                prev->next = temp2->next;
                free(temp);
                break;
            }
        }
        temp = temp->next;
    }
    rare--;
}

int pc =0;
void addprocess(struct proc process_arr[],int pid , int at, int bt){
    process_arr[pc].pid = pid;
    process_arr[pc].at = at;
    process_arr[pc].bt = bt;
    process_arr[pc].wt = 0;
    process_arr[pc].tat = 0;
    process_arr[pc].ct = 0;
    pc++;
}