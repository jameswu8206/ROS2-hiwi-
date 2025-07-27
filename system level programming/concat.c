#include<stdio.h>
#include<stdlib.h>

//function for calculate string length
int str_len(const char s[]){
    int n=0;
    while(s[n]!='\0'){
        n++;
    }
    return n;
}
//function for copy string
void str_cpy(char s[],const char t[]){
    int i=0;
    while((s[i]=t[i])!='\0'){
        i++;
    }
}
//function for concatenate string
void str_cat(char s[],const char t[]){
    int i=0;
    while(s[i]!='\0'){
        i++;
    }
    int j=0;
    while((s[i]=t[j])!='\0'){
        i++;
        j++;
    }
}
//function for allocating space for string
char *charArr(int n){
    return (char *)malloc(n* sizeof (char) );
}

int main(int argc,char *argv[]){
    //check of there's input error
    if(ferror(stdin)){
        printf("error input");
        exit(EXIT_FAILURE);
    }
    //check if there's no input
    if(argc==1){
        printf("no input given\n");
        exit(EXIT_FAILURE);
    }
    //iterate through input and calculate the string length
    int size=0;
    for(int i=1;i<argc;i++){
        size+=str_len(argv[i]);
    }
    //allocate required space
    char *array=charArr(size+1);
    if(NULL==array){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    //copy string to allocated space
    str_cpy(array,argv[1]);
    for(int i=2;i<argc;i++){
        str_cat(array," ");
        str_cat(array,argv[i]);
    }
    //print the string
    printf(array);
    printf("\n");
    //free the space after use
    free(array);
    return EXIT_SUCCESS;
}