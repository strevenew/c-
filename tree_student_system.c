#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define id_size 20
#define name_size 20


#define YES 1
#define NO 0



typedef struct stu{
    char id[id_size],name[name_size];
    struct stu *next;
}student, *stuptr;

typedef struct list{
    stuptr head;
}LIST,*LISTptr;

typedef struct grade_node{
    double grade;
    struct grade_node *lchild,*rchild;
    LIST stu;
}NODE,*NODEptr;

typedef struct tree{
    NODEptr root;
}TREE,*TREEptr;


int stu_create(stuptr *u){
    stuptr T=*u;
    (*u)=T=(stuptr)malloc(sizeof(student));
    if(T==NULL)return NO;
    T->next=NULL;
    return YES;
}

void list_init(LISTptr T){
    T->head=NULL;
}


int NODE_create(NODEptr *u){
    NODEptr T=*u;
    (*u)=T=(NODEptr)malloc(sizeof(NODE));
    if(T==NULL)return NO;
    T->lchild=T->rchild=NULL;
    list_init(&(T->stu));
    return YES;
}

void tree_init(TREEptr T){
    T->root=NULL;
}


void input_num(const char * mesg,int *n){
    int ok=0;
    while(!ok){
        printf("%s:\n",mesg);
        if(scanf("%d",n))ok=1;
        else
            printf("input error\n");
        fflush(stdin);
    }
}

void input_string(const char* mesg,char *ch){
    int ok=0;
    while(!ok){
        printf("%s:\n",mesg);
        if(scanf("%s",ch))ok=1;
        else
            printf("input error\n");
        fflush(stdin);
    }
}

void input_double(const char*mesg,double* n){
    int ok=0;
    while(!ok){
        printf("%s:\n",mesg);
        if(scanf("%lf",n)&&*n>=0&&*n<=100)ok=1;
        else
            printf("input error\n");
        fflush(stdin);
    }
}





stuptr list_search(stuptr head,char *id){
    while(head!=NULL){
        if(strcmp(head->id,id)==0)break;
        head=head->next;
    }
    return head;
}

NODEptr tree_search(NODEptr root,double grade){
    while(root!=NULL && (int)root->grade*100!=(int)grade*100){
        if((int)root->grade*100>(int)grade*100)root=root->lchild;
        else root=root->rchild;
    }
    return root;
}

int search_id(NODEptr T,char *id){
    if(T==NULL)return NO;
    if(list_search(T->stu.head,id)!=NULL)return YES;
    else return search_id(T->lchild,id)||search_id(T->rchild,id);
}

void list_push_back(LISTptr T,stuptr x){
    stuptr cur=T->head;
    if(cur==NULL) {
        T->head=x;
        return;
    }
    while(cur->next!=NULL)cur=cur->next;
    cur->next=x;
}


int tree_insert(TREE *T,double grade,char *id,char *name){
    NODEptr pos=tree_search(T->root,grade);

    stuptr new_stu;
    if(!stu_create(&new_stu))return NO;
    strcpy(new_stu->id,id);
    strcpy(new_stu->name,name);

    if(pos==NULL){
        NODEptr the_new;
        if(!NODE_create(&the_new))return NO;
        the_new->grade=grade;
        the_new->stu.head=new_stu;

        NODEptr parent=NULL,cur=T->root;
        while(cur!=NULL){
            parent=cur;
            if((int)cur->grade*100>(int)grade*100)
                cur=cur->lchild;
            else cur=cur->rchild;
        }
        if(parent==NULL)T->root=the_new;
        else if((int)parent->grade*100>(int)grade*100)
                parent->lchild=the_new;
        else    parent->rchild=the_new;
    }
    else{
        if(list_search(pos->stu.head,id)!=NULL)return NO;
        list_push_back(&(pos->stu),new_stu);
    }
    return YES;
}

int list_delete(LISTptr T,char* id){
    if(T->head==NULL)return NO;
    stuptr u=T->head;
    if(strcmp(u->id,id)==0){
        T->head=u->next;
        free(u);
        return YES;
    }
    else{
        while(u->next!=NULL){
            stuptr p=u->next;
            if(strcmp(p->id,id)==0){
                u->next=p->next;
                p->next=NULL;
                free(p);
                return YES;
            }
            else u=p;
        }
        return NO;
    }
}

