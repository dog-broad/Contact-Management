#include <stdio.h>
#include <gtk/gtk.h>
#include "contact_manager.h"

void initialize(win_data *userdata, int argc, char *argv[]) {
    userdata->contact_num = 0;
    gtk_init(&argc, &argv);

    userdata->build = gtk_builder_new_from_file("UI.glade");
    userdata->window = (GtkWidget *)gtk_builder_get_object(userdata->build, "window");
    g_signal_connect(G_OBJECT(userdata->window), "delete_event", G_CALLBACK(on_close), userdata);

    userdata->liststore = (GtkWidget *)gtk_builder_get_object(userdata->build, "liststore1");
    userdata->combo = (GtkWidget *)gtk_builder_get_object(userdata->build, "combo");

    GtkCellRenderer *rcell1 = gtk_cell_renderer_text_new();
    GtkCellRenderer *rcell2 = gtk_cell_renderer_text_new();
    GtkCellRenderer *rcell5 = gtk_cell_renderer_text_new();

    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(userdata->combo), rcell1, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(userdata->combo), rcell1, "text", 0, NULL);

    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(userdata->combo), rcell2, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(userdata->combo), rcell2, "text", 1, NULL);

    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(userdata->combo), rcell5, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(userdata->combo), rcell5, "text", 5, NULL);

    g_signal_connect(gtk_builder_get_object(userdata->build, "Display"), "clicked", G_CALLBACK(Show_Contact), userdata);
    g_signal_connect(gtk_builder_get_object(userdata->build, "AddContact"), "clicked", G_CALLBACK(Add_Contact), userdata);
    g_signal_connect(gtk_builder_get_object(userdata->build, "EditBtn"), "clicked", G_CALLBACK(Edit_Contact), userdata);
    g_signal_connect(gtk_builder_get_object(userdata->build, "Delete"), "clicked", G_CALLBACK(Delete_Contact), userdata);
    g_signal_connect(gtk_builder_get_object(userdata->build, "Print"), "clicked", G_CALLBACK(Print_Contact), userdata);

    gtk_widget_show_all(userdata->window);
    gtk_main();
}

gboolean on_close(GtkWidget *gwt, gpointer data) {
    gtk_main_quit();
    return TRUE;
}

void Delete_Contact(GtkWidget *gwt, gpointer data) {
    win_data *UIdata = data;

    GtkWidget *ErrorMsg = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Log");
    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(UIdata->combo), &UIdata->comboIter) == 0) {
        gtk_label_set_text(GTK_LABEL(ErrorMsg), "You didn't choose a contact to delete in the ComboBox.");
        return;
    }

    gtk_list_store_remove(GTK_LIST_STORE(UIdata->liststore), &UIdata->comboIter);

    UIdata->contact_num -= 1;

    GtkWidget *Contact_number = (GtkWidget *)gtk_builder_get_object(UIdata->build, "contact_num");
    gchar nbr_str[10];
    g_snprintf(nbr_str, 10, "%d", UIdata->contact_num);
    gtk_label_set_text(GTK_LABEL(Contact_number), nbr_str);
}

void Add_Contact(GtkButton *btn, gpointer data) {
    win_data *UIdata = data;
    GtkWidget *diag_Add_Contact = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Add_Edit_Contact");

    GtkWidget *FirstName = (GtkWidget *)gtk_builder_get_object(UIdata->build, "FirstName");
    GtkWidget *SurName = (GtkWidget *)gtk_builder_get_object(UIdata->build, "SurName");
    GtkWidget *Address = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Address");
    GtkWidget *T_Contact = (GtkWidget *)gtk_builder_get_object(UIdata->build, "T_Contact");
    GtkWidget *Mail = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Mail");
    GtkWidget *Num = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Num");

    GtkWidget *EditError = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Log");
    gtk_label_set_text(GTK_LABEL(EditError), "");

    contact new_contact;

    gtk_widget_show_all(diag_Add_Contact);

    gint res = gtk_dialog_run(GTK_DIALOG(diag_Add_Contact));

    if (gtk_entry_get_text_length(GTK_ENTRY(FirstName)) != 0 && gtk_entry_get_text_length(GTK_ENTRY(SurName)) != 0 &&
        gtk_entry_get_text_length(GTK_ENTRY(Num)) != 0) {
        switch (res) {
            case GTK_RESPONSE_ACCEPT:

                new_contact.Name = (gchar *)gtk_entry_get_text(GTK_ENTRY(FirstName));
                new_contact.Sur_Name = (gchar *)gtk_entry_get_text(GTK_ENTRY(SurName));
                new_contact.Address = (gchar *)gtk_entry_get_text(GTK_ENTRY(Address));
                new_contact.Contact_Type = (gchar *)gtk_entry_get_text(GTK_ENTRY(T_Contact));
                new_contact.email = (gchar *)gtk_entry_get_text(GTK_ENTRY(Mail));
                new_contact.Number = (gchar *)gtk_entry_get_text(GTK_ENTRY(Num));

                gtk_list_store_append(GTK_LIST_STORE(UIdata->liststore), &UIdata->iter);
                gtk_list_store_set(GTK_LIST_STORE(UIdata->liststore), &UIdata->iter,
                                    0, new_contact.Name,
                                    1, new_contact.Sur_Name,
                                    2, new_contact.Address,
                                    3, new_contact.Contact_Type,
                                    4, new_contact.email,
                                    5, new_contact.Number
                                    ,-1);

                UIdata->contact_num += 1;

                GtkWidget *Contact_number = (GtkWidget *)gtk_builder_get_object(UIdata->build, "contact_num");
                gchar nbr_str[10];
                g_snprintf(nbr_str, 10, "%d", UIdata->contact_num);
                gtk_label_set_text(GTK_LABEL(Contact_number), nbr_str);
                break;

            case GTK_RESPONSE_CANCEL:
            case GTK_RESPONSE_NONE:
                break;
        }
    } else {
        gtk_label_set_text(GTK_LABEL(EditError), "Mandatory fields have not been filled,\nAdding terminated");
    }

    gtk_entry_set_text(GTK_ENTRY(FirstName), "");
    gtk_entry_set_text(GTK_ENTRY(SurName), "");
    gtk_entry_set_text(GTK_ENTRY(Address), "");
    gtk_entry_set_text(GTK_ENTRY(T_Contact), "");
    gtk_entry_set_text(GTK_ENTRY(Mail), "");
    gtk_entry_set_text(GTK_ENTRY(Num), "");
    gtk_widget_hide(diag_Add_Contact);
}

