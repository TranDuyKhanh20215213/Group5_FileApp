﻿#pragma once
#include "Resources.h"
#include "DataIO.h"
#include "Account.h"
#include "Group.h"
#include "File.h"
#include <windows.h>
#include <gtk/gtk.h>

typedef struct Window
{
	GtkApplication *app;

	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *box;
	GtkWidget *buttons[10];

	GtkWidget *userName; // username
	GtkWidget *userID;	 // username
	GtkWidget *pass;	 // password
	GtkWidget *grName;	 // group name
	GtkWidget *folder;	 // folder name
	GtkWidget *folder2;	 // folder rename
	GtkWidget *file;	 // file name
	GtkWidget *file2;	 // file name

} Window;

SOCKET client;
Account acc;
Group gr;
char *curDir;

void check_login(GtkWidget *button, Window *win);
void signup(GtkWidget *b, Window *win);
void check_signup(GtkWidget *b, Window *win);
void main_page(GtkWidget *b, Window *win);
void show_group(GtkWidget *b, Window *win);
void show_other_group(GtkWidget *b, Window *win);
void create_group(GtkWidget *b, Window *win);
void enter_group(GtkWidget *b, Window *win);
void join_group(GtkWidget *b, Window *win);
void leave_group(GtkWidget *b, Window *win);
void log_out(GtkWidget *b, Window *win);
void check_group_create(GtkWidget *b, Window *win);
void check_group_enter(GtkWidget *b, Window *win);
void check_group_join(GtkWidget *b, Window *win);
void check_group_leave(GtkWidget *b, Window *win);
void in_group_page(GtkWidget *b, Window *win);
void upload_file(GtkWidget *b, Window *win);
void check_file_upload(GtkWidget *b, Window *win);
void download_file(GtkWidget *b, Window *win);
void check_file_download(GtkWidget *b, Window *win);
void delete_file(GtkWidget *b, Window *win);
void check_file_move(GtkWidget *button, Window *win);
void check_file_rename(GtkWidget *b, Window *win);
void check_file_delete(GtkWidget *b, Window *win);
void show_member(GtkWidget *b, Window *win);
void show_file(GtkWidget *b, Window *win);
void create_folder(GtkWidget *b, Window *win);
void delete_folder(GtkWidget *b, Window *win);
void check_folder_create(GtkWidget *b, Window *win);
void check_folder_rename(GtkWidget *b, Window *win);
void check_file_copy(GtkWidget *b, Window *win);
void check_folder_copy(GtkWidget *b, Window *win);
void check_folder_move(GtkWidget *b, Window *win);
void check_folder_delete(GtkWidget *b, Window *win);
void change_directory(GtkWidget *b, Window *win);
void check_directory_change(GtkWidget *b, Window *win);
void show_request(GtkWidget *b, Window *win);
void check_request(GtkWidget *b, Window *win);

void rename_folder(GtkWidget *b, Window *win);
void copy_file(GtkWidget *b, Window *win);
void copy_folder(GtkWidget *b, Window *win);
void move_folder(GtkWidget *b, Window *win);
void rename_file(GtkWidget *b, Window *win);
void remove_member(GtkWidget *button, Window *win);
void show_log(GtkWidget *b, Window *win);
void move_file(GtkWidget *button, Window *win);
void destroy_all_windows();
void add_window_to_list(GtkWidget *window);

GList *open_windows = NULL;

// initialize
void initialize(GtkApplication *app, Window *win)
{
	g_print("Initializing\n");

	GtkWidget *button, *button_1;
	GtkWidget *label_user;
	GtkWidget *label_pass;
	Window *win1;
	win1 = win;
	// create window
	win1->window = gtk_application_window_new(win1->app);
	g_print("Initialized\n");
	gtk_window_set_title(GTK_WINDOW(win1->window), "Log In");
	gtk_window_set_default_size(GTK_WINDOW(win1->window), 320, 200);

	// create grid
	win1->grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(win1->window), win1->grid);

	// label
	label_user = gtk_label_new("Username  ");
	gtk_grid_attach(GTK_GRID(win1->grid), label_user, 0, 1, 1, 1);
	label_pass = gtk_label_new("Password  ");
	gtk_grid_attach(GTK_GRID(win1->grid), label_pass, 0, 2, 1, 1);

	// insert field
	win1->userName = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->userName), "Username");
	gtk_grid_attach(GTK_GRID(win1->grid), win1->userName, 1, 1, 1, 1);

	win1->pass = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->pass), "Password");
	gtk_entry_set_visibility(GTK_ENTRY(win1->pass), 0); // hide password
	gtk_grid_attach(GTK_GRID(win1->grid), win1->pass, 1, 2, 1, 1);

	// button login
	button = gtk_button_new_with_label("\nLog In\n");
	gtk_grid_attach(GTK_GRID(win1->grid), button, 1, 3, 1, 1);
	// g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), win1->window);
	g_signal_connect(button, "clicked", G_CALLBACK(check_login), win1); // G_CALLBACK (Login) goi ham login

	// button signup
	button_1 = gtk_button_new_with_label("Sign Up");
	gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 6, 1, 1);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), win1->window);
	g_signal_connect(button_1, "clicked", G_CALLBACK(signup), win1);
	gtk_widget_set_visible(win1->window, true);
}

