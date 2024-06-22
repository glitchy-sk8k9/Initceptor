#include <stdio.h>
#include "user_hash_utils.h"

#define USERNAME "noperms"
#define PASSWORD "d"

int main(){
    bool success = hash_matches_for_user(USERNAME, PASSWORD);
    if (success){
	    printf("Works!");
    } else {
	    printf("Oh boy, here we go debugging...");
    }
    printf("exit");
}