void Edit_Contact(GtkWidget *btn, gpointer data) {
    win_data *UIdata = data;
    contact edit_contact;
    GtkWidget *diag_Edit_Contact = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Add_Edit_Contact");

    GtkWidget *FirstName = (GtkWidget *)gtk_builder_get_object(UIdata->build, "FirstName");
    GtkWidget *SurName = (GtkWidget *)gtk_builder_get_object(UIdata->build, "SurName");
    GtkWidget *Address = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Address");
    GtkWidget *T_Contact = (GtkWidget *)gtk_builder_get_object(UIdata->build, "T_Contact");
    GtkWidget *Mail = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Mail");
    GtkWidget *Num = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Num");

    GtkWidget *EditError = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Log");

    gtk_label_set_text(GTK_LABEL(EditError), "");

    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(UIdata->combo), &UIdata->comboIter) == 0) {
        gtk_label_set_text(GTK_LABEL(EditError), "You didn't choose a contact to edit in the ComboBox.");
        return;
    }

    gtk_tree_model_get(GTK_TREE_MODEL(UIdata->liststore), &UIdata->comboIter,
                       0, &edit_contact.Name,
                       1, &edit_contact.Sur_Name,
                       2, &edit_contact.Address,
                       3, &edit_contact.Contact_Type,
                       4, &edit_contact.email,
                       5, &edit_contact.Number
                       ,-1);

    gtk_entry_set_text(GTK_ENTRY(FirstName), edit_contact.Name);
    gtk_entry_set_text(GTK_ENTRY(SurName), edit_contact.Sur_Name);
    gtk_entry_set_text(GTK_ENTRY(Address), edit_contact.Address);
    gtk_entry_set_text(GTK_ENTRY(T_Contact), edit_contact.Contact_Type);
    gtk_entry_set_text(GTK_ENTRY(Mail), edit_contact.email);
    gtk_entry_set_text(GTK_ENTRY(Num), edit_contact.Number);

    gtk_widget_show_all(diag_Edit_Contact);

    gint res = gtk_dialog_run(GTK_DIALOG(diag_Edit_Contact));

    if (gtk_entry_get_text_length(GTK_ENTRY(FirstName)) != 0 && gtk_entry_get_text_length(GTK_ENTRY(SurName)) != 0 &&
        gtk_entry_get_text_length(GTK_ENTRY(Num)) != 0) {
        switch (res) {
            case GTK_RESPONSE_ACCEPT:

                edit_contact.Name = (gchar *)gtk_entry_get_text(GTK_ENTRY(FirstName));
                edit_contact.Sur_Name = (gchar *)gtk_entry_get_text(GTK_ENTRY(SurName));
                edit_contact.Address = (gchar *)gtk_entry_get_text(GTK_ENTRY(Address));
                edit_contact.Contact_Type = (gchar *)gtk_entry_get_text(GTK_ENTRY(T_Contact));
                edit_contact.email = (gchar *)gtk_entry_get_text(GTK_ENTRY(Mail));
                edit_contact.Number = (gchar *)gtk_entry_get_text(GTK_ENTRY(Num));
                
                gtk_list_store_set(GTK_LIST_STORE(UIdata->liststore), &UIdata->comboIter,
                                    0, edit_contact.Name,
                                    1, edit_contact.Sur_Name,
                                    2, edit_contact.Address,
                                    3, edit_contact.Contact_Type,
                                    4, edit_contact.email,
                                    5, edit_contact.Number
                                    ,-1);
                break;

            case GTK_RESPONSE_CANCEL:
            case GTK_RESPONSE_NONE:
                break;
        }
    } else {
        gtk_label_set_text(GTK_LABEL(EditError), "Mandatory fields have not been filled,\nEditing terminated");
    }

    gtk_entry_set_text(GTK_ENTRY(FirstName), "");
    gtk_entry_set_text(GTK_ENTRY(SurName), "");
    gtk_entry_set_text(GTK_ENTRY(Address), "");
    gtk_entry_set_text(GTK_ENTRY(T_Contact), "");
    gtk_entry_set_text(GTK_ENTRY(Mail), "");
    gtk_entry_set_text(GTK_ENTRY(Num), "");
    gtk_widget_hide(diag_Edit_Contact);
}