// active
void activate(GtkApplication *app, Window *win)
{
	initialize(app, win);
}

// on_open
void on_open(GApplication *application, GFile **files, gint n_files, const gchar *hint)
{
	gint i;

	for (i = 0; i < n_files; i++)
	{
		gchar *uri = g_file_get_uri(files[i]);
		g_print("open %s\n", uri);
		g_free(uri);
	}

	/* Note: when doing a longer-lasting action here that returns
	 * to the mainloop, you should use g_application_hold() and
	 * g_application_release() to keep the application alive until
	 * the action is completed.
	 */
}

void set_non_unique(GApplication *application, Window *win)
{
	// Disable unique bus name mechanism
	g_application_set_flags(application, G_APPLICATION_NON_UNIQUE);
}

// command_line
int command_line(GApplication *application, GApplicationCommandLine *cmdline, Window *win)
{
	gchar **argv;
	gint argc;
	gint i;

	argv = g_application_command_line_get_arguments(cmdline, &argc);

	for (i = 0; i < argc; i++)
		g_print("handling argument %s remotely\n", argv[i]);

	g_strfreev(argv);

	initialize(GTK_APPLICATION(application), win);

	return 0;
}

void check_login(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;
	GtkWidget *button1;
	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	// message msg;

	char username[30], password[30];
	strcpy(username, gtk_editable_get_text(GTK_EDITABLE(win1->userName)));
	// gtk_editable_get_text co kieu du lieu (char *)

	strcpy(password, gtk_editable_get_text(GTK_EDITABLE(win1->pass)));
	// //process:
	int result = login(client, acc, username, password);
	switch (result)
	{
	case SECOND_FRAME:
		g_print("Main page\n");
		gtk_window_destroy(GTK_WINDOW(win1->window));
		main_page(button1, win1);
		break;
	case WRONG_PASSWORD:
	case ID_NOT_FOUND:
		gtk_widget_set_visible(win1->window, true);
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Login failed");

		grid = gtk_grid_new();
		gtk_window_set_child(GTK_WINDOW(subwindow), grid);

		label = gtk_label_new("WRONG PASSWORD OR USERNAME");
		gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

		button1 = gtk_button_new_with_label("Retry");
		g_signal_connect_swapped(button1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect_swapped(button1, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

		gtk_grid_attach(GTK_GRID(grid), button1, 1, 3, 1, 1);

		gtk_widget_set_visible(subwindow, true);
		break;
	case CANT_LOGIN:
		gtk_widget_set_visible(win1->window, true);
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Account in use");

		grid = gtk_grid_new();
		gtk_window_set_child(GTK_WINDOW(subwindow), grid);

		label = gtk_label_new("ACCOUNT IS IN USE");
		gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

		button1 = gtk_button_new_with_label("Retry");
		g_signal_connect_swapped(button1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect_swapped(button1, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

		gtk_grid_attach(GTK_GRID(grid), button1, 1, 3, 1, 1);

		gtk_widget_set_visible(subwindow, true);
		break;
	default:
		gtk_widget_set_visible(win1->window, true);
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Something wrong");

		grid = gtk_grid_new();
		gtk_window_set_child(GTK_WINDOW(subwindow), grid);

		label = gtk_label_new("SOME THING WRONG!");
		gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

		button1 = gtk_button_new_with_label("Retry");
		g_signal_connect_swapped(button1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect_swapped(button1, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

		gtk_grid_attach(GTK_GRID(grid), button1, 1, 3, 1, 1);

		gtk_widget_set_visible(subwindow, true);
		break;
	}
}

void signup(GtkWidget *b, Window *win)
{
	GtkWidget *button, *button_1;
	GtkWidget *label_1, *label_2, *label_3, *label_4, *label_5, *label_6, *re_pass;
	Window *win1;
	win1 = win;

	// create window
	win1->window = gtk_application_window_new(win1->app);
	gtk_window_set_title(GTK_WINDOW(win1->window), "Sign Up");
	gtk_window_set_default_size(GTK_WINDOW(win1->window), 320, 200);

	// create grid
	win1->grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(win1->window), win1->grid);

	// label
	label_1 = gtk_label_new("Username:");
	gtk_grid_attach(GTK_GRID(win1->grid), label_1, 0, 1, 1, 1);
	label_2 = gtk_label_new("Password:");
	gtk_grid_attach(GTK_GRID(win1->grid), label_2, 0, 2, 1, 1);

	// insert field
	win1->userName = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->userName), "Username");
	gtk_grid_attach(GTK_GRID(win1->grid), win1->userName, 1, 1, 1, 1);

	win1->pass = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->pass), "Password");
	gtk_entry_set_visibility(GTK_ENTRY(win1->pass), 0); // hide password
	gtk_grid_attach(GTK_GRID(win1->grid), win1->pass, 1, 2, 1, 1);

	// button signup
	button = gtk_button_new_with_label("\nSign Up\n");
	gtk_grid_attach(GTK_GRID(win1->grid), button, 1, 7, 1, 1);
	//   g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_hide), win1->window);
	g_signal_connect(button, "clicked", G_CALLBACK(check_signup), win1);

	// button login
	button_1 = gtk_button_new_with_label("Back");
	gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 8, 1, 1);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), win1->window);
	g_signal_connect(button_1, "clicked", G_CALLBACK(activate), win1);

	gtk_widget_set_visible(win1->window, true);
}

