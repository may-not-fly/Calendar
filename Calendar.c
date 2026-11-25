#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <stdbool.h> 
#include <string.h> 
#include <sys/stat.h> // allows use of mkdir()
#include <dirent.h> // for checking directories

// initialise all functions in order to have them in descending order with main() at the top
void startup(void) ;
void get_time(char * current_time) ;
void check_year(int current_year) ;
void make_year_folder(int year_num) ;
void make_month_file(int year, int month_num) ;
int days_in_month(int year, int month) ;
bool leap_year(int year) ;
void options(void) ;
void calendar_edit(void) ;
void calendar_view(void) ;
void day_planner(void) ;
void tasks(void) ;


// arrays which will not change (months)
const char MONTHS[][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"} ;
const int MONTH_DATES[] = {31, 27, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} ;

/// Loads initial functions
int main(void) {

    int year = 2026 ;
    leap_year(year) ;
    
    startup() ;
    options() ;
    
    printf("--process ran--\n") ; //test
	return 0 ;
}

/// Calls functions for checking whether newer calendar folders and files need to be made
void startup(void) {
    
    char current_time[25] ; // initialises string which will contain the full time and date
    get_time(current_time) ; // puts time and date into string
    printf("\ntime:%s\n", current_time) ; // tells the user the full time and date
    
    int current_year = (((current_time[20] - 48) * 1000) + ((current_time[21] - 48) * 100) + ((current_time[22] - 48) * 10) + (current_time[23] - 48)) ;
    check_year(current_year) ;
}

/// Gets system time
void get_time(char * current_time) {

    // gets the current time
    time_t laptop_time ; 
    time(&laptop_time) ; 

    // writes the input to the file that is pointed at
    FILE * file ;
    file = fopen("time.txt", "w") ; 
    fprintf(file, ctime(&laptop_time)) ; 
    fclose(file) ;

    // reads the time into the variable
	file = fopen("time.txt", "r") ;
	fgets(current_time, 25, file ) ;
    fclose(file) ;
    
    
}

/// Checks whether you have at least two years of calendar left to work with
/// and if not makes more
void check_year(int current_year) {
    
    char year[5] ;
    FILE * file ;
    file = fopen("last_folder_made.txt", "r") ;
    if (!(file)) {
		file = fopen("last_folder_made.txt", "w") ;
		fprintf(file, "0000") ;
		fclose(file) ;
        file = fopen("last_folder_made.txt", "r") ;
    }
    
    fgets(year, 5, file ) ;
    fclose(file) ;
    int last_folder_made = (((year[0] - 48) * 1000) + ((year[1] - 48) * 100) + ((year[2] - 48) * 10) + (year[3] - 48)) ;
    printf("Folders were previously up to: %d\n", last_folder_made) ;
    
    if (opendir("Years") == NULL) {
	    int folder = mkdir("Years", 0770 ) ;
    }
    
    if (last_folder_made == 0000) { 
        for (int i = 0; i < 3; i++) {
         
            int var_year = current_year + i ;
            make_year_folder(var_year) ;
            file = fopen("last_folder_made.txt", "w") ;
            fprintf(file, "%d", var_year) ;
            fclose(file) ;
        }
    }
    else if (last_folder_made < (current_year + 2)) {
        for (int i = 0; i < (current_year - last_folder_made) + 3; i++) {    // for loop makes folders until it catches up to current year + 2
           
            int var_year = last_folder_made + i ;
            make_year_folder(var_year) ;
            file = fopen("last_folder_made.txt", "w") ;
            fprintf(file, "%d",var_year) ;
            fclose(file) ;
        } 
    }
    
    file = fopen("last_folder_made.txt", "r") ;
    fgets(year, 5, file ) ;
    fclose(file) ;
    last_folder_made = (((year[0] - 48) * 1000) + ((year[1] - 48) * 100) + ((year[2] - 48) * 10) + (year[3] - 48)) ;
    printf("Folders created up to: %d\n", last_folder_made) ;
}

/// Makes a folder to contain each of the month text files
void make_year_folder(int year_num) {
	
	char year[10] ;
	sprintf(year, "Years/%d", year_num) ; 
    // checks whether to make a directory or not
    if (opendir(year) == NULL) {
	    int folder = mkdir(year, 0770 ) ;
    }
    
    // printf("got past null check %d\n", year_num) ; //test
	for (int i = 0; i < 12; i++) {
		
		make_month_file(year_num, i) ;
	}
    // printf("made all files %d\n", year_num) ; //test
}

/// Opening files not there and putting date numbers in them
void make_month_file(int year, int month_num) {
	
	char filepath[63] ;
	sprintf(filepath, "Years/%d/%s.txt", year, MONTHS[month_num]) ;
	
	if (!(fopen(filepath, "r"))) {
		FILE * file ;
		file = fopen(filepath, "w") ;
		int days = days_in_month(year, month_num) ;
		for (int i = 0; i < days; i++) {
			
			fprintf(file, "#%d.\n", i + 1) ;		
		}
		fclose(file) ;
    }
    // printf("%s\n", filepath) ; //test
}

int days_in_month(int year, int month) {
	
	int days ;
	if (month == 1) {
		
		if (leap_year(year)) {days = 28 ;}
		else {days = MONTH_DATES[month] ;}
	}
	else {days = MONTH_DATES[month] ;}
	
	return days ;
}

/// Checks whether its a leap year
bool leap_year(int year) {
    year = 4 ;
	if ((year % 4) == 0) {
        if ((year % 100) == 0) {
            if ((year % 400) == 0) {
                return true ;
            }
            return false ;
        }
        return true ;
    }
    return false ;
}


/// Options Menu
void options(void) {

	bool is_closed = false ;

    // Keeps looping through the options menu until user inputs 4(is_closed == true)
	while (is_closed == false) {

        int options_input = 0 ;
        // Asks for an input
		printf("Options:\n1. Calendar\n2. Day Planner\n3. Tasks\n4. Close\n") ;
		scanf("%i%*c", &options_input) ;
        printf("\n\n\n") ;
        if (options_input == 1) {

            int calendar_input = 0 ;
        	bool calendar_exit = false ;
            while (calendar_exit == false) {

		        printf("1. Edit Calendar\n2. View Calendar\n3. Back\n") ;
		        scanf("%i", &calendar_input) ;
		        if (calendar_input == 1) { calendar_edit() ;} //calendar edit
                else if(calendar_input == 2) { calendar_view() ;} // calendar view
                else if(calendar_input == 3) { calendar_exit = true ;} // exit
                else { printf("That was not a valid input, please try again\n") ;}
        	}
        }

		else if (options_input == 2) {

            int day_planner_input = 0 ;
	        bool day_planner_exit = false ;
            while (day_planner_exit == false) {

                printf("1. Edit Day Planner\n2. View Day Planner\n3. Back\n") ;
                scanf("%i", day_planner_input) ;
                if (day_planner_input == 1) {}
                else if (day_planner_input == 2) {}
                else if (day_planner_input == 3) { day_planner_exit = true ;}
                else { printf("That was not a valid input, please try again\n") ;}
            }
		}

		else if (options_input == 3) {
			tasks() ;
		}

		else if (options_input == 4) {

            is_closed = true ;
		}

		else {

			printf("That was not a valid input, please try again\n") ;
		}
	}
}


void calendar_edit(void) {

}

void calendar_view(void) {

}


void day_planner(void) {

}

/// Glorified To-Do-List
void tasks(void) {

}
