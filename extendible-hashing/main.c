#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bucket.h"

int main(int argc, const char *argv[]) {
    char buffer[10];
    int bk_size, key;
    FILE *file = fopen("chaves.txt", "r");
    
    if (argc != 2) {
        bk_size = 10;
    } else {
        bk_size = atoi(argv[argc-1]);
    }

    init_dir(bk_size);

    while (!feof(stdin)) {
        fgets(buffer, 10, stdin);
        key = atoi(buffer);
        op_add(key);
    }

    print_dir();

#if _WIN32
    system("pause");
#endif

    return 0;
}
