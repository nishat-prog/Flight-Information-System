#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct qelement
{
    double cost;
    char s[20],d[20],air[20],dt[8],at[8],route[100],dur[10];
    struct qelement *next;
};
struct queue
{
    struct qelement *front,*rear;

};
void display_details(FILE *,int *,int *,char[],char[]);
void displaycost(FILE *,int,int *,char[],char[],struct queue*);
void display_route(FILE *,int *,char *,char *,struct queue*);
void display_no_filter(FILE *,int *,int,char[],char[]);
void changecase(char[]);
int return_time(char *);
int tellday(int);
int calday(int ,int,int );
int fcorrect(int,int);

void qinit(struct queue *qp)
{
    qp->front=NULL;
    qp->rear=NULL;
}

void qinsert(struct queue *qp,double num,char s[],char d[],char a[],char dt[],char at[],char r[],char du[] )
{
   struct qelement *temp,*sp,*trv;
    temp=(struct qelement *)malloc(sizeof(struct qelement));
    temp->cost=num;
    strcpy(temp->s,s);
    strcpy(temp->d,d);
    strcpy(temp->air,a);
    strcpy(temp->dt,dt);
    strcpy(temp->at,at);
    strcpy(temp->route,r);
    strcpy(temp->dur,du);
    temp->next=NULL;
    sp=NULL;
    if(qp->rear==NULL)
      {
         qp->rear=temp;
         qp->front=temp;
      }
    else
      {
          trv=qp->front;
        if(trv->cost > temp->cost){
          temp->next=trv;
         qp->front=trv=temp;
        }
        else
        {
            while(trv!=NULL && temp->cost > trv->cost)
            {
                sp=trv;
                trv=trv->next;
            }
            sp->next=temp;
            temp->next=trv;
            if(trv == NULL)
                qp->rear = temp;
        }
      }
}

void queue_display_cost(struct queue*qp,char s[],char d[]){
   struct qelement *q;
    q=qp->front;
    while(q!=NULL)
     {
         if(strcmp(q->s,s)==0 && strcmp(q->d,d)==0)
         printf("%s\t\t%s\t\t%s\t\t%.3lf\n",q->dt,q->at,q->air,q->cost);
        q=q->next;
     }
}

void queue_display_route(struct queue*qp,char s[],char d[]){
   struct qelement *q;
    q=qp->front;
    while(q!=NULL)
     {
         if(strcmp(q->s,s)==0 && strcmp(q->d,d)==0)
           printf("%s\t\t%s\t\t%s\t\t%s\t%s\n",q->dt,q->at,q->air,q->route,q->dur);
         q=q->next;
     }
}