void check_signup(GtkWidget *button, Window *win)
{
	GtkWidget *subwindow, *grid, *label, *button1;
	char username[30], password[30];

	strcpy(username, gtk_editable_get_text(GTK_EDITABLE(win->userName)));
	strcpy(password, gtk_editable_get_text(GTK_EDITABLE(win->pass)));

	int result = registerAcc(client, username, password);
	if (result == REGISTER_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Register successfully!");

		grid = gtk_grid_new();
		gtk_window_set_child(GTK_WINDOW(subwindow), grid);

		label = gtk_label_new("");
		gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

		button = gtk_button_new_with_label("OK");
		gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
		g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win->window);

		gtk_widget_set_visible(subwindow, true);
	}
	// if group name is invalid
	else if (result == ACCOUNT_EXIST)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Username exist!");

		grid = gtk_grid_new();
		gtk_window_set_child(GTK_WINDOW(subwindow), grid);

		label = gtk_label_new("");
		gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

		button = gtk_button_new_with_label("OK");
		gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
		g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win->window);

		gtk_widget_set_visible(subwindow, true);
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");

		grid = gtk_grid_new();
		gtk_window_set_child(GTK_WINDOW(subwindow), grid);

		label = gtk_label_new("");
		gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

		button = gtk_button_new_with_label("OK");
		gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
		g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);
		g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win->window);

		gtk_widget_set_visible(subwindow, true);
	}
}

void main_page(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	// 1_Tao nhom 2_Tham gia nhom 3_Nhom da tham gia 4_Nhom cua ban
	GtkWidget *button_1, *label;
	GtkWidget *subwindow;

	subwindow = gtk_application_window_new(win1->app);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Menu");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 640, 400);

	win1->grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), win1->grid);

	// label
	char *hello = (char *)malloc(sizeof(char) * 20);
	strcpy(hello, "Hello ");
	label = gtk_label_new(strcat(hello, acc.user));
	gtk_grid_attach(GTK_GRID(win1->grid), label, 1, 0, 1, 1);
	free(hello);

	// button

	button_1 = gtk_button_new_with_label("\t\t\t\nShow your group\n\n");
	gtk_grid_attach(GTK_GRID(win1->grid), button_1, 0, 5, 1, 1);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button_1, "clicked", G_CALLBACK(show_group), win1);

	button_1 = gtk_button_new_with_label("\t\t\t\nShow other group\n\n");
	gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 5, 1, 1);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button_1, "clicked", G_CALLBACK(show_other_group), win1);

	button_1 = gtk_button_new_with_label("\t\t\t\nCreate group\n\n");
	gtk_grid_attach(GTK_GRID(win1->grid), button_1, 2, 5, 1, 1);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button_1, "clicked", G_CALLBACK(create_group), win1);

	// button_1 = gtk_button_new_with_label("\t\t\t\nEnter Group\n\n");
	// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 3, 5, 1, 1);
	// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	// g_signal_connect(button_1, "clicked", G_CALLBACK(enter_group), win1);

	// button_1 = gtk_button_new_with_label("\t\t\t\nJoin Group\n\n");
	// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 4, 5, 1, 1);
	// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	// g_signal_connect(button_1, "clicked", G_CALLBACK(join_group), win1);

	// button_1 = gtk_button_new_with_label("\t\t\t\nLeave Group\n\n");
	// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 5, 5, 1, 1);
	// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	// g_signal_connect(button_1, "clicked", G_CALLBACK(leave_group), win1);

	button_1 = gtk_button_new_with_label("\t\t\t\nLog out\n\n");
	gtk_grid_attach(GTK_GRID(win1->grid), button_1, 6, 5, 1, 1);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(log_out), subwindow);
	g_signal_connect(button_1, "clicked", G_CALLBACK(initialize), win1);

	gtk_widget_set_visible(subwindow, true);
}

