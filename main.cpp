#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

#define TEMP_MAX_TIME 300

time_t getFileCreationTime(char *path);
void pause();

int main() {
    DIR *dir;
    char enterprises_directory[] = "##MEUPATH##\\Uninfe\\";

    struct dirent *ent;
    time_t now = time(0);

    if ((dir = opendir (enterprises_directory)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            char temp_directory[] = "\\Prod\\Envio\\Temp\\";

            if(ent->d_type == DT_DIR){

                char path_temp[strlen(enterprises_directory) + strlen(ent->d_name) + strlen(temp_directory)] = "";
                strcat(path_temp, enterprises_directory);
                strcat(path_temp, ent->d_name);
                strcat(path_temp, temp_directory);

                DIR * temp_dir;
                struct dirent *file;
                if ((temp_dir = opendir (path_temp)) != NULL) {
                    while ((file = readdir(temp_dir)) != NULL) {
                        if(file->d_type == DT_REG || file->d_type == DT_UNKNOWN) {
                            char complete_path[strlen(path_temp) + strlen(file->d_name)] = "";
                            strcat(complete_path, path_temp);
                            strcat(complete_path, file->d_name);

                            time_t tempDate = getFileCreationTime(complete_path);
                            time(&now);
                            double seconds = difftime(now, tempDate);

                            if(seconds > TEMP_MAX_TIME) {
                                char envio_directory[] = "\\Prod\\Envio\\";
                                char new_directory[strlen(enterprises_directory) + strlen(ent->d_name) + strlen(envio_directory) + strlen(file->d_name)] = "";
                                strcat(new_directory, enterprises_directory);
                                strcat(new_directory, ent->d_name);
                                strcat(new_directory, envio_directory);
                                strcat(new_directory, file->d_name);

                                if(rename(complete_path, new_directory) == -1){
                                    printf("Erro ao mover arquivo.\n\n");
                                }else{
                                    printf("Arquivo %s movido com sucesso.\n", file->d_name);
                                }
                            }
                        }
                    }
                }
                closedir (temp_dir);
            }
        }
        closedir (dir);
        pause();
    } else {
        perror ("Erro ao abrir diretório.");
        pause();
        return EXIT_FAILURE;
    }
}

time_t getFileCreationTime(char *path) {
    struct stat attrib;
    if(stat(path, &attrib) == -1){
        perror("Erro ao obter dados do arquivo");
        exit(1);
    }
    return attrib.st_mtime;
}

void pause(){
    printf("\nPressione uma tecla para continuar.");
    getchar();
}
