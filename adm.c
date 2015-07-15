/*
Author: Wilfredo R. Rodriguez H.
e-mail: wilfredor@gmail.com
Lic: GPL3
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lang_es.h"
#define FILE_NAME "pcs.dat"
#define TAM 20

typedef struct Checkin {
    char name[TAM];
    char id_number[TAM];
    char serial[TAM];
    int outed;

} nCheckin;
nCheckin ncheckin;

void option(char);
void mainmenu();
void checkouter(char[TAM]);
void checkout();
void checkinter(nCheckin);
void checkin();
void search();
void delfile();
int find (char[TAM]);

int msg_confirmation(const char*, ...);
void show_record(nCheckin);

void mainmenu()
{
	printf(SYSTEM_NAME);
	printf(CHOICE);
	printf(CHOICE1);
	printf(CHOICE2);
	printf(CHOICE3);
	printf(CHOICE4);
	option(getchar());
}

void option(char opt)
{
        switch(opt)
        {
            case '1':
                checkin();
                break;
            case '2':
                checkout();
                break;
            case '3':
                search();
                break;
            case '4':
                return;
                break;
            default :
                mainmenu();
                break;
        }
}

int find (char serial[TAM]) {

    FILE *fp;
	int entro = 0;
    fp = fopen(FILE_NAME,"r");
    if (fp==NULL)
    {
        printf(NORECORDS);
        getchar();
        fclose(fp);
        return 0;
    }
    else
    {
        while (!feof(fp)&&(!entro))
        {
            fread(&ncheckin,sizeof ncheckin,1,fp);
			if (ncheckin.serial!=NULL)
            if ((strcmp(serial,ncheckin.serial)==0))
            {
                show_record(ncheckin);
                entro = 1;
            }
        }
    }
	fclose(fp);
    return entro;
}

int msg_confirmation(const char * mensaje, ...)
{
        printf(mensaje); char resp = toupper(getchar());
        return (resp==YES_KEY)?1:((resp==NO_KEY)?0:(msg_confirmation(mensaje)));
}

void search()
{
    char serial[TAM];
	fflush(stdin);
    printf(IN_SERIAL);
    scanf("%*c%[^\n]",serial);
    if (!find(serial))
        printf(NOSERIAL ,serial);
    if (msg_confirmation(ANOTHER_SEARCH))
		search();
    mainmenu();
}

void checkout()
{
    char serial[TAM];
    printf(IN_SERIAL);
    scanf("%*c%[^\n]", serial);
    checkouter(serial);

    if (msg_confirmation(ANOTHER_CHECKIN))
       checkout();
    mainmenu();
}

void checkin()
{


	printf(IN_PERSON);
	scanf("%*c%[^\n]",ncheckin.name);

    	printf(IN_IDENTI);
	scanf("%*c%[^\n]",ncheckin.id_number);

   	printf(IN_SERIAL);
	scanf("%*c%[^\n]",ncheckin.serial);

	ncheckin.outed = 0;

    checkinter(ncheckin);

    if (msg_confirmation(ANOTHER_CHECKOUT))
    	checkin();

    mainmenu();
}

void checkinter(nCheckin ncheckin_local)
{
    FILE *fp,*fptemp;
    int entro = 0;
    fp = fopen(FILE_NAME, "r");
    fptemp = fopen("temp~~","a");
    if (fp!=NULL)
    {
        while (!feof(fp))
        {
            fread(&ncheckin,sizeof ncheckin,1,fp);

            if ((!strcmp(ncheckin_local.serial,ncheckin.serial))&&(ncheckin.serial!=NULL)&&(!entro))
            {
            	entro = 1;
            	if (ncheckin.outed)
                {
                    printf(CHECKIN_SAVED,ncheckin.serial);
                    ncheckin.outed = 0;
                } else
                {
                    printf(ALREADY_CHECKIN,ncheckin.serial);
					show_record(ncheckin);
                }
            }
            fwrite(&ncheckin, sizeof ncheckin,1,fptemp);
        }
        fclose(fp);
    }
	if (!(entro))
		fwrite(&ncheckin_local, sizeof (ncheckin_local),1,fptemp);


    fclose(fptemp);
    delfile();

}

void checkouter(char serial[TAM])
{

    FILE *fp,*fptemp;
    fp = fopen(FILE_NAME, "r");
	fptemp = fopen("temp~~","a");
    int entro = 0;

    if (fp==NULL)
    {
        printf(NORECORDS);
        getchar();
        return;
    }
    else
    {
        while (!feof(fp))
        {
            fread(&ncheckin,sizeof ncheckin,1,fp);

			if ((ncheckin.serial!=NULL)&&(!entro))
				if (strcmp(serial,ncheckin.serial)==0)
				{
					entro = 1;
					if (!ncheckin.outed)
					{
						printf(CHECKOUT_SAVED,ncheckin.serial);
						ncheckin.outed = 1;
					} else
						printf(CHECKIN_SAVED,ncheckin.serial);

					show_record(ncheckin);
				}
            fwrite(&ncheckin, sizeof ncheckin,1,fptemp);

        }
        fclose(fp);
    }
    fclose(fptemp);
	delfile();
}
void delfile()
{
    remove(FILE_NAME);
    if ((rename("temp~~", FILE_NAME))!=0)
    	perror(ERROR_RENAME);
}

void show_record(nCheckin ncheckin_local)
{
	printf(YESRECORD);
	printf(COLUMNS);
	printf("%s\t%s\t%s\t%c \n",ncheckin_local.id_number,
								  ncheckin_local.name,
								  ncheckin_local.serial,
								  ((ncheckin_local.outed)?(YES_KEY):(NO_KEY)));

	getchar();
}
int main(){mainmenu(); return 0;}