void show_group(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Your Group");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 500, 400);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// group list
	char *groupList = (char *)malloc(sizeof(char) * 100);
	showGroup(client, acc.id, SHOW_MY_GROUP, groupList);
	label = gtk_label_new("\t\tGroups you are in: ");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	// label = gtk_label_new("\n\n");
	// gtk_grid_attach(GTK_GRID(subgrid), label, 2, 1, 1, 1);
	label = gtk_label_new(groupList);
	gtk_grid_attach(GTK_GRID(subgrid), label, 2, 1, 1, 1);

	button = gtk_button_new_with_label("Back");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(main_page), win1);
	win1->window = subwindow;

	button = gtk_button_new_with_label("Enter group");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 2, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(enter_group), win1);
	win1->window = subwindow;

	button = gtk_button_new_with_label("Leave group");
	gtk_grid_attach(GTK_GRID(subgrid), button, 1, 2, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(leave_group), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void show_other_group(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Your Group");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// group list
	char *groupList = (char *)malloc(sizeof(char) * 100);
	showGroup(client, acc.id, SHOW_OTHER_GROUP, groupList);
	label = gtk_label_new("\t\tOther groups: ");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 2, 1, 1, 1);
	label = gtk_label_new(groupList);
	gtk_grid_attach(GTK_GRID(subgrid), label, 3, 1, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 2, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(main_page), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
	button = gtk_button_new_with_label("Join group");
	gtk_grid_attach(GTK_GRID(subgrid), button, 1, 2, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(join_group), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void create_group(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Create Group");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Create Group!");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nGroup Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->grName = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->grName), "Group Name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->grName, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(main_page), win1);

	button_1 = gtk_button_new_with_label("Submit");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_group_create), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void enter_group(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Enter Group");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Enter Group!");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nGroup Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->grName = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->grName), "Group Name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->grName, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(main_page), win1);

	button_1 = gtk_button_new_with_label("Submit");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_group_enter), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void join_group(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Join Group");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Join Group!");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nGroup Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->grName = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->grName), "Group Name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->grName, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(main_page), win1);

	button_1 = gtk_button_new_with_label("Submit");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_group_join), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void leave_group(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Leave Group");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Leave Group!");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nGroup Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->grName = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->grName), "Group Name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->grName, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(main_page), win1);

	button_1 = gtk_button_new_with_label("Submit");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_group_leave), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void log_out(GtkWidget *button, Window *win)
{
	logout(client, acc.id);
}

