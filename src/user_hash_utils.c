#include "../headers/user_hash_utils.h"
// Yes i did have to segment this, well, this far.

void get_salt_from_hash(const char *hash, char *salt) {
    // Find the third dollar sign
    const char *dollar = strchr(hash, '$');
    if (!dollar) {
        strcpy(salt, hash);
        return;
    }
    dollar = strchr(dollar + 1, '$'); // Skip the algorithm identifier
    if (!dollar) {
        strcpy(salt, hash);
        return;
    }
    dollar = strchr(dollar + 1, '$'); // Skip the cost factor
    if (!dollar) {
        strcpy(salt, hash);
        return;
    }
    dollar = strchr(dollar + 1, '$'); // Move to the end of the salt
    if (!dollar) {
        strcpy(salt, hash);
        return;
    }
    size_t len = dollar - hash;
    strncpy(salt, hash, len);
    salt[len] = '\0';
    
}

int get_user_hash(const char *username, char *hash) {
    FILE *fp = fopen(SHADOW_FILE, "r");
    if (!fp) {
        perror("fopen");
        return -3;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char *user = strtok(line, ":");
        if (user && strcmp(user, username) == 0) {
            char *hashed_password = strtok(NULL, ":");
            if (hashed_password) {
                strcpy(hash, hashed_password);
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);
    fprintf(stderr, "User %s not found\n", username);
    return -1;
}
    


int hash_matches_for_user(const char *username, const char *input_password) {
    char hash[256];
    char salt[256];

    int initial_check = get_user_hash(username, hash);

    if (initial_check != 0) {
        return initial_check; // Getting hash failed
    }

    get_salt_from_hash(hash, salt);

    char *computed_hash = crypt(input_password, salt);
    if (computed_hash == NULL) {
        return -2; // Hashing itself failed, we do NOT want to see that
    }

    if (strcmp(hash, computed_hash) == 0) {
        return 0; // Works :D
    } else {
        return 1; // Works but hash did not match
    }
}

