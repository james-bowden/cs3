# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>

// makes sure that exactly 2 arguments passed in, returns 1 if not
int check_usage(int argc){
  if (argc != 2) {
      return 1;
  }
  return 0;
}

// adds a null terminator to str at index ind
void add_null_term(char *str, int ind){
  str[ind] = '\0';
}

// computes longest word in file and prints it
int main(int argc, char *argv[]) {
    if(check_usage(argc) == 1){ // exit if wrong # arguments
	    exit(3);
    }
    char *filename = argv[1];
    FILE *f = fopen(filename, "r");
    if(f == NULL){ // exits with error msg if file doesn't exist
      printf("wc: %s: No such file or directory\n", filename);
      exit(1);
    }
    fseek(f, 0L, SEEK_END); // point to end
    long int res = ftell(f); // size of file
    if(res == 0){ // size is 0 => no words => print nothing and return
      fclose(f);
      return 0;
    }
    fseek(f, 0L, SEEK_SET); // point to beginning
    char *curr_res = malloc(102 * sizeof(char));
    char *temp_res = malloc(102 * sizeof(char));
    if(curr_res == NULL || temp_res == NULL){ // make sure that space actually malloced
      exit(2);
    }
    add_null_term(curr_res, 0);
    add_null_term(temp_res, 0);
    int ind = 0;
    char *temp = malloc(2*sizeof(char)); // to store each char read in until concatenated
    add_null_term(temp, 0);
    while(feof(f) == 0){ // while not at end of file
      fread(temp, 1, 1, f); //should keep replacing one char
      add_null_term(temp, 1);
      ind++;
      if(temp[0] == '\n' || temp[0] == ' '){ // compare and reset
        add_null_term(temp_res, ind);
        if(strlen(temp_res) > strlen(curr_res)){
          strcpy(curr_res, temp_res); // copies temp onto curr
          add_null_term(curr_res, ind);
        }
        memset(temp_res, 0, strlen(temp_res));
        add_null_term(temp_res, 0);
        // temp is reset to empty string
        ind = 0;
      }
      else{
        strcat(temp_res, temp); // adds temp to end of temp_res
        add_null_term(temp_res, ind);
      }
    }
    free(temp);
    free(temp_res);
    printf("%s\n", curr_res);
    free(curr_res);
    fclose(f);
    return 0;
}