void check_group_create(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char gname[30];
	strcpy(gname, gtk_editable_get_text(GTK_EDITABLE(win1->grName)));

	if (strlen(gname) == 0)
	{
		return;
	}

	// if success:
	int result = createGroup(client, acc.id, gname);
	if (result == CREATE_GROUP_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Create group successfully!");
	}
	// if group name is invalid
	else if (result == GROUP_ALREADY_EXIST)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Group exist!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void check_group_enter(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char gname[30];
	strcpy(gname, gtk_editable_get_text(GTK_EDITABLE(win1->grName)));

	if (strlen(gname) == 0)
	{
		return;
	}

	// if success:
	int result = enterGroup(client, acc, gr, gname);
	if (result == ENTER_SUCCESS)
	{
		// Initialize directory
		curDir = (char *)malloc(sizeof(char) * 50);
		sprintf(curDir, "%s", gr.nameGroup);

		// Move to next screen
		gtk_window_destroy(GTK_WINDOW(win1->window));
		in_group_page(button, win1);

		return;
	}
	// if group name is invalid
	else if (result == ENTER_FAILED)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Enter group failed!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void check_group_join(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char gname[30];
	strcpy(gname, gtk_editable_get_text(GTK_EDITABLE(win1->grName)));

	if (strlen(gname) == 0)
	{
		return;
	}

	// if success:
	int result = joinGroup(client, acc.id, gname);
	if (result == REQUEST_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Request sent!");
	}
	// if group name is invalid
	else if (result == GROUP_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Group not found!");
	}
	else if (result == ALREADY_IN_GROUP)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Already In Group!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void check_group_leave(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char gname[30];
	strcpy(gname, gtk_editable_get_text(GTK_EDITABLE(win1->grName)));

	if (strlen(gname) == 0)
	{
		return;
	}

	// if success:
	int result = leaveGroup(client, acc.id, gname);
	if (result == LEAVE_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Leave group successfully!");
	}
	else if (result == GROUP_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Group not found!");
	}
	else if (result == LEAVE_FAILED)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Leave group failed!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void in_group_page(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;
	GtkWidget *label;
	GtkWidget *button_1;
	GtkWidget *subwindow;

	subwindow = gtk_application_window_new(win1->app);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Menu");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 640, 400);

	win1->grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), win1->grid);

	// label
	char *hello = (char *)malloc(sizeof(char) * 20);
	strcpy(hello, "Hello ");
	label = gtk_label_new(strcat(hello, acc.user));
	gtk_grid_attach(GTK_GRID(win1->grid), label, 0, 0, 1, 1);
	free(hello);

	label = gtk_label_new(curDir);
	gtk_grid_attach(GTK_GRID(win1->grid), label, 0, 1, 1, 1);

	// if admin role
	if (acc.role == 1)
	{
		// button_1 = gtk_button_new_with_label("\t\t\t\nUpload file\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 0, 2, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(upload_file), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nDowload file\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 2, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(download_file), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nDelete file\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 2, 2, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(delete_file), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nMove file\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 3, 2, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(move_file), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nList Member\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 0, 2, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(show_member), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nDelete Member\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 5, 2, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(remove_member), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nList of file/folder\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 0, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(show_file), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nCreate folder\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 3, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(create_folder), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nDelete folder\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 2, 3, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(delete_folder), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nRename file\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 4, 3, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(rename_file), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nMove to folder\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 5, 3, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(change_directory), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nShow request\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(show_request), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nShow log\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 2, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(show_log), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nBack\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 3, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(main_page), win1);
	}
	else // if member role
	{
		// button_1 = gtk_button_new_with_label("\t\t\t\nUpload file\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 0, 2, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(upload_file), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nDowload file\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 2, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(download_file), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nList Member\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 2, 2, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(show_member), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nList of file/folder\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 3, 2, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(show_file), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nCreate folder\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 0, 3, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(create_folder), win1);

		// button_1 = gtk_button_new_with_label("\t\t\t\nMove to folder\n\n");
		// gtk_grid_attach(GTK_GRID(win1->grid), button_1, 1, 3, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_1, "clicked", G_CALLBACK(change_directory), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nShow log\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 2, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(show_log), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nBack\n\n");
		gtk_grid_attach(GTK_GRID(win1->grid), button_1, 3, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(main_page), win1);
	}
	gtk_widget_set_visible(subwindow, true);
}

void upload_file(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Upload File");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Upload file");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nFile to upload: ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->file = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->file), "File path\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->file, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Upload");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_file_upload), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void check_file_upload(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char path[30];
	strcpy(path, gtk_editable_get_text(GTK_EDITABLE(win1->file)));

	if (strlen(path) == 0)
	{
		return;
	}

	// if success:
	int result = upload(client, curDir, path);
	gtk_init();
	subwindow = gtk_window_new();
	if (result == UPLOAD_SUCCESS)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "Upload successfully!");
	}
	else if (result == FILE_NOT_FOUND)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "File not found!");
	}
	else if (result == -1)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "Upload failed!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void download_file(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Download File");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Download file");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nFile to download: ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->file = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->file), "File path\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->file, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Download");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_file_download), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void check_file_download(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char path[30];
	strcpy(path, gtk_editable_get_text(GTK_EDITABLE(win1->file)));

	if (strlen(path) == 0)
	{
		return;
	}

	// if success:
	int result = download(client, curDir, path);
	gtk_init();
	subwindow = gtk_window_new();
	if (result == DOWNLOAD_SUCCESS)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "Download successfully!");
	}
	else if (result == FILE_NOT_FOUND)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "File not found!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void delete_file(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Delete File");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Delete file");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nFile to delete: ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->file = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->file), "File path\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->file, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Delete");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_file_delete), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void move_file(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Move File");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 500, 500);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Move File");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label_1 = gtk_label_new("File to be moved:   ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 2, 1, 1);

	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 3, 1, 1);

	label_1 = gtk_label_new("Destination:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 4, 1, 1);
	win1->folder2 = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder2), "Destination\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder2, 1, 4, 1, 1);
	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Move");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_file_move), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void check_file_delete(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char path[30];
	strcpy(path, gtk_editable_get_text(GTK_EDITABLE(win1->file)));

	if (strlen(path) == 0)
	{
		return;
	}

	// if success:
	int result = deleteFile(client, curDir, path);
	gtk_init();
	subwindow = gtk_window_new();
	if (result == DELETE_FILE_SUCCESS)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "Delete file successfully!");
	}
	else if (result == FILE_NOT_FOUND)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "File not found!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void check_member_remove(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char user[30];
	strcpy(user, gtk_editable_get_text(GTK_EDITABLE(win1->file)));

	if (strlen(user) == 0)
	{
		return;
	}

	// if success:
	int result = removeMember(client, gr.nameGroup, user);
	gtk_init();
	subwindow = gtk_window_new();
	if (result == DELETE_MEMBER_SUCCESS)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "Delete member successfully!");
	}
	else if (result == DELETE_MEMBER_FAILED)
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "Member not found or not in group!");
	}
	else
	{
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some ting wong");
	}

	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void remove_member(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Delete Member");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Delete Member");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nEnter member: ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->file = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->file), "Member name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->file, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(in_group_page), win1);

	button_1 = gtk_button_new_with_label("Delete");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_member_remove), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void show_member(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Show all members");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// group list
	char *memberList = (char *)malloc(sizeof(char) * 100);
	showListMember(client, gr.nameGroup, memberList);
	label = gtk_label_new("\t\tMembers in group: ");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 2, 1, 1, 1);
	label = gtk_label_new(memberList);
	gtk_grid_attach(GTK_GRID(subgrid), label, 3, 1, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 2, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(in_group_page), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
	if (acc.role == 1)
	{
		button = gtk_button_new_with_label("Delete Member");
		gtk_grid_attach(GTK_GRID(subgrid), button, 1, 2, 1, 1);
		g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button, "clicked", G_CALLBACK(remove_member), win1);
		win1->window = subwindow;
		gtk_widget_set_visible(subwindow, true);
	}
}

