#ifndef LANG_H
#define LANG_H

typedef struct {
    const char *system_name;
    const char *choice;
    const char *choice1;
    const char *choice2;
    const char *choice3;
    const char *choice4;
    const char *choice5;
    const char *choice6;
    const char *choice7;
    const char *invalid_option;
    const char *norecords;
    const char *yesrecord;
    const char *in_serial;
    const char *in_person;
    const char *in_identi;
    const char *noserial;
    const char *input_required;
    const char *confirm_overwrite;
    const char *data_file;
    const char *export_done;
    const char *summary;
    const char *export_prompt;
    const char *another_checkin;
    const char *another_checkout;
    const char *another_search;
    const char *another_export;
    char yes_key;
    char no_key;
    const char *columns;
    const char *listing;
    const char *checkin_saved;
    const char *checkout_saved;
    const char *already_checkin;
    const char *already_checkout;
    const char *error_rename;
} LangTexts;

extern const LangTexts LANG_ES;
extern const LangTexts LANG_EN;

#endif
