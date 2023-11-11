#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void scan_dir(const char* root_dir, map* dir_map) {
    DIR* dir = opendir(root_dir);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    char* path;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        path = malloc(strlen(root_dir) + strlen(entry->d_name) + 2);
        strcpy(path, root_dir);
        strcat(path, "/");
        strcat(path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            scan_dir(path, dir_map);
            insert_into_map(dir_map, path);
        }

        free(path);
    }

    closedir(dir);
}

int main() {
    const char* root_dir = "/tmp";
    map dir_map = create_map();
    scan_dir(root_dir, &dir_map);

    // print the saved folder paths
    for (int i = 0; i < dir_map.size; i++) {
    
    	if (fork() == 0)
    	{
    		execve("./main",dir_map.entries[i].name,NULL);
    	}
    	
    }

    // cleanup
    for (int i = 0; i < dir_map.size; i++) {
        free(dir_map.entries[i].name);
    }
    free(dir_map.entries);

    return 0;
}