void show_file(GtkWidget *button, Window *win)
{
	Window *win1;
	Window *win2;
	win1 = win;
	win2 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label;
	GtkWidget *button_1, *button_2, *button_3, *button_4, *button5, *button_6;
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Show all files/folders");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 600, 600);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// group list
	char *listFile = (char *)malloc(sizeof(char) * 100);
	showListFile(client, curDir, listFile);
	label = gtk_label_new("\t\tAll files/folders: \n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label = gtk_label_new(listFile);
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("EXIT");
	gtk_grid_attach(GTK_GRID(subgrid), button, 1, 6, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(in_group_page), win1);

	button_1 = gtk_button_new_with_label("Enter folder");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 3, 5, 1, 1);
	g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button_1, "clicked", G_CALLBACK(change_directory), win1);

	button_1 = gtk_button_new_with_label("Upload file");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 3, 1, 1);
	// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button_1, "clicked", G_CALLBACK(upload_file), win1);

	button_1 = gtk_button_new_with_label("Dowload file");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 2, 3, 1, 1);
	// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button_1, "clicked", G_CALLBACK(download_file), win1);

	if (acc.role == 1)
	{
		button_1 = gtk_button_new_with_label("Delete file");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 3, 3, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(delete_file), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nCreate folder\n\n");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 4, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(create_folder), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nDelete folder\n\n");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 5, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(delete_folder), win1);

		button_1 = gtk_button_new_with_label("Rename folder");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 4, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(rename_folder), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nRename file\n\n");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 2, 4, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(rename_file), win1);

		button_1 = gtk_button_new_with_label("Move folder");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 3, 4, 1, 1);
		// g_signal_connect_swapped(button_2, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(move_folder), win1);

		button_1 = gtk_button_new_with_label("Move file");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 4, 4, 1, 1);
		// g_signal_connect_swapped(button_3, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(move_file), win1);

		button_1 = gtk_button_new_with_label("\t\t\t\nCopy file\n\n");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 5, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(copy_file), win1);

		button_1 = gtk_button_new_with_label("Copy folder");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 2, 5, 1, 1);
		// g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(copy_folder), win1);

		win1->window = subwindow;
		gtk_widget_set_visible(subwindow, true);
	}
	else
	{
		button_1 = gtk_button_new_with_label("\t\t\t\nCreate folder\n\n");
		gtk_grid_attach(GTK_GRID(subgrid), button_1, 3, 3, 1, 1);
		g_signal_connect_swapped(button_1, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_1, "clicked", G_CALLBACK(create_folder), win1);

		button_2 = gtk_button_new_with_label("Move folder");
		gtk_grid_attach(GTK_GRID(subgrid), button_2, 1, 4, 1, 1);
		// g_signal_connect_swapped(button_2, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_2, "clicked", G_CALLBACK(move_folder), win1);

		button_3 = gtk_button_new_with_label("Move file");
		gtk_grid_attach(GTK_GRID(subgrid), button_3, 2, 4, 1, 1);
		// g_signal_connect_swapped(button_3, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		g_signal_connect(button_3, "clicked", G_CALLBACK(move_file), win1);

		// button_3 = gtk_button_new_with_label("Copy folder");
		// gtk_grid_attach(GTK_GRID(subgrid), button_3, 3, 3, 1, 1);
		// g_signal_connect_swapped(button_3, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
		// g_signal_connect(button_3, "clicked", G_CALLBACK(copy_folder), win1);
		win1->window = subwindow;
		gtk_widget_set_visible(subwindow, true);
	}
}

void create_folder(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Create Folder");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Create Folder!");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nFolder Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "Folder name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Create");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_folder_create), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}
void move_folder(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Move Folder");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 500, 500);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);
	// group list
	// char *listFile = (char *)malloc(sizeof(char) * 100);
	// showListFile(client, curDir, listFile);
	// label = gtk_label_new("\t\tAll files/folders: \n");
	// gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	// label = gtk_label_new(listFile);
	// gtk_grid_attach(GTK_GRID(subgrid), label, 0, 2, 1, 1);

	// label
	label = gtk_label_new("Move Folder");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label_1 = gtk_label_new("  Source folder  ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 2, 1, 1);

	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 3, 1, 1);

	label_1 = gtk_label_new("  Destination Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 4, 1, 1);
	win1->folder2 = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder2), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder2, 1, 4, 1, 1);
	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Move");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_folder_move), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void rename_folder(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Rename Folder");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 500, 500);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Rename Folder");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label_1 = gtk_label_new("source name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 2, 1, 1);

	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 3, 1, 1);

	label_1 = gtk_label_new("Destination Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 4, 1, 1);
	win1->folder2 = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder2), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder2, 1, 4, 1, 1);
	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Rename");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_folder_rename), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}
void copy_file(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Copy file");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 500, 500);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Copy file");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label_1 = gtk_label_new("Source file name");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 2, 1, 1);

	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 3, 1, 1);

	label_1 = gtk_label_new("Destination Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 4, 1, 1);
	win1->folder2 = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder2), "destination name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder2, 1, 4, 1, 1);
	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Copy");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_file_copy), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void copy_folder(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Copy folder");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 500, 500);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Copy folder");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label_1 = gtk_label_new("Source file name");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 2, 1, 1);

	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 3, 1, 1);

	label_1 = gtk_label_new("Destination Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 4, 1, 1);
	win1->folder2 = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder2), "destination name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder2, 1, 4, 1, 1);
	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Copy");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_folder_copy), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}
