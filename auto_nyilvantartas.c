#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DB 200
#define MAX 1024

#define RSZTAG 3
#define RSZKOZEP 3

//*****************************RENDSZ ELLENORZES*****************************

int rendszame(char s[]){
int i;
const char *ptr = &s[strlen(s)-RSZTAG];
char string[MAX];

if(strlen(s)!=7)
    return 1;

for(i=0;i<RSZTAG;i++)
    if(!isupper(s[i]))
        return 1;

if(s[RSZKOZEP]!='-')
    return 1;

for(i=0;i<RSZTAG;i++)
    if(!isdigit(ptr[i]))
        return 1;

return 0;
}

//*****************************DATUM ELLENORZES*****************************

int datume(char s[]){
int i, szokoev = 0, n = 0;
const int datumsz = 8, pont1 = 4, pont2 = 7, dhossz = 10, honapok = 12, febr = 2;
int napoksz [2][13] = {
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
char string[MAX];
char ev[MAX];
char honap[MAX];
char nap[MAX];

//FORMAI ELLENORZES:

if(strlen(s)!=dhossz)
    return 1;

if(s[pont1]!='.' || s[pont2]!='.')
    return 1;

for(i=0;s[i]!='\0';i++)
    if(isdigit(s[i]))
        n++;

if(n!=datumsz)
    return 1;

//TARTALMI ELLENORZES

strcpy(string,s);
strcpy(ev, strtok(string , "."));
strcpy(honap, strtok(NULL, "."));
strcpy(nap , strtok(NULL, "."));

//szokoev-e?
if(atoi(ev) % 4 == 0 && atoi(ev) % 100 != 0 || atoi(ev) % 400 == 0){
    if(atoi(honap)==febr)
        szokoev = 1;
}
else {
    if(atoi(honap)==febr)
        szokoev = 0;
}

//ha szokoev ha nem...
for(i=0;i<honapok;i++){
if(atoi(honap) == i)
    if(atoi(nap) == 0 || atoi(nap) > napoksz[szokoev][i] )
        return 1;
}

//honapok szama
if(atoi(honap) == 0 || atoi(honap) > honapok)
    return 1;


return 0;
}

//*****************************KMSZA ELLENORZES*****************************

int poz_szame(char s[]){
int i;
if(s[0]=='-' || isdigit(s[0])){
for(i=1;s[i]!='\0';i++)
    if(!isdigit(s[i]))
        return 1;
}
else return 1;
return 0;
}

//STRUKTURABA FOGLALAS

struct adatok {

char rendsz[MAX];
char datum[MAX];
char szamlalo[MAX];

}kocsik[DB];

//*****************************PROGRAM INDUL********************************

int main(void)
{
    int i = 0, j, a;
    FILE *fp;
    char *buffer;

fp = fopen("km.dat","ab+");
if(!fp){
    perror("Hiba: ");
    exit(1);
}
printf("A km.dat fajl letezik!\nA fajl a kovetkezo adatokat tartalmazza:\n\n");

buffer=malloc(sizeof(char)*MAX);
while(fgets(buffer,MAX,fp)!=NULL)
    printf("%s",buffer);

fseek(fp,0,SEEK_SET);
free(buffer);
printf("\n");

while(1){

    while(1){

    printf("Adja meg a gepjarmu rendszamat: ");

    fgets(kocsik[i].rendsz,MAX,stdin);
    if(kocsik[i].rendsz[0]=='\n')
        goto vegrehajt;

    kocsik[i].rendsz[strlen(kocsik[i].rendsz)-1]='\0';

    if(rendszame(kocsik[i].rendsz)!=0){
        printf("Nem ervenyes rendszam formatum!\n");
    continue;
    }

    break;
    }

    while(1){

    printf("Adja meg a datumot: ");

    fgets(kocsik[i].datum,MAX,stdin);

    kocsik[i].datum[strlen(kocsik[i].datum)-1]='\0';

    if(datume(kocsik[i].datum)!=0){
        printf("Hiba! Nem helyes datum!\n");
    continue;
    }

    break;
    }

    while(1){

    printf("Adja meg a km szamlalo allasat: ");

    fgets(kocsik[i].szamlalo,MAX,stdin);

    kocsik[i].szamlalo[strlen(kocsik[i].szamlalo)-1]='\0';

    if(poz_szame(kocsik[i].szamlalo)!=0){
        printf("Hiba! Nem szam!\n");
    continue;
    }
    else if(atoi(kocsik[i].szamlalo)<=0){
        printf("Hiba! A km ora allasnak pozitiv egeszszamnak kell lennie!\n");
    continue;
    }

    break;
    }

    i++;
}

//*****************************PROGRAM VEGREHAJTASA*****************************

vegrehajt:
/*
fseek(fp,0,SEEK_END);
int filesize = ftell(fp);
fseek(fp,0,SEEK_SET);
*/
buffer=malloc(sizeof(char)*MAX);
printf("\nAz uj adatokkal frissitett fajl tartalma:\n\n");
while(fgets(buffer,MAX,fp)!=NULL)
    printf("%s",buffer);
for(j=0;j<i;j++)
    printf("%s\t%s\t%s\n",kocsik[j].rendsz, kocsik[j].datum,
        kocsik[j].szamlalo);

//fajl frissitese az uj adatokkal
fseek(fp,0,SEEK_SET);
//kkirat stdin-rol
for(j=0;j<i;j++)
    fprintf(fp,"%s\t%s\t%s\r\n",kocsik[j].rendsz, kocsik[j].datum,
        kocsik[j].szamlalo);

    fclose(fp);
    free(buffer);

    return 0;
}