int tree_delete(TREEptr T,double grade){
    NODEptr u=T->root,parent=NULL;
    while(u!=NULL){
        if((int)(u->grade*100)==(int)grade*100)break;
        parent=u;
        if((int)(u->grade*100)>(int)grade*100)
            u=u->lchild;
        else u=u->rchild;
    }
    if(u==NULL)return NO;

    if(u->lchild==NULL||u->rchild==NULL){
        NODEptr child;
        if(u->lchild==NULL)
            child=u->rchild;
        else if(u->rchild==NULL)
            child=u->lchild;
        if(parent==NULL)T->root=child;
        else if(parent->lchild==u)parent->lchild=child;
        else if(parent->rchild==u)parent->rchild=child;
        return YES;
    }
    else {
        NODEptr pos=u->rchild,pos_parent=u;
        while(pos->lchild!=NULL){
            pos_parent=pos;
            pos=pos->lchild;
        }
        double grade_temp=pos->grade;
        LIST stu_temp=pos->stu;
        pos->grade=u->grade;
        pos->stu=u->stu;
        u->grade=grade_temp;
        u->stu=stu_temp;
        pos_parent->lchild=pos->rchild;
    }

    return YES;
}

int print_list(LIST T){
    stuptr u=T.head;
    int count=0;
    while(u!=NULL){
        count++;
        printf("\tid:%s,name:%s\n",u->id,u->name);
        u=u->next;
    }
    return count;
}


void sort_dfs(NODEptr u,int *count,double *sum){
    if(u==NULL)return;
    sort_dfs(u->rchild,count,sum);
    printf("%.2lf\n",u->grade);
    int add=print_list(u->stu);
    (*count)+=add;
    (*sum)=(*sum)+(add*(u->grade));
    sort_dfs(u->lchild,count,sum);
}

void sort_menu(TREE T){
    int count=0;
    double sum=0.0;
    sort_dfs(T.root,&count,&sum);
    printf("avg: %.2lf\n",sum/count);
}

void search_menu(TREE T){
    double grade;
    input_double("please input your want search grade",&grade);
    NODEptr ans=tree_search(T.root,grade);
    if(ans==NULL)
        printf("no such information!\n");
    else {
        printf("information:\n");
        print_list(ans->stu);
    }
}


void insert_menu(TREEptr T){
    double grade;
    char id[id_size],name[name_size];
    input_string("please input your id",id);
    if(search_id(T->root,id)){
        printf("already have this id\n");
        return;
    }
    input_string("please input your name",name);
    input_double("please input your grade",&grade);
    if(!tree_insert(T,grade,id,name))printf("insert failed!\n");
    else printf("insert successful!\n");
}

void delete_menu(TREEptr T){
    double grade;
    char id[id_size];
    input_string("please input your id",id);
    input_double("please input your grade",&grade);
    NODEptr pos=tree_search(T->root,grade);
    if(pos==NULL){
        printf("no this grade!\n");
        return;
    }
    else {
        stuptr ans=list_search(pos->stu.head,id);
        if(ans==NULL){
            printf("no this id!\n");
            return;
        }
        else {
            if(!list_delete(&(pos->stu),id)){
                printf("delete student list failed!\n");
                return;
            }
            if(pos->stu.head==NULL){
                if(!tree_delete(T,grade)){
                    printf("delete grade tree failed!\n");
                    return;
                }
            }
        }
    }
    printf("delete successful!\n");
}
void dfs1(NODEptr T){
    if(T==NULL)return;
    printf("%.2lf ",T->grade);
    dfs1(T->lchild);
    dfs1(T->rchild);
}
void p_struct(TREE T){
    printf("this is tree struct\n");
    dfs1(T.root);
    printf("\n");
}

void dfs(NODEptr T,double l,double m){
    if(T==NULL)return;
    dfs(T->rchild,l,m);
    if((int)(T->grade*100)>=(int)l*100&&(int)(T->grade*100)<=(int)(m*100)){
        printf("%.2lf\n",T->grade);
        print_list(T->stu);
    }
    dfs(T->lchild,l,m);
}

void Statistics(TREE T){
    double l,m;
    input_double("please input lower limit grade",&l);
    input_double("please input upper limit grade",&m);
    printf("Statistics information:\n");
    dfs(T.root,l,m);
}

int main(){
    TREE tree;
    tree_init(&tree);
    while(1){
        printf("###Student Management System###\n");
        printf("\t1.query\n\t2.insert\n\t3.delete\n\t4.sort\n\t5.print tree structure\n\t6.Statistics\n\t7.exit\n");
        int choice;
        input_num("please input your choice",&choice);
        switch(choice){
            case 1:search_menu(tree);break;
            case 2:insert_menu(&tree);break;
            case 3:delete_menu(&tree);break;
            case 4:sort_menu(tree);break;
            case 5:p_struct(tree);break;
            case 6:Statistics(tree);break;
            case 7:return 0;
        }
    }
    return 0;
}