void rename_file(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Rename File");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 500, 500);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Rename File");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label_1 = gtk_label_new("Destination to be renamed:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->file = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->file), "Destination name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->file, 1, 2, 1, 1);

	label = gtk_label_new("\n\n");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 3, 1, 1);

	label_1 = gtk_label_new("Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 4, 1, 1);
	win1->file2 = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->file2), "name\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->file2, 1, 4, 1, 1);
	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Rename");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_file_rename), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void delete_folder(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Delete Folder");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Delete Folder!");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\nFoler Name:    ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "Folder path\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect(button, "clicked", G_CALLBACK(destroy_all_windows), NULL);
	g_signal_connect(button, "clicked", G_CALLBACK(show_file), win1);

	button_1 = gtk_button_new_with_label("Delete");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_folder_delete), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void check_folder_create(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char folder[30];
	strcpy(folder, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));

	if (strlen(folder) == 0)
	{
		return;
	}

	// if success:
	int result = createFolder(client, curDir, folder);
	if (result == CREATE_FOLDER_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Create folder successfully!");
	}
	// if group name is invalid
	else if (result == FOLDER_ALREADY_EXIST)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Folder exist!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}
void check_folder_move(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char folder[30];
	char name[30];
	strcpy(folder, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));
	strcpy(name, gtk_editable_get_text(GTK_EDITABLE(win1->folder2)));

	// if success:
	int result = moveFolder(client, gr.nameGroup, folder, name);
	if (result == MOVE_FOLDER_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Move folder successfully!");
		// g_signal_connect(button, "clicked", G_CALLBACK(gtk_window_destroy), win1->window);
	}
	// if group name is invalid
	else if (result == MOVE_FOLDER_FAILED)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Move folder failed");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);

	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}
void check_folder_rename(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char folder[30];
	char rename[30];
	strcpy(folder, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));
	strcpy(rename, gtk_editable_get_text(GTK_EDITABLE(win1->folder2)));
	if (strlen(folder) == 0)
	{
		return;
	}

	if (strlen(rename) == 0)
	{
		return;
	}

	// if success:
	int result = renameFolder(client, curDir, folder, rename);
	if (result == RENAME_FOLDER_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Rename folder successfully!");
	}
	// if group name is invalid
	else if (result == FOLDER_ALREADY_EXIST)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Folder name exist!");
	}
	else if (result == FOLDER_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Folder not found");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void check_file_copy(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char source[50];
	char copy[50];
	strcpy(source, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));
	strcpy(copy, gtk_editable_get_text(GTK_EDITABLE(win1->folder2)));
	if (strlen(source) == 0)
	{
		return;
	}

	// if success:
	int result = copyFile(client, curDir, source, copy);
	if (result == COPY_FILE_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Copy file successfully!");
	}
	else if (result == FILE_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "File not found!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}
void check_folder_copy(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char source[50];
	char copy[50];
	strcpy(source, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));
	strcpy(copy, gtk_editable_get_text(GTK_EDITABLE(win1->folder2)));
	if (strlen(source) == 0)
	{
		return;
	}

	// if success:
	int result = copyFolder(client, curDir, source, copy);
	if (result == COPY_FOLDER_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Copy folder successfully!");
	}
	else if (result == FOLDER_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Folder not found!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}
void check_file_move(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char folder[50];
	char name[50];
	strcpy(folder, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));
	strcpy(name, gtk_editable_get_text(GTK_EDITABLE(win1->folder2)));

	// if success:
	int result = moveFile(client, gr.nameGroup, folder, name);
	if (result == MOVE_FILE_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Move file successfully!");
	}
	// if group name is invalid
	else if (result == MOVE_FILE_FAILED)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Move file failed");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void check_file_rename(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char file[30];
	char rename[30];
	strcpy(file, gtk_editable_get_text(GTK_EDITABLE(win1->file)));
	strcpy(rename, gtk_editable_get_text(GTK_EDITABLE(win1->file2)));
	if (strlen(file) == 0)
	{
		return;
	}

	if (strlen(rename) == 0)
	{
		return;
	}

	// if success:
	int result = renameFile(client, curDir, file, rename);
	if (result == RENAME_FILE_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Rename file successfully!");
	}
	// if group name is invalid
	else if (result == FILE_ALREADY_EXIST)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "File name exist!");
	}
	else if (result == FILE_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "File not found!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void check_folder_delete(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char folder[30];
	strcpy(folder, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));

	if (strlen(folder) == 0)
	{
		return;
	}

	// if success:
	int result = deleteFolder(client, curDir, folder);
	if (result == DELETE_FOLDER_SUCCESS)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Delete folder successfully!");
	}
	// if group name is invalid
	else if (result == FOLDER_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Folder not found!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void change_directory(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label, *label_1, *label_2, *button_1;
	//
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Change directory");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// label
	label = gtk_label_new("Move to folder");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 0, 1, 1);
	label_1 = gtk_label_new("\fFolder path: ");
	gtk_grid_attach(GTK_GRID(subgrid), label_1, 0, 2, 1, 1);

	// insert field
	win1->folder = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->folder), "Folder path\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->folder, 1, 3, 1, 1);

	// button
	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 8, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(in_group_page), win1);

	button_1 = gtk_button_new_with_label("Change");
	gtk_grid_attach(GTK_GRID(subgrid), button_1, 1, 8, 1, 1);
	g_signal_connect(button_1, "clicked", G_CALLBACK(check_directory_change), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void check_directory_change(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char folder[30];
	strcpy(folder, gtk_editable_get_text(GTK_EDITABLE(win1->folder)));

	if (strlen(folder) == 0)
	{
		return;
	}

	// if success:
	int result = changeDirectory(client, curDir, folder);
	if (result == CHANGE_SUCCESS)
	{

		// Move to next screen
		gtk_window_destroy(GTK_WINDOW(win1->window));
		in_group_page(button, win1);

		return;
	}
	// if group name is invalid
	else if (result == FOLDER_NOT_FOUND)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Folder not found!");
	}
	else if (result == -1)
	{
		gtk_init();
		subwindow = gtk_window_new();
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}

void show_request(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *subgrid;
	GtkWidget *subwindow;
	GtkWidget *label;
	//
	// Cua so nhap ten nhom, mo ta nhom
	//
	subwindow = gtk_application_window_new(win1->app);
	add_window_to_list(subwindow);
	gtk_window_set_title(GTK_WINDOW(subwindow), "Show request");
	gtk_window_set_default_size(GTK_WINDOW(subwindow), 320, 200);

	subgrid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), subgrid);

	// request list
	char *memberList = (char *)malloc(sizeof(char) * 100);
	showRequest(client, gr.nameGroup, memberList);
	if (strcmp(memberList, "") == 0 || strcmp(memberList, "") == 0 || !isalnum(memberList[0]))
	{
		g_print("No request\n");
		sprintf(memberList, "None");
	}

	label = gtk_label_new("\t\tAll request:\t");
	gtk_grid_attach(GTK_GRID(subgrid), label, 0, 1, 1, 1);
	label = gtk_label_new(memberList);
	gtk_grid_attach(GTK_GRID(subgrid), label, 2, 1, 1, 1);

	win1->userID = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(win1->userID), "Accept ID\n");
	gtk_grid_attach(GTK_GRID(subgrid), win1->userID, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 3, 1, 1);
	// g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(check_request), win1);

	button = gtk_button_new_with_label("Cancel");
	gtk_grid_attach(GTK_GRID(subgrid), button, 0, 4, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(in_group_page), win1);
	win1->window = subwindow;
	gtk_widget_set_visible(subwindow, true);
}