void Show_Contact(GtkButton *btn, gpointer data) {
    win_data *UIdata = data;
    contact disp_contact;

    GtkWidget *diag_display_contact = (GtkWidget *)gtk_builder_get_object(UIdata->build, "display_contact");

    GtkWidget *FirstName = (GtkWidget *)gtk_builder_get_object(UIdata->build, "FirstName_disp");
    GtkWidget *SurName = (GtkWidget *)gtk_builder_get_object(UIdata->build, "SurName_disp");
    GtkWidget *Address = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Address_disp");
    GtkWidget *T_Contact = (GtkWidget *)gtk_builder_get_object(UIdata->build, "T_Contact_disp");
    GtkWidget *Mail = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Mail_disp");
    GtkWidget *Num = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Num_disp");

    GtkWidget *EditLog = (GtkWidget *)gtk_builder_get_object(UIdata->build, "Log");
    gtk_label_set_text(GTK_LABEL(EditLog), "");

    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(UIdata->combo), &UIdata->comboIter) == 0) {
        gtk_label_set_text(GTK_LABEL(EditLog), "No Contact Selected, Display Terminated");
        return;
    }

    gtk_tree_model_get(GTK_TREE_MODEL(UIdata->liststore), &UIdata->comboIter,
                       0, &disp_contact.Name,
                       1, &disp_contact.Sur_Name,
                       2, &disp_contact.Address,
                       3, &disp_contact.Contact_Type,
                       4, &disp_contact.email,
                       5, &disp_contact.Number
                       ,-1);

    gtk_label_set_text(GTK_LABEL(FirstName), disp_contact.Name);
    gtk_label_set_text(GTK_LABEL(SurName), disp_contact.Sur_Name);
    gtk_label_set_text(GTK_LABEL(Address), disp_contact.Address);
    gtk_label_set_text(GTK_LABEL(T_Contact), disp_contact.Contact_Type);
    gtk_label_set_text(GTK_LABEL(Mail), disp_contact.email);
    gtk_label_set_text(GTK_LABEL(Num), disp_contact.Number);

    gtk_widget_show_all(diag_display_contact);

    gint res = gtk_dialog_run(GTK_DIALOG(diag_display_contact));

    switch (res) {
        case GTK_RESPONSE_ACCEPT:
            break;

        case GTK_RESPONSE_NONE:
            break;
    }

    gtk_widget_hide(diag_display_contact);
}

void Print_Contact(GtkButton *btn, gpointer data) {
    int valid;
    int i;
    char *filename = "contacts.txt";
    FILE *fp = fopen(filename, "w");
    char s1[30], s2[30], s3[30], s4[30], s5[30], s6[30];

    fprintf(fp, "Contact Information\n");
    fprintf(fp, "--------------------------------------------\n");

    win_data *UIdata = data;
    contact edit_contact;

    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(UIdata->combo), &UIdata->comboIter) == 0) {
        gtk_label_set_text(GTK_LABEL(UIdata->combo), "You didn't choose a contact to print in the ComboBox.");
        return;
    }

    i = 0;
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(UIdata->liststore), &UIdata->iter);
    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(UIdata->liststore), &UIdata->iter,
                           0, &edit_contact.Name,
                           1, &edit_contact.Sur_Name,
                           2, &edit_contact.Address,
                           3, &edit_contact.Contact_Type,
                           4, &edit_contact.email,
                           5, &edit_contact.Number
                           ,-1);

        if (fp == NULL) {
            printf("Error opening the file %s", filename);
            exit(1);
        }

        fprintf(fp, "Name: %s\n", edit_contact.Name);
        fprintf(fp, "Surname: %s\n", edit_contact.Sur_Name);
        fprintf(fp, "Address: %s\n", edit_contact.Address);
        fprintf(fp, "Contact Type: %s\n", edit_contact.Contact_Type);
        fprintf(fp, "Email: %s\n", edit_contact.email);
        fprintf(fp, "Number: %s\n", edit_contact.Number);
        fprintf(fp, "--------------------------------------------\n");

        i++;
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(UIdata->liststore), &UIdata->iter);
    }

    fclose(fp);

    GtkWidget *InfoLabel = (GtkWidget *)gtk_builder_get_object(UIdata->build, "InfoLabel");
    gtk_label_set_text(GTK_LABEL(InfoLabel), "Contacts have been printed to contacts.txt");
}
