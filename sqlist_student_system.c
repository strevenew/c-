#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define id_size 20
#define name_size 20
#define YES 1
#define NO 0

void input_num(const char * mesg,int *n);
void input_string(const char* mesg,char *ch);
void input_double(const char*mesg,double* n);

typedef struct student{
    char id[id_size];
    char name[name_size];
    double grade1;
    double grade2;
} STUDENT,*STUptr;

typedef STUDENT elemtype;

typedef struct node{
    elemtype elem;
    struct node* next;
} NODE,*NODEptr;

typedef struct sqlist{
    NODEptr head;
    int lenght;
}LIST,*LISTptr;

/*定义查询结果链*/
typedef struct info{
    NODEptr elem;
    struct info *next;
}INFO,*INFOptr;

int list_init(LISTptr n){
    /*链表初始化*/
    n->head=(NODEptr)malloc(sizeof(NODE));
    if(n->head==NULL)return NO;
    n->head->next=NULL;
    n->lenght=0;
    return YES;
}

void info_init(INFOptr n){
    /*回答集初始化*/
    n->elem=NULL;
    n->next=NULL;
}



int cmp1(elemtype a,elemtype b){
    if(!strcmp(a.id,b.id))
        return YES;
    else return NO;
}

int cmp2(elemtype a,elemtype b){
    if(!strcmp(a.name,b.name))
        return YES;
    else return NO;
}

int cmp3(elemtype a,elemtype b){
    if((int)a.grade1*100==(int)b.grade1*100)
        return YES;
    else return NO;
}

int cmp4(elemtype a,elemtype b){
    if((int)a.grade2*100==(int)b.grade2*100)
        return YES;
    else return NO;
}



void list_search(LISTptr n,INFOptr ans/*答案集*/,int (*cmp)(elemtype,elemtype)/*函数指针*/,elemtype a/*查询对象*/){
    NODEptr u=n->head;
    info_init(ans);// 答案集初始化
    INFOptr cur=ans;
    for(int i=0;i<n->lenght;i++){
        u=u->next;
        if(cmp(a,u->elem)){ //记录满足答案的节点指针
            INFOptr p=(INFOptr)malloc(sizeof(INFO));
            if(p==NULL){printf("Memory application failed\n");return;}
            p->elem=u;
            cur->next=p;
            p->next=NULL;
            cur=cur->next;
        }
    }
}

int list_push_back(LISTptr n,elemtype stu){
    /*向链表尾插入元素*/
    NODEptr u=n->head;
    for(int i=0;i<n->lenght;i++)u=u->next;  //指向最后一个节点
    NODEptr p=(NODEptr)malloc(sizeof(NODE));
    if(p==NULL)return NO;
    p->elem=stu;
    p->next=NULL;
    u->next=p;
    n->lenght++;
    return YES;
}


int list_delete(LISTptr n,char* id){
    NODEptr u=n->head;
    int ok=0;
    while(u->next!=NULL){
        if(strcmp(u->next->elem.id,id)==0){
            ok=1;
            NODEptr p=u->next;
            u->next=p->next;
            p->next=NULL;
            free(p);
            n->lenght--;
            break;
        }
        u=u->next;
    }
    return ok;
}




int cmp5(elemtype a,elemtype b){
    if((int)a.grade1*100==(int)b.grade1*100)
        return (int)a.grade2*100>(int)b.grade2*100;
    else return a.grade1>b.grade1;
}
int cmp6(elemtype a,elemtype b){
    return (int)a.grade1*100>(int)b.grade1*100;
}
int cmp7(elemtype a,elemtype b){
    return (int)a.grade2*100>(int)b.grade2*100;
}
void list_sort(LISTptr n,int (*cmp)(elemtype,elemtype)){
    NODEptr u=n->head;
    for(int i=0;i<n->lenght-1 ;i++){
        u=u->next;
        NODEptr p=u;
        for(int j=i+1;j<n->lenght;j++){
            p=p->next;
            if(cmp(p->elem,u->elem)){
                elemtype temp=u->elem;
                u->elem=p->elem;
                p->elem=temp;
            }
        }
    }

}