void check_request(GtkWidget *button, Window *win)
{
	Window *win1;
	win1 = win;

	GtkWidget *label;
	GtkWidget *grid;
	GtkWidget *subwindow;
	int bytes_sent, bytes_recv;
	char userID[30];
	strcpy(userID, gtk_editable_get_text(GTK_EDITABLE(win1->userID)));

	if (strlen(userID) == 0)
	{
		return;
	}

	// if success:
	int result = acceptRequet(client, gr.nameGroup, userID);
	if (result == ACCEPT_SUCCESS)
	{

		// Move to request screen
		printf("ACCEPT_SUCCESS\n");
		gtk_window_set_title(GTK_WINDOW(subwindow), "Accept successful!");

		gtk_window_destroy(GTK_WINDOW(win1->window));
		show_request(button, win1);
		return;
	}
	// if userID is invalid
	else if (result == ID_NOT_FOUND)
	{
		printf("ID_NOT_FOUND\n");
		gtk_window_set_title(GTK_WINDOW(subwindow), "UserID not found!");
	}
	else if (result == -1)
	{
		printf("ERROR\n");
		gtk_window_set_title(GTK_WINDOW(subwindow), "Some thing wrong!");
	}

	gtk_init();
	subwindow = gtk_window_new();
	grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(subwindow), grid);

	label = gtk_label_new("");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	button = gtk_button_new_with_label("OK");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 3, 1, 1);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), subwindow);
	g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_show), win1->window);

	gtk_widget_set_visible(subwindow, true);
}
void show_log(GtkWidget *b, Window *win)
{
	showLog(client, gr.nameGroup);

	GtkWidget *log_window = gtk_application_window_new(win->app);
	gtk_window_set_title(GTK_WINDOW(log_window), "Log Viewer");
	gtk_window_set_default_size(GTK_WINDOW(log_window), 320, 120);

	GtkWidget *grid = gtk_grid_new();
	gtk_window_set_child(GTK_WINDOW(log_window), grid);

	GtkWidget *log_label = gtk_label_new("Logs printed to console.");
	gtk_grid_attach(GTK_GRID(grid), log_label, 0, 0, 1, 1);

	GtkWidget *back_button = gtk_button_new_with_label("Back to group");
	gtk_grid_attach(GTK_GRID(grid), back_button, 0, 1, 1, 1);

	// Thêm khoảng cách giữa các widget
	gtk_widget_set_margin_bottom(log_label, 10);
	gtk_widget_set_margin_top(back_button, 10);

	// Signal quay trở lại giao diện chính
	g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(gtk_window_destroy), log_window);
	g_signal_connect(back_button, "clicked", G_CALLBACK(in_group_page), win);

	gtk_widget_show(log_window); // Hiển thị cửa sổ log
}

void destroy_all_windows()
{
    while (open_windows != NULL)
    {
        if (GTK_IS_WINDOW(open_windows->data))
        {
            gtk_window_destroy(GTK_WINDOW(open_windows->data));
        }
        open_windows = g_list_delete_link(open_windows, open_windows);
    }
}
void add_window_to_list(GtkWidget *window)
{
	open_windows = g_list_append(open_windows, window);
}