struct proc findmin(){
    
    temp = head;
    struct proc min = head->node;
    
    while(temp!= NULL)
    {
        if(min.at > temp->node.at){
            min = temp->node; 
        }
        temp = temp->next;
    }
    return min;
}

int gpc = 0;
void fcfs(struct proc arr[]){

    //sort process w.r.t their at

    for(int i =0;i<pc;i++){
        for(int j=i+1;j<pc;j++){
            if(arr[j].at < arr[i].at){
                struct proc tempnode = arr[j];
                arr[j] = arr[i];
                arr[i] = tempnode;
            }
        }
    }

    int visited[pc+1];
    for(int i =0;i<pc+1;i++){
        visited[i] =0;
    }
    visited[0] = 99;

    int ct =arr[0].at;

    insert(arr[0]);
    visited[arr[0].pid] = 1;

    while(rare != 0){

        struct proc curr = findmin();

        ct = ct + curr.bt;
        curr.ct = ct;
        curr.wt = curr.ct - (curr.at + curr.bt);
        curr.tat = curr.bt + curr.wt;

        ganchart[gpc++] = curr;

        for(int i =0;i<pc;i++){
            if(arr[i].at <= ct && visited[arr[i].pid] == 0){
                insert(arr[i]);
                visited[arr[i].pid] = 1;
            }
        }
        deleteData(curr.pid);
    }
}