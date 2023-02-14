#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *CREDENTIALS_FILE = "credential_file";

struct user {
  char username[50];
  char password[256];
  char firstname[50];
  char lastname[50];
  int admin;
};

char *cs621Hash(const char *password) { return crypt(password, "00"); }

int get_num_lines(char *filename) {
  int counter = 0;
  int buffer_size = 100; // random buffer - is there a better way?
  char *buffer = malloc(buffer_size * sizeof(char));
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file %s\nExiting.", CREDENTIALS_FILE);
    exit(1);
  }
  while (fgets(buffer, buffer_size, file) != NULL) {
    counter++;
  }
  fclose(file);
  return counter;
}

struct user *createUsers(int *count) {
  int num_users = get_num_lines(CREDENTIALS_FILE);
  struct user *users = malloc(num_users * sizeof(struct user));
  (*count) = num_users;
  return users;
}

void populateUsers(void *users) {
  FILE *file;
  int buffer_size = 100; // again - random buffer size, better way?
  char *buffer = malloc(buffer_size * sizeof(char));
  file = fopen(CREDENTIALS_FILE, "r");
  if (file == NULL) {
    printf("Error opening file %s\nExiting.", CREDENTIALS_FILE);
    exit(1);
  }
  struct user *users_ptr = users;
  while (fgets(buffer, buffer_size, file) != NULL) {
    struct user new_user;
    int index = 0;
    char *token = strtok(buffer, "\t");
    while (token != NULL) {
      switch (index) {
      case 0:
        strcpy(new_user.firstname, token);
        break;
      case 1:
        strcpy(new_user.lastname, token);
        break;
      case 2:
        strcpy(new_user.username, token);
        break;
      case 3:
        strcpy(new_user.password, token);
        break;
      case 4:
        new_user.admin = atoi(token);
        break;
      }
      index++;
      token = strtok(NULL, "\t");
    }
    *users_ptr = new_user;
    users_ptr = users_ptr + 1;
  }
  fclose(file);
}

int checkAdminPassword(char *password, struct user *users, int count) {
  for (int i = 0; i < count; ++i) {
    struct user *u = users + i;
    if (strcmp(u->username, "admin") == 0) {
      if (strcmp(u->password, password)) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  return 0;
}

struct user *addUser(struct user *users, int *count, char *username,
                     char *password, char *firstname, char *lastname,
                     int admin) {
  (*count)++;
  users = realloc(users, (*count) * sizeof(struct user));
  struct user *u = users;
  u = u + (*count) - 1;
  strcpy(u->username, username);
  strcpy(u->password, cs621Hash(password));
  strcpy(u->firstname, firstname);
  strcpy(u->lastname, lastname);
  u->admin = admin;
  return users;
}

void saveUsers(struct user *users, int count) {
  FILE *file;
  file = fopen(CREDENTIALS_FILE, "w");
  if (file == NULL) {
    printf("Error saving users in %s\n", CREDENTIALS_FILE);
    exit(1);
  }
  for (int i = 0; i < count; i++) {
    struct user *u = users + i;
    fprintf(file, "%s\t%s\t%s\t%s\t%d\n", u->firstname, u->lastname,
            u->username, u->password, u->admin);
  }
  fclose(file);
}

int main(void) {
  int user_count = 0;
  struct user *users = createUsers(&user_count);
  if (users == NULL) {
    return EXIT_FAILURE;
  }
  populateUsers(users);

  printf("Enter admin password to add new users:");
  char entered_admin_password[50];
  scanf("%s", entered_admin_password);
  if (checkAdminPassword(entered_admin_password, users, user_count)) {
    struct user new_user;
    printf("\nEnter details for new user.\n");
    printf("Enter username:");
    scanf("%s", new_user.username);
    printf("Enter first name:");
    scanf("%s", new_user.firstname);
    printf("Enter last name:");
    scanf("%s", new_user.lastname);
    printf("Enter password:");
    scanf("%s", new_user.password);
    printf("Enter admin level:");
    scanf("%d", &(new_user.admin));
    users = addUser(users, &user_count, new_user.username, new_user.password,
                    new_user.firstname, new_user.lastname, new_user.admin);
    if (users == NULL) {
      return EXIT_FAILURE;
    }
  } else {
    printf("Incorrect Password.\n");
  }
  saveUsers(users, user_count);
  free(users);
  return EXIT_SUCCESS;
}
