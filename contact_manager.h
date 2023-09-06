#ifndef CONTACT_MANAGER_H
#define CONTACT_MANAGER_H

#include <gtk/gtk.h>

typedef struct ContactInfo
{
    gchar *Name;
    gchar *Sur_Name;
    gchar *Address;
    gchar *Contact_Type;
    gchar *email;
    gchar *Number;
}contact;



typedef struct gtk_win_data
{
    GtkTreeIter iter;
    GtkTreeIter comboIter;
    GtkWidget *combo;
    GtkWidget *liststore;
    GtkWidget *window;
    GtkBuilder *build;
    gint contact_num;
} win_data;

void initialize(win_data *userdata, int argc, char *argv[]);
gboolean on_close(GtkWidget *gwt, gpointer data);
void Delete_Contact(GtkWidget *gwt, gpointer data);
void Edit_Contact(GtkWidget *btn, gpointer data);
void Show_Contact(GtkButton *btn, gpointer data);
void Add_Contact(GtkButton *btn, gpointer data);
void Print_Contact(GtkButton *btn, gpointer data);

#endif