main()
{   
    int date,month,year;
    char Source[20],Destination[20];
    FILE *fp;
    struct queue q;
    qinit(&q);
    struct qelement Q;
  back :
     printf("Enter the date\n");
    scanf("%d",&date);
    printf("Enter the month\n");
    printf("1.Jan\n2.Feb\n3.Mar\n4.Apr\n5.May\n6.Jun\n7.Jul\n8.Aug\n9.Sep\n10.Oct\n11.Nov\n12.Dec\n");
    scanf("%d",&month);
    printf("Enter the year\n");
    scanf("%d",&year);
    char database[7][40]={"Mon.text","Tue.text","Wed.text","Thu.text","Fri.text","Sat.text","Sun.text"};
    printf("Enter the Source\t: ");
    scanf("%s",Source);
    printf("Enter the Destination\t: ");
    scanf("%s",Destination);
    int class;
    printf("Enter class\n1.Economic class\n2.Business class\n");
    scanf("%d",&class);
    int cnt1=0,cnt2=0,cnt3=0,cnt4=0,dummy_cnt=0,i;
    int y;
    int nres=calday(date,month,year);
    int day=tellday(nres);
    changecase(Source);
    changecase(Destination);
    switch(day)
    {
       case 1:
       fp = fopen(database[0],"r");
       break;

       case 2:
       fp = fopen(database[1],"r");
       break;

       case 4:
       fp = fopen(database[3],"r");
       break;

       case 5:
       fp = fopen(database[4],"r");
       break;

       case 6:
       fp = fopen(database[5],"r");

       break;

       case 7:
       fp = fopen(database[6],"r");
       break;

       default :
       printf("Invalid choice\n");
       break;
    }
    while(!feof(fp))
             display_details(fp,&cnt1,&dummy_cnt,Source,Destination);
       if(dummy_cnt==0)
         {
             printf("No Flights Available!!\n");
             printf("Would you like to see flights on any other date?\n1.YES\n2.NO\n");
             scanf("%d",&y);
             if(y==1)
                goto back;
             else
                exit(0);
         }
       rewind(fp);
       i=askchoice();
       if(i==1)
           {
             while(!feof(fp))
                display_cost(fp,class,&cnt2,Source,Destination,&q);
           }
        else if(i==2)
           {
              while(!feof(fp))
                 display_route(fp,&cnt3,Source,Destination,&q);
           }
        else
           {
              while(!feof(fp))
                 display_no_filter(fp,&cnt4,class,Source,Destination);
           }
         fclose(fp);
}
void display_details(FILE *fp,int *c,int *dummy,char s[],char d[])
{
       if(*c==0)
           {
                printf("Airline Agency\t\tRoute\n");
                *c=*c+1;
                printf("______________________________________________________________________________\n\m");
           }
       
        char src[20],dest[20],air[20],dtime[20],atime[20],route[100],dur[15];
        double ecost,bcost;

       fscanf(fp, " %s %s %s %s %s %s %s %lf %lf ", src,dest,air,dtime,atime,route,dur,&ecost,&bcost);
       changecase(src);
       changecase(dest);
       if((strcmp(s,src))==0 && (strcmp(d,dest)==0))
           { *dummy=*dummy+1;
              printf("%s\t\t",air);
              printf("\t%s\n",route);
           }
       else
           return;

}
void display_cost(FILE *fp,int class,int *c,char* s,char* d,struct queue *qp)
 {      if(*c==0)
         {
           if(class==1)
                printf("Boarding Time\tLanding time\tAirline Agency\tEconomic class cost\n");
           else
                 printf("Boarding Time\tLanding time\tAirline Agency\tBusiness class cost\n");
           *c=*c+1;
           printf("______________________________________________________________________________\n\n");
         }
         
          char src[20],dest[20],air[20],dtime[20],atime[20],route[100],dur[15];
            double ecost,bcost; 
        if(class==1){
            while(!feof(fp))
            {
                fscanf(fp, " %s %s %s %s %s %s %s %lf %lf ", src,dest,air,dtime,atime,route,dur,&ecost,&bcost);
                changecase(src);
                changecase(dest);
                qinsert(qp,ecost,src,dest,air,dtime,atime,route,dur);
            } 
            queue_display_cost(qp,s,d);
        }
        else{
            while(!feof(fp))
            {
                fscanf(fp, " %s %s %s %s %s %s %s %lf %lf ", src,dest,air,dtime,atime,route,dur,&ecost,&bcost);
                changecase(src);
                changecase(dest);
                qinsert(qp,bcost,src,dest,air,dtime,atime,route,dur);
            } 
            queue_display_cost(qp,s,d);
        }
}
        


void display_route(FILE *fp,int *c,char *s,char *d,struct queue *qp)
 {      if(*c==0)
           {
                printf("Boarding Time\tLanding time\tAirline Agency\tRoute\t\t\t\t\t\t\tDuration\n");
                *c=*c+1;
                 printf("_______________________________________________________________________________________________________________________________\n\n");
           }
           
        char src[20],dest[20],air[20],dtime[20],atime[20],route[100],dur[15];
        double ecost,bcost;
        while(!feof(fp))
         {
           fscanf(fp, " %s %s %s %s %s %s %s %lf %lf ", src,dest,air,dtime,atime,route,dur,&ecost,&bcost);
           changecase(src);
           changecase(dest);
           double time_in_min=return_time(dur);
           qinsert(qp,time_in_min,src,dest,air,dtime,atime,route,dur);
          } 
        queue_display_route(qp,s,d);
  }

