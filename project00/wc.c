# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>

// makes sure that there are exactly two arguments at command line, and notifies if not by returning 1
int check_usage(int argc){
  if (argc != 2) {
      return 1;
  }
  return 0;
}

// computes the number of lines in a file and prints it along with the filename
int main(int argc, char *argv[]) {
    if(check_usage(argc) == 1){
	    exit(3);  // exit script if not right # of arguments
    }
    char *filename = argv[1];
    FILE *f = fopen(filename, "r");
    if(f == NULL){
      printf("wc: %s: No such file or directory\n", filename);
      exit(1);
    }
    fseek(f, 0L, SEEK_END); // point to end of file
    long int res = ftell(f); // size of file
    if(res == 0){  // if empty file, close and return 0 words
      printf("0 %s\n", filename);
      fclose(f);
      return 0;
    }
    fseek(f, 0L, SEEK_SET); // point to beginning of file
    int num_lines = 0;
    char *result = malloc(sizeof(char)); // stores only one char at a time
    if(result == NULL){ // make sure that result has space allocated for it
      exit(2);
    }
    while(feof(f) == 0){ // feof = 0 means not at end of file
      fread(result, 1, 1, f); //should keep replacing one char
      if(result[0] == '\n'){
        num_lines++;
      }
    }
    if(num_lines != 0){ // bc reads an extra new line at very end
      num_lines--;
    }
    printf("%d %s\n", num_lines, filename);
    free(result);
    fclose(f);
    return 0;
}
