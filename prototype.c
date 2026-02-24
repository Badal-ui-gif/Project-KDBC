#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 20
#define MAX_BOOKINGS 200
#define PASSWORD "admin123"



typedef struct {
    int movieID;
    char title[50];
    int totalSeats;
    int availableSeats;
    float price;
} Movie;

typedef struct {
    int bookingID;
    char customerName[50];
    int movieID;
    int seatsBooked;
    float totalAmount;
} Booking;



Movie movies[MAX_MOVIES];
Booking bookings[MAX_BOOKINGS];

int movieCount = 0; 
int bookingCount = 0;



void mainMenu();
void adminLogin();
void adminMenu();
void addMovie();
void deleteMovie();
void viewMovies();
void bookTicket();
void cancelTicket();
void viewBookings();
void saveData();
void loadData();



int main() {
    loadData();
    mainMenu();
    saveData();
    return 0;
}



void mainMenu() {
    int choice;

    do {
        printf("\n====== FILM TICKET MANAGEMENT SYSTEM ======\n");
        printf("1. View Movies\n");
        printf("2. Book Ticket\n");
        printf("3. Cancel Ticket\n");
        printf("4. View Bookings\n");
        printf("5. Admin Panel\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: viewMovies(); break;
            case 2: bookTicket(); break;
            case 3: cancelTicket(); break;
            case 4: viewBookings(); break;
            case 5: adminLogin(); break;
            case 0: printf("Saving data & exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while(choice != 0);
}



void adminLogin() {
    char pass[20];
    printf("Enter Admin Password: ");
    scanf("%s", pass);

    if(strcmp(pass, PASSWORD) == 0) {
        adminMenu();
    } else {
        printf("Incorrect password!\n");
    }
}



void adminMenu() {
    int choice;

    do {
        printf("\n------ ADMIN PANEL ------\n");
        printf("1. Add Movie\n");
        printf("2. Delete Movie\n");
        printf("3. View Movies\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addMovie(); break;
            case 2: deleteMovie(); break;
            case 3: viewMovies(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }

    } while(choice != 0);
}


void addMovie() {
    if(movieCount >= MAX_MOVIES) {
        printf("Movie limit reached!\n");
        return;
    }

    movies[movieCount].movieID = movieCount + 1;

    printf("Enter movie title: ");
    scanf(" %[^\n]", movies[movieCount].title);

    printf("Enter total seats: ");
    scanf("%d", &movies[movieCount].totalSeats);

    movies[movieCount].availableSeats = movies[movieCount].totalSeats;

    printf("Enter ticket price: ");
    scanf("%f", &movies[movieCount].price);

    movieCount++;
    printf("Movie added successfully!\n");
}



void deleteMovie() {
    int id, found = 0;

    printf("Enter Movie ID to delete: ");
    scanf("%d", &id);

    for(int i = 0; i < movieCount; i++) {
        if(movies[i].movieID == id) {
            for(int j = i; j < movieCount - 1; j++) {
                movies[j] = movies[j + 1];
            }
            movieCount--;
            found = 1;
            printf("Movie deleted successfully!\n");
            break;
        }
    }

    if(!found)
        printf("Movie not found!\n");
}



void viewMovies() {
    if(movieCount == 0) {
        printf("No movies available.\n");
        return;
    }

    printf("\n------ AVAILABLE MOVIES ------\n");
    for(int i = 0; i < movieCount; i++) {
        printf("ID: %d | Title: %s | Available Seats: %d | Price: %.2f\n",
               movies[i].movieID,
               movies[i].title,
               movies[i].availableSeats,
               movies[i].price);
    }
}



void bookTicket() {
    int movieID, seats;

    if(movieCount == 0) {
        printf("No movies available!\n");
        return;
    }

    viewMovies();

    printf("Enter Movie ID: ");
    scanf("%d", &movieID);

    if(movieID < 1 || movieID > movieCount) {
        printf("Invalid Movie ID!\n");
        return;
    }

    printf("Enter your name: ");
    scanf(" %[^\n]", bookings[bookingCount].customerName);

    printf("Enter number of seats: ");
    scanf("%d", &seats);

    if(seats > movies[movieID - 1].availableSeats) {
        printf("Not enough seats available!\n");
        return;
    }

    bookings[bookingCount].bookingID = bookingCount + 1;
    bookings[bookingCount].movieID = movieID;
    bookings[bookingCount].seatsBooked = seats;
    bookings[bookingCount].totalAmount = seats * movies[movieID - 1].price;

    movies[movieID - 1].availableSeats -= seats;

    printf("Booking Successful!\n");
    printf("Booking ID: %d\n", bookings[bookingCount].bookingID);
    printf("Total Amount: %.2f\n", bookings[bookingCount].totalAmount);

    bookingCount++;
}



void cancelTicket() {
    int bookingID;
    printf("Enter Booking ID to cancel: ");
    scanf("%d", &bookingID);

    for(int i = 0; i < bookingCount; i++) {
        if(bookings[i].bookingID == bookingID) {

            movies[bookings[i].movieID - 1].availableSeats += bookings[i].seatsBooked;

            for(int j = i; j < bookingCount - 1; j++) {
                bookings[j] = bookings[j + 1];
            }

            bookingCount--;
            printf("Booking cancelled successfully!\n");
            return;
        }
    }

    printf("Booking ID not found!\n");
}



void viewBookings() {
    if(bookingCount == 0) {
        printf("No bookings found.\n");
        return;
    }

    printf("\n------ ALL BOOKINGS ------\n");
    for(int i = 0; i < bookingCount; i++) {
        printf("Booking ID: %d | Name: %s | Movie ID: %d | Seats: %d | Total: %.2f\n",
               bookings[i].bookingID,
               bookings[i].customerName,
               bookings[i].movieID,
               bookings[i].seatsBooked,
               bookings[i].totalAmount);
    }
}



void saveData() {
    FILE *fm = fopen("movies.dat", "wb");
    fwrite(&movieCount, sizeof(int), 1, fm);
    fwrite(movies, sizeof(Movie), movieCount, fm);
    fclose(fm);

    FILE *fb = fopen("bookings.dat", "wb");
    fwrite(&bookingCount, sizeof(int), 1, fb);
    fwrite(bookings, sizeof(Booking), bookingCount, fb);
    fclose(fb);
}



void loadData() {
    FILE *fm = fopen("movies.dat", "rb");
    if(fm != NULL) {
        fread(&movieCount, sizeof(int), 1, fm);
        fread(movies, sizeof(Movie), movieCount, fm);
        fclose(fm);
    }

    FILE *fb = fopen("bookings.dat", "rb");
    if(fb != NULL) {
        fread(&bookingCount, sizeof(int), 1, fb);
        fread(bookings, sizeof(Booking), bookingCount, fb);
        fclose(fb);
    }
}