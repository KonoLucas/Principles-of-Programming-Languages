#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Function Declarations
void registerUser();
void displayMovies();
void rateMovie();
void getMovieRecommendations();

struct User{  int id; char name[20];  };
typedef struct{  char name[100];  char type[20]; double rate; int index;}Movie;

// Function to check if a file exists
int checkFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) { fclose(file);  return 1; } 
    else { return 0;}
}

// Function to get the next ID by reading the last user's ID from the file
int getNextId(FILE *file) {
    int lastId = 0; 
    int idnumber;
    char line[100]; 
    
    // Read the file line by line to find the last ID
    while ( !feof(file) ) { printf("*"); fgets(line,100,file); idnumber=idnumber+1; }

    // The next ID will be the last ID found, plus one
    return lastId=(idnumber + 1);
}

int userExists(const char *username) {
    struct User user;
    FILE *namefile = fopen("user_data.txt","r");
    
    if (!namefile) { return 0; }
    else { 
        while (fscanf(namefile, "%s %d", user.name, &user.id) != EOF)  
          {  if (strcmp(user.name, username) == 0) {return 1; } }
        }
    return 0; 

}

int return_user_id(const char *username) {
    struct User user;
    FILE *namefile = fopen("user_data.txt","r");
    
    if (!namefile) { return 0; }
    else { 
        while (fscanf(namefile, "%s %d", user.name, &user.id) != EOF)  
          {  if (strcmp(user.name, username) == 0) {return user.id; } }
        }
    return 0; 

}

double* create_matrix_pointer(const char* file_name){
    FILE* file = fopen(file_name, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL; 
    }
    int rows;
    int cols;
    fscanf(file, "%d %d",&rows,&cols);
    fclose(file);
    return (double*) calloc(rows * cols, sizeof(double));
}

void fill_matrix_from_file(double* matrix, const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        printf("Failed to open file");
        return;
    }
    
    int rows;
    int cols;
    fscanf(file, "%d %d",&rows,&cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%lf", &matrix[i * cols + j]); 
            //printf(" socre %.1lf",matrix[i * cols + j]);
  
        }//printf("\n");
    }
    fclose(file);
}


void modify_element(double* matrix, int cols, int input_rows, int inut_cols, double new_value) {
    matrix[input_rows * cols + inut_cols] = new_value;
}

int get_rows(const char* file_name){
int rows, cols;
    FILE* file = fopen(file_name, "r");
    if (!file) {
        printf("Failed to open file");
    }

    if (fscanf(file, "%d %d", &rows, &cols) != 2) {
        printf("Failed to read rows and columns");
    }

    fclose(file);
    return(rows);    
}

int get_cols(const char* file_name){
int rows, cols;
    FILE* file = fopen(file_name, "r");
    if (!file) {
        printf("Failed to open file");
         return -1;
    }

    if (fscanf(file, "%d %d", &rows, &cols) != 2) {
        printf("Failed to read rows and columns");
    }

    fclose(file);
    return(cols);    
}

float get_movie_rate(int movie_index,int rows,int cols,double* matrix){
    float average_rate=0;
    float sum_rate=0;
    int num_valid_rate=0;
    float result;
    for(int i=0; i<rows;i++){
        double rate = matrix[i * cols + (movie_index - 1)];
        if(rate>0.001){
            sum_rate += rate;
            num_valid_rate++;
         }
    } 
    if(num_valid_rate==0){return(average_rate);}
    else{average_rate=sum_rate/num_valid_rate;}
    //printf("The sum is : %lf  The num is: %d", sum_rate, num_valid_rate);
    return (average_rate);
}

void save_matrix_to_file(const char* file_name, int rows, int cols, double* matrix) {
    FILE* file = fopen(file_name, "w"); 
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    fprintf(file, "%d %d\n", rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
           
            fprintf(file, "%.1lf ", matrix[i * cols + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file); 
}

void print_specific_line(const char* file_name, int line_number){
    char buffer[200];
    int current_line=0;
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return;
    }
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        current_line++;
        if (current_line == line_number) {
            printf("%s", buffer); 
            fclose(fp);
            return;
        }
    }
    printf("Line did't found.");
    fclose(fp);
}