void display_no_filter(FILE *fp,int *c,int class,char s[],char d[])
 {      if(*c==0)
         {
           if(class==1)
                printf("Boarding Time\tLanding time\tAirline Agency\tRoute\t\t\t\t\t\tDuration\tEconomic class cost\n");
           else
                 printf("Boarding Time\tLanding time\tAirline Agency\tRoute\t\t\t\t\t\tDuration\tBusiness class cost\n");
           *c=*c+1;
           printf("_____________________________________________________________________________________________________________________________________\n\n");
         }
         
          char src[20],dest[20],air[20],dtime[20],atime[20],route[100],dur[15];
          double ecost,bcost;
          while(!feof(fp))
       {  fscanf(fp, " %s %s %s %s %s %s %s %lf %.2lf ", src,dest,air,dtime,atime,route,dur,&ecost,&bcost);
         changecase(src);
         changecase(dest);
         if((strcmp(s,src))==0 && (strcmp(d,dest)==0))
           {
               if(class==1)
                    printf("%s\t\t%s\t\t%s\t\t%s\t%s\t\t%.0lf\n",dtime,atime,air,route,dur,ecost);
               else
                    printf("%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%.0lf\n",dtime,atime,air,route,dur,bcost);
           }
         else
              return;
       }
  }

void changecase(char str[])
{     int i;


 for (i = 0; str[i]!='\0'; i++)
        {
            if(str[i] >= 'a' && str[i] <= 'z')
                str[i] = str[i] -32;
        }
}

int askchoice()
{  int c;
   printf("On what basis whould you like to sort  the flights\n1.Cost\n2.Duration\n3.No Filters\n");
   scanf("%d",&c);
   return(c);
}



int res; //global declaration of variable
int fcorrect(int res,int yr)
{
int ycode;
//calculated in the range of 1600-2200
if(yr>=1600)
ycode=5;
if(yr>=1700)
ycode=3;
if(yr>=1800)
ycode=1;
if(yr>=1900)
ycode=0;
if(yr>=2000)
ycode=-1;
if(yr>=2100)
ycode=-3;
if(yr>=2200 && yr<=2300)
ycode=-5;
return (res+ycode);
}
int calday(int day, int mon,int yr)
{ int quot,mcode,sum,res,nres;
//code to find the last two digits of yr
int year,r1,r2,newyr;
newyr=yr;
r1=yr%10;
yr=yr/10;
r2=yr%10;
year=10*r2+r1;
quot=year/4; //to find the quotient of yr divides 4
//Code to assign month codes
switch(mon)
{
case 1: mcode=1;break;
case 2: mcode=4;break;
case 3: mcode=4;break;
case 4: mcode=0;break;

case 5: mcode=2;break;
case 6: mcode=5;break;
case 7: mcode=0;break;
case 8: mcode=3;break;
case 9: mcode=6;break;
case 10: mcode=1;break;
case 11: mcode=4;break;
case 12: mcode=6;break;
}
//main calculation
sum=day+quot+mcode+year;
res=sum%7;
nres=fcorrect(res,newyr);
// to rectify the ans. in accordance with century code
return nres;
}
int tellday(int nres)
{
if(nres==0 || nres==-7)
return(6);
if(nres==1 || nres==-6)
return(7);
if(nres==2 || nres==-5)
return(1);
if(nres==3 || nres==-4)
return(2);
if(nres==4 || nres==-3)
return(3);
if(nres==5 || nres==-2)
return(4);
if(nres==6 || nres==-1)
return(5);
}
int return_time(char dur[])
{
     int hr_d1=dur[0]-'0';
     int hr_d2=dur[1]-'0';
     int hr=hr_d1*10+hr_d2;
     int min_d1=dur[3]-'0';
     int min_d2=dur[4]-'0';
     int min=min_d1*10+min_d2;
     int time_in_min=hr*60+min;
     return(time_in_min);
}