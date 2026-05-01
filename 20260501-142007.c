#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_STATIONS 12
#define MAX_TRIPS 3

typedef enum { REGULAR = 0, DISCOUNTED = 1 } FareType;

typedef struct {
    char stationName[30];
    float baseFare; // We will use index math to calculate fare dynamically
    char southBound[MAX_TRIPS][15]; // Naga -> Sipocot
    char northBound[MAX_TRIPS][15]; // Sipocot -> Naga
} Station;

typedef struct Ticket {
    int ticketID;
    char passengerName[50];
    char fromStation[30];
    char toStation[30];
    char tripTime[15];
    float ticketPrice;
    char paymentMethod[40];
    char seatNumber[10];
    char bookingDate[20];
    struct Ticket *next;
} Ticket;

Station stations[MAX_STATIONS];
Ticket *ticketHead = NULL;
int totalTickets = 0;

void initializeSystem() {
    char *names[] = {"NAGA", "SAMPALOC", "BORABOD", "PAMPLONA", "MAMBULO", "MALANSAD", "RONGOS", "LIBMANAN", "CAMAMBUGAN", "MANTALISAY", "AWAYAN", "SIPOCOT"};
    
    // Naga to Sipocot Times
    char *sbTimes[12][3] = {
        {"4:29 AM", "9:49 AM", "3:14 PM"}, {"4:41 AM", "10:01 AM", "3:26 PM"}, {"4:47 AM", "10:07 AM", "3:32 PM"},
        {"4:53 AM", "10:13 AM", "3:38 PM"}, {"4:59 AM", "10:19 AM", "3:44 PM"}, {"5:05 AM", "10:25 AM", "3:50 PM"},
        {"5:11 AM", "10:31 AM", "3:56 PM"}, {"5:18 AM", "10:38 AM", "4:03 PM"}, {"5:24 AM", "10:44 AM", "4:09 PM"},
        {"5:30 AM", "10:50 AM", "4:15 PM"}, {"5:36 AM", "10:56 AM", "4:21 PM"}, {"5:42 AM", "11:02 AM", "4:27 PM"}
    };

    // Sipocot to Naga Times (Example Reversed/Standardized)
    char *nbTimes[12][3] = {
        {"7:40 AM", "1:00 PM", "6:15 PM"}, {"7:28 AM", "12:48 PM", "6:03 PM"}, {"7:22 AM", "12:42 PM", "5:57 PM"},
        {"7:16 AM", "12:36 PM", "5:51 PM"}, {"7:10 AM", "12:30 PM", "5:45 PM"}, {"7:04 AM", "12:24 PM", "5:39 PM"},
        {"6:58 AM", "12:18 PM", "5:33 PM"}, {"6:51 AM", "12:11 PM", "5:26 PM"}, {"6:45 AM", "12:05 PM", "5:20 PM"},
        {"6:39 AM", "11:59 AM", "5:14 PM"}, {"6:33 AM", "11:53 AM", "5:08 PM"}, {"6:25 AM", "11:45 AM", "5:00 PM"}
    };

    // Fares relative to Naga for calculation
    float fares[] = {0, 15, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60};

    for(int i = 0; i < 12; i++) {
        strcpy(stations[i].stationName, names[i]);
        stations[i].baseFare = fares[i];
        for(int j = 0; j < 3; j++) {
            strcpy(stations[i].southBound[j], sbTimes[i][j]);
            strcpy(stations[i].northBound[j], nbTimes[i][j]);
        }
    }
}

int searchStation(char *name) {
    for (int i = 0; i < 12; i++) {
        if (strcmp(stations[i].stationName, name) == 0) return i;
    }
    return -1;
}

int processPayment(float amount, char *method) {
    int choice, eChoice;
    float cash;
    char accountNum[20];
    printf("\n--- PAYMENT ---\nAmount: PHP %.2f\n1. Cash\n2. E-Wallet\nChoice: ", amount);
    if(scanf("%d", &choice) != 1) return 0;

    if (choice == 1) {
        strcpy(method, "CASH");
        printf("Amount Tendered: "); scanf("%f", &cash);
        if (cash < amount) return 0;
        printf("Change: PHP %.2f\n", cash - amount);
    } else if (choice == 2) {
        printf("1. GCash\n2. PayMaya\nChoice: "); scanf("%d", &eChoice);
        printf("Account Number: "); scanf("%s", accountNum);
        sprintf(method, "%s (%s)", (eChoice == 1 ? "GCASH" : "PAYMAYA"), accountNum);
    } else return 0;
    return 1;
}

