#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct new_user {
  char username[50];
  char password[50];
  bool write;
  bool read;
  bool execute;
};

typedef unsigned char bitmap8;

struct user {
  char username[50];
  char password[50];
  bitmap8 permissions;
};

void compare() {
  printf("The size of new_user is %d.\n", (int)sizeof(struct new_user));
  printf("The size of user is %d.\n", (int)sizeof(struct user));
}

void grantPermission(int bitIndex, struct user *user) {
  u_int8_t mask = 0b1 << bitIndex;
  user->permissions = user->permissions | mask;
}

void revokePermission(int bitIndex, struct user *user) {
  u_int8_t mask = ~(0b1 << bitIndex);
  user->permissions = user->permissions & mask;
}

int checkPermission(int bitIndex, struct user *user) {
  u_int8_t mask = 0b1 << bitIndex;
  return (user->permissions & mask) == mask;
}

void printPermissions(struct user *user) {
  if (checkPermission(0, user)) {
    printf("%s has write permission.\n", user->username);
  } else {
    printf("%s doesn't have write permission.\n", user->username);
  }
  if (checkPermission(1, user)) {
    printf("%s has read permission.\n", user->username);
  } else {
    printf("%s doesn't have read permission.\n", user->username);
  }
  if (checkPermission(2, user)) {
    printf("%s has execute permission.\n", user->username);
  } else {
    printf("%s doesn't have execute permission.\n", user->username);
  }
}

void setPermissions(int new_permissions, struct user *user) {
  u_int8_t mask_size = 3;
  for (int i = 0; i < mask_size; i++) {
    u_int8_t mask = 0b1 << i;
    if ((new_permissions & mask) == mask) {
      grantPermission(i, user);
    } else {
      revokePermission(i, user);
    }
  }
}

int main(void) {
  struct user user;
  strcpy(user.username, "admin");
  strcpy(user.password, "s#1Pa5");
  user.permissions = 0; // Sets the permissions to 0
  grantPermission(0, &user);
  grantPermission(1, &user);
  printPermissions(&user);
  revokePermission(1, &user);
  grantPermission(2, &user);
  printPermissions(&user);
  return EXIT_SUCCESS;
}
