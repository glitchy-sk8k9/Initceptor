#ifndef HASH_UTILS_H
#define HASH_UTILS_H

#include <stdio.h>
#include <stdbool.h>

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#define SHADOW_FILE "/etc/shadow"

// Function to extract the salt from a hashed password
void get_salt_from_hash(const char *hash, char *salt);

// Function to read the hashed password for a user from /etc/shadow
int get_user_hash(const char *username, char *hash);

// Function to verify if the input password matches the hashed password for a user
int hash_matches_for_user(const char *username, const char *input_password);

#endif