void bookTicket(char *name, char *from, char *to, int fType, int tripNum) {
    int fIdx = searchStation(from), tIdx = searchStation(to);
    
    if (fIdx == -1 || tIdx == -1 || fIdx == tIdx) {
        printf("\n✗ Invalid Route.\n"); return;
    }

    // Dynamic Fare Calculation: Difference between station points
    float price = abs((int)stations[fIdx].baseFare - (int)stations[tIdx].baseFare);
    if (price < 15) price = 15; // Minimum fare
    if (fType == DISCOUNTED) price *= 0.8; // 20% Discount

    char method[40];
    if (!processPayment(price, method)) { printf("✗ Payment Failed.\n"); return; }

    Ticket *newTicket = (Ticket *)malloc(sizeof(Ticket));
    newTicket->ticketID = 202600 + totalTickets;
    strcpy(newTicket->passengerName, name);
    strcpy(newTicket->fromStation, from);
    strcpy(newTicket->toStation, to);
    
    // Determine Direction for Schedule
    if (fIdx < tIdx) // Southbound (Naga toward Sipocot)
        strcpy(newTicket->tripTime, stations[fIdx].southBound[tripNum-1]);
    else // Northbound (Sipocot toward Naga)
        strcpy(newTicket->tripTime, stations[fIdx].northBound[tripNum-1]);

    strcpy(newTicket->paymentMethod, method);
    newTicket->ticketPrice = price;
    sprintf(newTicket->seatNumber, "C%d-S%d", (totalTickets/60)+1, (totalTickets%60)+1);
    
    time_t now = time(NULL);
    strftime(newTicket->bookingDate, 20, "%Y-%m-%d %H:%M", localtime(&now));

    newTicket->next = ticketHead;
    ticketHead = newTicket;
    totalTickets++;
    printf("\n✓ TICKET ISSUED: %s departure from %s\n", newTicket->tripTime, newTicket->fromStation);
}

void displayTicket() {
    if (!ticketHead) { printf("\n✗ No records found.\n"); return; }
    Ticket *t = ticketHead;
    printf("\n===============================================================\n");
    printf("              PHILIPPINE NATIONAL RAILWAYS (PNR)             \n");
    printf("===============================================================\n");
    printf("  TICKET ID: %-10d DATE: %s\n", t->ticketID, t->bookingDate);
    printf("  PASSENGER: %s\n", t->passengerName);
    printf("  ROUTE    : %s TO %s\n", t->fromStation, t->toStation);
    printf("  SCHEDULE : %s\n", t->tripTime);
    printf("  FARE     : PHP %.2f\n", t->ticketPrice);
    printf("  SEAT     : %s\n", t->seatNumber);
    printf("===============================================================\n");
}

int main() {
    int choice, fType, trip;
    char name[50], from[30], to[30];
    initializeSystem();

    while (1) {
        printf("\n--- PNR NAGA-SIPOCOT SYSTEM ---\n1. View Schedules\n2. Book Ticket\n3. Show Last Ticket\n4. Exit\nChoice: ");
        if(scanf("%d", &choice) != 1) break;
        getchar();
        
        if (choice == 1) {
            printf("\n--- SOUTHBOUND (Naga -> Sipocot) ---\n%-12s | %-10s | %-10s | %-10s\n", "STATION", "TRIP 1", "TRIP 2", "TRIP 3");
            for(int i=0; i<12; i++) printf("%-12s | %-10s | %-10s | %-10s\n", stations[i].stationName, stations[i].southBound[0], stations[i].southBound[1], stations[i].southBound[2]);
            
            printf("\n--- NORTHBOUND (Sipocot -> Naga) ---\n%-12s | %-10s | %-10s | %-10s\n", "STATION", "TRIP 1", "TRIP 2", "TRIP 3");
            for(int i=11; i>=0; i--) printf("%-12s | %-10s | %-10s | %-10s\n", stations[i].stationName, stations[i].northBound[0], stations[i].northBound[1], stations[i].northBound[2]);
        } else if (choice == 2) {
            printf("Name: "); fgets(name, 50, stdin); name[strcspn(name, "\n")] = 0;
            printf("Origin: "); scanf("%s", from);
            printf("Destination: "); scanf("%s", to);
            for(int i=0; from[i]; i++) from[i]=toupper(from[i]);
            for(int i=0; to[i]; i++) to[i]=toupper(to[i]);
            printf("Trip (1-3): "); scanf("%d", &trip);
            printf("Type (0-Regular, 1-Discounted): "); scanf("%d", &fType);
            bookTicket(name, from, to, fType, trip);
        } else if (choice == 3) {
            displayTicket();
        } else if (choice == 4) {
            break;
        }
    }
    return 0;
}
    