int main() {
    int choice;
    Movie movielist[50];

    // Welcome message and main menu
    do { 
        printf("***** Movie Recommendation System *****\n");
        printf("1. Register User\n");
        printf("2. Display Movies\n");
        printf("3. Rate a Movie\n");
        printf("4. Get Movie Recommendations\n");
        printf("0. Exit\n");
        printf("**************************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
    
    switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                displayMovies(movielist);
                break;
            case 3:
                rateMovie();
                break;
            case 4:
                getMovieRecommendations();
                break;
            case 0:
                printf("Exiting the system.\n");
                return 0;
                //since each funtion include it's own fclose(), and free(), there have nothing to Deallocation;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);return 0;

    
            }

void registerUser() {
    printf("\n \n****New user register page****\n");
    
    do{  
       printf("Enter username for registration:");
       char username[20];
       scanf("%s",username);
       const char* file_name = "user_ratings.txt";
       int new_id= 1 + get_rows(file_name);
       int new_rows = new_id;
       int cols= get_cols(file_name);
    
       if (userExists(username)) {printf("User already exists. Please choose a different name.\n");} 
        else { 


              FILE *fp2 = fopen("user_data.txt", "a");
              if (fp2) {
                fseek(fp2, 0, SEEK_END);
                fprintf(fp2, "\n%s %d", username, new_id);
                fclose(fp2);

                double* const matrix = create_matrix_pointer(file_name);
                fill_matrix_from_file(matrix, file_name);//load the rating data;
                save_matrix_to_file(file_name,new_rows,cols,matrix);//rewrite the file with new number of rows
                free(matrix);

                printf("User %s is successfully registered.\n \n", username);
                break; 
                } 
            
               else { printf("Error opening user data file.\n");   exit(1);} 
        }
    }while (1); 

}
   

 void displayMovies(Movie list[]) {
    // movie list display function
    printf("\n****Movie Database****\n");
    FILE *movie_file = fopen("movie_database.txt", "r");

    int index=0;
    if (!movie_file) {printf("File not exsit. \n");}
    else {
         printf("The movie list: \n");
         while ( !feof(movie_file) )
               {  Movie movie;
                  fscanf(movie_file, "%s %s %lf", movie.name, movie.type,&movie.rate);
                  list[index];
                  index=index+1;
                  printf("%d. %s %s %.2lf\n",index,movie.name,movie.type,movie.rate);
                  
               }  
        }
    
    fclose(movie_file);
    printf("The end of the list. \n \n");        
}

int displayMovies_count(){
FILE *movie_file = fopen("movie_database.txt", "r");
int index=0;
 if (!movie_file) {printf("File not exsit. \n");}
    else {
         while ( !feof(movie_file) )
               {  Movie movie;
                  fscanf(movie_file, "%s %s %lf", movie.name, movie.type,&movie.rate);
                  index=index+1;
                  printf("%d. %s %s %.2lf\n",index,movie.name,movie.type,movie.rate);
                  
               }  
        }
    
    fclose(movie_file);
    printf("The end of the list. \n \n");
    return index;            
}
    
    

void rateMovie() {
    
    printf("\n \nRate a movie, please enter your user name first(case sensitive):");
    const char* file_name = "user_ratings.txt";
    int rows, cols;
    rows=get_rows(file_name);
    cols=get_cols(file_name);

    double* const matrix = create_matrix_pointer(file_name);
    fill_matrix_from_file(matrix, file_name);
    int movice_choice;
    double rate;
    char username[20];
    scanf("%s",username);
    if (userExists(username))
    {
     printf("User found! \n");
     int userid=return_user_id(username);

     int range=displayMovies_count();
     printf("Choose a movie:");
     do{scanf("%d",&movice_choice);
        if (movice_choice>range||movice_choice<1)
        {printf("Invaild input! Please choose between 1-%d :",range);}

       }while(movice_choice>range||movice_choice<1);

     printf("Enter your rate(1-5): ");
     do{scanf("%lf",&rate);
        if (rate>5.0||rate<1.0)
        {printf("Invaild input! Please enter valid rate: ");}

       }while(rate>5.0||rate<1.0);

    int input_rows= userid-1;
    int inut_cols= movice_choice-1;
    modify_element(matrix,cols,input_rows,inut_cols,rate);
    printf("Operation success. \n \n");

    // for(int i = 0; i < rows; i++) {
    // for(int j = 0; j < cols; j++) {
    //     printf("%lf ", matrix[i * cols + j]);
    // }
    // printf("\n"); }

    save_matrix_to_file(file_name, rows, cols, matrix);
    free(matrix);

    }
    if (!userExists(username)) {printf("User not found, please register first.\n \n");}
}

void getMovieRecommendations() {
    
    char username[20];
    int  userid, rows, cols;
    int number_of_unwatch_movie=0;
    int movie_id_list[50];
    const char* file_name = "user_ratings.txt";
    const char* file_name2="movie_database.txt";
    printf("\nGet movie recommendations.\n");
    printf("Enter your user name first(case sensitive):");
    
    scanf("%s",username);
    if (userExists(username)){
        int userid=return_user_id(username);
        rows=get_rows(file_name);
        cols=get_cols(file_name);
       
        double* const matrix = create_matrix_pointer(file_name);
        fill_matrix_from_file(matrix, file_name);

        printf("Unwated movie id: ");
        for(int i=0; i<cols;i++){
        float temp=matrix[(userid-1) * cols +i];
         if(temp<0.001){
            //printf("%lf   ",matrix[(userid-1) * cols +i]);
            movie_id_list[number_of_unwatch_movie++]=(i+1);
            printf(" %d ",i+1);
         }
       } 
    
       if(number_of_unwatch_movie==0){printf("You watch all the movie!\n"); EXIT_SUCCESS;}
       
       int recommend_movie_id = movie_id_list[0];
       float highest_movie_rate = get_movie_rate(movie_id_list[0], rows, cols, matrix);
       float temp;
       for(int x = 1; x < number_of_unwatch_movie; x++) {
         temp = get_movie_rate(movie_id_list[x], rows, cols, matrix);
         if(temp > highest_movie_rate) {
            highest_movie_rate = temp;
            recommend_movie_id = movie_id_list[x]; // change to the higest movie id and score;
            }
       }
       printf("\nRecommended movie is (");
       print_specific_line(file_name2,recommend_movie_id);
       printf(") with user rate %.2lf\n", highest_movie_rate);
       free(matrix);

    }

    if (!userExists(username)) {printf("User not found, please register first.\n \n");}
}
