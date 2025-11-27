/*
Author: Wilfredo R. Rodriguez H.
e-mail: wilfredor@gmail.com
Lic: GPL3
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "lang.h"
#define TAM 20

typedef struct Checkin {
    char name[TAM];
    char id_number[TAM];
    char serial[TAM];
    int outed;
} nCheckin;

static nCheckin ncheckin;
static const LangTexts *lang = &LANG_ES;
static char data_file[256] = "pcs.dat";

static void clear_line(void);
static void read_line(char *, size_t);
static int option(char);
static void list_records(void);
static void print_record_line(const nCheckin *);
static void select_language(int, char **);
static void summarize(void);
static void export_csv(void);
static void normalize_serial(char *);
static void mainmenu(void);
static void checkouter(char[TAM]);
static void checkout(void);
static void checkinter(nCheckin);
static void checkin(void);
static void search(void);
static void delfile(void);
static int find (char[TAM]);

static int msg_confirmation(const char*);
static void show_record(nCheckin);

static void clear_line(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

static void read_line(char *buffer, size_t length)
{
    if (fgets(buffer, (int)length, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

static int is_empty(const char *text)
{
    return text[0] == '\0';
}

static void normalize_serial(char *serial)
{
    size_t len = strlen(serial);
    while (len > 0 && isspace((unsigned char)serial[len-1])) {
        serial[len-1] = '\0';
        len--;
    }
    size_t start = 0;
    while (serial[start] && isspace((unsigned char)serial[start])) {
        start++;
    }
    if (start > 0)
        memmove(serial, serial + start, strlen(serial + start) + 1);
    for (size_t i = 0; serial[i]; ++i)
        serial[i] = (char)toupper((unsigned char)serial[i]);
}

static void select_language(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        const char *arg = argv[i];
        if (strncmp(arg, "--lang=", 7) == 0)
        {
            const char *code = arg + 7;
            if (strcmp(code, "en") == 0)
                lang = &LANG_EN;
            else if (strcmp(code, "es") == 0)
                lang = &LANG_ES;
            else
                fprintf(stderr, "Idioma '%s' no soportado, usando español.\n", code);
        }
        else if (strncmp(arg, "--file=", 7) == 0)
        {
            snprintf(data_file, sizeof data_file, "%s", arg + 7);
        }
        else if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0)
        {
            printf("Uso: %s [--lang=en|es] [--file=ruta]\n", argv[0]);
            exit(0);
        }
    }
}

static void mainmenu(void)
{
	int running = 1;
    while (running)
    {
        printf("%s", lang->system_name);
        printf("%s", lang->choice);
        printf("%s", lang->choice1);
        printf("%s", lang->choice2);
        printf("%s", lang->choice3);
        printf("%s", lang->choice4);
        printf("%s", lang->choice5);
        printf("%s", lang->choice6);
        printf("%s", lang->choice7);
        running = option((char)getchar());
        clear_line();
    }
}

static int option(char opt)
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
                return 0;
            case '5':
                list_records();
                break;
            case '6':
                export_csv();
                break;
            case '7':
                return 0;
            default :
                printf("%s", lang->invalid_option);
                break;
        }
        return 1;
}

static int find (char serial[TAM]) {

    FILE *fp;
	nCheckin record;
	int entro = 0;
    fp = fopen(data_file,"rb");
    if (fp==NULL)
    {
        printf("%s", lang->norecords);
        return 0;
    }
    else
    {
        while (fread(&record,sizeof record,1,fp) == 1 && (!entro))
        {
			if (record.serial[0]!='\0')
            {
                if ((strcmp(serial,record.serial)==0))
                {
                    show_record(record);
                    entro = 1;
                }
            }
        }
    }
	fclose(fp);
    return entro;
}

static int msg_confirmation(const char * mensaje)
{
        for (;;)
        {
            printf("%s", mensaje);
            int ch = getchar();
            if (ch == EOF)
                return 0;
            ch = toupper((unsigned char)ch);
            clear_line();
            if (ch == toupper((unsigned char)lang->yes_key)) return 1;
            if (ch == toupper((unsigned char)lang->no_key)) return 0;
        }
}

static void search(void)
{
    int again = 0;
    do {
        char serial[TAM];
        printf("%s", lang->in_serial);
        read_line(serial, sizeof serial);
        if (is_empty(serial)) {
            printf("%s", lang->input_required);
            again = msg_confirmation(lang->another_search);
            continue;
        }
        normalize_serial(serial);
        if (!find(serial))
            printf(lang->noserial ,serial);
        again = msg_confirmation(lang->another_search);
    } while (again);
}

static void checkout(void)
{
    int again = 0;
    do {
        char serial[TAM];
        printf("%s", lang->in_serial);
        read_line(serial, sizeof serial);
        if (is_empty(serial)) {
            printf("%s", lang->input_required);
            again = msg_confirmation(lang->another_checkin);
            continue;
        }
        normalize_serial(serial);
        checkouter(serial);
        again = msg_confirmation(lang->another_checkin);
    } while (again);
}

static void list_records(void)
{
    FILE *fp = fopen(data_file, "rb");
    nCheckin record;
    int count = 0;
    if (fp == NULL)
    {
        printf("%s", lang->norecords);
        return;
    }

    printf("%s", lang->listing);
    printf("%s", lang->columns);
    while (fread(&record, sizeof record, 1, fp) == 1)
    {
        if (record.serial[0]=='\0')
            continue;
        print_record_line(&record);
        count++;
    }
    if (count == 0)
        printf("%s", lang->norecords);
    fclose(fp);
}

static void summarize(void)
{
    FILE *fp = fopen(data_file, "rb");
    nCheckin record;
    int total = 0, inside = 0, outside = 0;
    if (fp != NULL)
    {
        while (fread(&record, sizeof record, 1, fp) == 1)
        {
            if (record.serial[0]=='\0')
                continue;
            total++;
            if (record.outed)
                outside++;
            else
                inside++;
        }
        fclose(fp);
    }
    printf(lang->summary, total, inside, outside);
}

static void export_csv(void)
{
    char path[256];
    printf("%s", lang->export_prompt);
    read_line(path, sizeof path);
    if (is_empty(path))
        snprintf(path, sizeof path, "pcs.csv");

    FILE *fp_in = fopen(data_file, "rb");
    if (fp_in == NULL)
    {
        printf("%s", lang->norecords);
        return;
    }
    FILE *fp_out = fopen(path, "w");
    if (fp_out == NULL)
    {
        perror(path);
        fclose(fp_in);
        return;
    }
    fprintf(fp_out, "id,name,serial,out\n");
    nCheckin record;
    while (fread(&record, sizeof record, 1, fp_in) == 1)
    {
        if (record.serial[0]=='\0')
            continue;
        fprintf(fp_out, "%s,%s,%s,%d\n",
                record.id_number,
                record.name,
                record.serial,
                record.outed);
    }
    fclose(fp_in);
    fclose(fp_out);
    printf(lang->export_done, path);
    if (msg_confirmation(lang->another_export))
        export_csv();
}

static void checkin(void)
{
    int again = 0;
    do {
        printf("%s", lang->in_person);
        read_line(ncheckin.name, sizeof ncheckin.name);

        printf("%s", lang->in_identi);
        read_line(ncheckin.id_number, sizeof ncheckin.id_number);

        printf("%s", lang->in_serial);
        read_line(ncheckin.serial, sizeof ncheckin.serial);
        if (is_empty(ncheckin.name) || is_empty(ncheckin.id_number) || is_empty(ncheckin.serial)) {
            printf("%s", lang->input_required);
            again = msg_confirmation(lang->another_checkout);
            continue;
        }
        normalize_serial(ncheckin.serial);

        ncheckin.outed = 0;

        checkinter(ncheckin);
        again = msg_confirmation(lang->another_checkout);
    } while (again);
}

static void checkinter(nCheckin ncheckin_local)
{
    FILE *fp,*fptemp;
    int entro = 0;
    nCheckin record;
    fp = fopen(data_file, "rb");
    fptemp = fopen("temp~~","wb");
    if (fptemp == NULL)
    {
        perror("temp~~");
        if (fp) fclose(fp);
        return;
    }
    if (fp!=NULL)
    {
        while (fread(&record,sizeof record,1,fp) == 1)
        {
            if ((record.serial[0]!='\0') && (!strcmp(ncheckin_local.serial,record.serial))&&(!entro))
            {
            	entro = 1;
            	if (record.outed)
                {
                    printf(lang->checkin_saved, record.serial);
                    record.outed = 0;
                } else
                {
                    printf(lang->already_checkin,record.serial);
                    if (msg_confirmation(lang->confirm_overwrite))
                    {
                        strncpy(record.name, ncheckin_local.name, sizeof record.name);
                        record.name[sizeof record.name - 1] = '\0';
                        strncpy(record.id_number, ncheckin_local.id_number, sizeof record.id_number);
                        record.id_number[sizeof record.id_number - 1] = '\0';
                        record.outed = 0;
                        printf(lang->checkin_saved, record.serial);
                    }
					show_record(record);
                }
            }
            fwrite(&record, sizeof record,1,fptemp);
        }
        fclose(fp);
    }
	if (!(entro))
    {
		fwrite(&ncheckin_local, sizeof (ncheckin_local),1,fptemp);
        printf(lang->checkin_saved, ncheckin_local.serial);
    }


    fclose(fptemp);
    delfile();

}

static void checkouter(char serial[TAM])
{

    FILE *fp,*fptemp;
    nCheckin record;
    fp = fopen(data_file, "rb");
    fptemp = fopen("temp~~","wb");
    int entro = 0;

    if (fptemp == NULL)
    {
        perror("temp~~");
        if (fp) fclose(fp);
        return;
    }

    if (fp==NULL)
    {
        printf("%s", lang->norecords);
        fclose(fptemp);
        return;
    }
    else
    {
        while (fread(&record,sizeof record,1,fp) == 1)
        {
			if ((record.serial[0]!='\0')&&(!entro))
				if (strcmp(serial,record.serial)==0)
				{
					entro = 1;
					if (!record.outed)
					{
						printf(lang->checkout_saved,record.serial);
						record.outed = 1;
					} else
						printf(lang->already_checkout,record.serial);

					show_record(record);
				}
            fwrite(&record, sizeof record,1,fptemp);

        }
        fclose(fp);
    }
    if (!entro)
        printf(lang->noserial, serial);
    fclose(fptemp);
	delfile();
}
static void delfile(void)
{
    char backup[512];
    snprintf(backup, sizeof backup, "%s.bak", data_file);
    /* Keep a backup; ignore if source doesn't exist. */
    if ((rename(data_file, backup)!=0) && (errno != ENOENT))
    {
        perror(data_file);
        return;
    }
    if ((rename("temp~~", data_file))!=0)
    {
    	perror(lang->error_rename);
        /* Try to restore original */
        rename(backup, data_file);
    }
}

static void print_record_line(const nCheckin *record)
{
    printf("%s\t%s\t%s\t%c \n",
           record->id_number,
           record->name,
           record->serial,
           ((record->outed)?(lang->yes_key):(lang->no_key)));
}

static void show_record(nCheckin ncheckin_local)
{
	printf("%s", lang->yesrecord);
	printf("%s", lang->columns);
	print_record_line(&ncheckin_local);

	getchar();
}
int main(int argc, char **argv)
{
    select_language(argc, argv);
    printf(lang->data_file, data_file);
    mainmenu();
    summarize();
    return 0;
}
