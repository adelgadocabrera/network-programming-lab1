#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct user {
  char username[50];
  char password[50];
  int admin;
};

struct user *createUsers(int max_number_of_users) {
  struct user *users = malloc(max_number_of_users * sizeof(struct user));
  return users;
}

void initUsers(void *users, int max_number_of_users) {
  struct user *user = users;
  for (int i = 0; i < max_number_of_users; i++) {
    strcpy(user->username, "default");
    strcpy(user->password, "default");
    user->admin = 0;
    user++;
  }
}

void addUser1(struct user *users, char *username, char *password, int admin,
              int *count) {
  struct user *u = users;
  u = u + (*count);
  strcpy(u->username, username);
  strcpy(u->password, password);
  u->admin = admin;
  (*count)++;
}

void addUser2(struct user *users, struct user *new_user, int *count) {
  struct user *u = users;
  u = u + (*count);
  *u = *new_user;
  (*count)++;
}

void printUser(struct user *users, int number_of_users) {
  struct user *u;
  u = users;
  for (int i = 0; i < number_of_users; i++) {
    printf("username%d: %s\n", i, u->username);
    printf("password%d: %s\n", i, u->password);
    u++;
  }
}

int main(void) {
  struct user *users = createUsers(10);
  if (users == NULL) {
    return 0;
  }
  int userCount = 0;
  initUsers(users, 10);
  addUser1(users, "admin", "s#1Pa5", 1, &userCount);
  addUser1(users, "vahab", "fgH!12", 0, &userCount);

  struct user u1;
  u1.admin = 2;
  strcpy(u1.username, "usf");
  strcpy(u1.password, "gA24?m");
  addUser2(users, &u1, &userCount);
  printUser(users, userCount);
  free(users);
  return 0;
}