void search(LISTptr n,int ch){
    elemtype elem;
    INFO ans;
    switch(ch){
        case 1:
            input_string("please input your id",elem.id);
            list_search(n,&ans,cmp1,elem);
        break;
        case 2:
            input_string("please input your name",elem.name);
            list_search(n,&ans,cmp2,elem);
        break;
        case 3:
            input_double("please input your grade1",&(elem.grade1));
            list_search(n,&ans,cmp3,elem);
        break;
        case 4:
            input_double("please input your grade2",&(elem.grade2));
            list_search(n,&ans,cmp4,elem);
        break;
    }
    if(ans.next==NULL)
        printf("No query results!\n");
    else {
        printf("query results:\n");
        INFOptr cur=&ans;
        while(cur->next!=NULL){
            cur=cur->next;
            elemtype valse=cur->elem->elem;
            printf("\tid:%s,name:%s,grade1:%.2lf,grade2:%.2lf\n",valse.id,valse.name,valse.grade1,valse.grade2);
        }
    }
    printf("\n");
}

void search_menu(LISTptr n){
    while(1){
        printf("\t1.search by id\n\t2.search by name\n\t3.search by grade1\n\t4.search by grade2\n\t5.exit\n");
        int choice;
        input_num("please input your choice",&choice);
        switch(choice){
            case 1:
            case 2:
            case 3:
            case 4:
                search(n,choice);
                break;
            case 5:return;
        }
    }
}


void push_back_emnu(LISTptr n){
    elemtype elem;
    input_string("please input your id",elem.id);
    input_string("please input your name",elem.name);
    input_double("please input your grade1",&(elem.grade1));
    input_double("please input your grade2",&(elem.grade2));
    INFO ans;
    list_search(n,&ans,cmp1,elem);
    if(ans.next!=NULL){
        printf("Existence of the same data!\n");
        return;
    }
    if(list_push_back(n,elem))
        printf("insert successful!\n");
    else printf("insert failed!\n");
}


void delete_menu(LISTptr n){
    char id[id_size];
    input_string("please input you want delete id",id);
    if(list_delete(n,id))
        printf("delete successful!\n");
    else
        printf("delete failed, no this id!\n");
}


void sort_menu(LISTptr n){
    list_sort(n,cmp5);
    printf("list informations:\n");
    NODEptr u=n->head;
    for(int i=0;i<n->lenght;i++){
        u=u->next;
        printf("id:%s, name:%s, grade1:%.2lf, grade2:%.2lf\n",u->elem.id,u->elem.name,u->elem.grade1,u->elem.grade2);
    }

}


int _max(int a,int b){
    return a>=b?a:b;
}
void Statistics_menu(LISTptr n){
    //grade1成绩统计
    list_sort(n,cmp6);
    printf("grade1:\n");
    const char* mesg[5]={"less 60","60----70","70----80","80----90","more 90"};
    int used[5]={0};
    NODEptr u=n->head;
    for(int i=0;i<n->lenght;i++){
        u=u->next;
        int index=_max(0,((int)u->elem.grade1/10)-5);
        if(!used[index]){
            printf("%s:\n",mesg[index]);
            used[index]=1;
        }
        printf("\tid:%s, name:%s, grade1:%.2lf, grade2:%.2lf\n",u->elem.id,u->elem.name,u->elem.grade1,u->elem.grade2);
    }
    // grade2成绩统计
    list_sort(n,cmp7);
    for(int i=0;i<5;i++)used[i]=0;
    printf("\ngrade2:\n");
    u=n->head;
    for(int i=0;i<n->lenght;i++){
        u=u->next;
        int index=_max(0,((int)u->elem.grade2/10)-5);
        if(!used[index]){
            printf("%s:\n",mesg[index]);
            used[index]=1;
        }
        printf("\tid:%s, name:%s, grade1:%.2lf, grade2:%.2lf\n",u->elem.id,u->elem.name,u->elem.grade1,u->elem.grade2);
    }
    //平均分统计
    u=n->head;
    double sum_1=0.0,sum_2=0.0;
    int count=n->lenght;
    for(int i=0;i<n->lenght;i++){
        u=u->next;
        sum_1+=u->elem.grade1;
        sum_2+=u->elem.grade2;
    }
    double avg_1=sum_1/count,avg_2=sum_2/count;
    printf("\n\ttotal: avg_grade1:%.2lf, avg_grade2:%.2lf\n",avg_1,avg_2);
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



int main(){
    LIST n;
    if(list_init(&n))printf("init successful!\n");
    else printf("init fails!\n");
    while(1){
        printf("###Student Management System###\n");
        printf("\t1.query\n\t2.insert\n\t3.delete\n\t4.sort\n\t5.Average score and statistics\n\t6.exit\n");
        int choice;
        input_num("please input your choice",&choice);
        switch(choice){
            case 1:search_menu(&n);break;
            case 2:push_back_emnu(&n);break;
            case 3:delete_menu(&n);break;
            case 4:sort_menu(&n);break;
            case 5:Statistics_menu(&n);break;
            case 6: return 0;
        }
    }
    return 0;
}

