#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

#define MAX_STATIONS 12
#define MAX_TRIPS 3
#define MAX_QUEUE 100
#define MAX_STACK 100
#define FILE_NAME "tickets.dat"

typedef enum { REGULAR = 0, STUDENT = 1, PWD = 2, SENIOR = 3 } FareType;

//ARRAY
typedef struct {
    char stationName[30];
    float baseFare;
    char southBound[MAX_TRIPS][15];
    char northBound[MAX_TRIPS][15];
} Station;

//LINKED LIST
typedef struct Ticket {
    int ticketID;
    char passengerName[50];
    char fromStation[30];
    char toStation[30];
    char tripTime[15];
    float ticketPrice;
    char paymentMethod[40];
    char issueDate[20];
    char status[20];
    struct Ticket *next;
} Ticket;

//QUEUE
typedef struct {
    int ticketID;
    char passengerName[50];
    char action[30];
} QueueNode;

typedef struct {
    QueueNode items[MAX_QUEUE];
    int front, rear;
} Queue;

//STACK
typedef struct {
    int ticketID;
    char previousStatus[20];
    char action[30];
    time_t timestamp;
} StackNode;

typedef struct {
    StackNode items[MAX_STACK];
    int top;
} Stack;

Station stations[MAX_STATIONS];
Ticket *ticketHead = NULL;
int totalTickets = 0;
Queue operationQueue;
Stack operationHistory;

void filterStatus(char status[]);
void initializeQueue(Queue *q);
void enqueue(Queue *q, int id, char *name, char *action);
void displayQueue(Queue *q);
void initializeStack(Stack *s);
void push(Stack *s, int id, char *status, char *action);
void displayStack(Stack *s);
void bubbleSortTickets(Ticket **head);
void displaySortedHistory();

//QUEUE OPERATIONS
void initializeQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
}

void enqueue(Queue *q, int id, char *name, char *action) {
    if (q->rear >= MAX_QUEUE - 1) return;
    q->rear++;
    q->items[q->rear].ticketID = id;
    strcpy(q->items[q->rear].passengerName, name);
    strcpy(q->items[q->rear].action, action);
}

void displayQueue(Queue *q) {
    if (q->front > q->rear) {
        printf("Queue is empty.\n");
        return;
    }
    printf("\n==================== OPERATION QUEUE ====================\n");
    for (int i = q->front; i <= q->rear; i++)
        printf("ID: %d | Name: %s | Action: %s\n", q->items[i].ticketID, q->items[i].passengerName, q->items[i].action);
    printf("=========================================================\n");
}

//STACK OPERATIONS
void initializeStack(Stack *s) {
    s->top = -1;
}

void push(Stack *s, int id, char *status, char *action) {
    if (s->top >= MAX_STACK - 1) return;
    s->top++;
    s->items[s->top].ticketID = id;
    strcpy(s->items[s->top].previousStatus, status);
    strcpy(s->items[s->top].action, action);
    s->items[s->top].timestamp = time(NULL);
}

void displayStack(Stack *s) {
    if (s->top < 0) {
        printf("Stack is empty.\n");
        return;
    }
    printf("\n=============== OPERATION HISTORY (UNDO) ===============\n");
    for (int i = s->top; i >= 0; i--)
        printf("ID: %d | Status: %s | Action: %s\n", s->items[i].ticketID, s->items[i].previousStatus, s->items[i].action);
    printf("========================================================\n");
}

//BUBBLE SORT
void bubbleSortTickets(Ticket **head) {
    if (*head == NULL || (*head)->next == NULL) return;
    Ticket *current, *nextNode;
    int swapped;
    Ticket temp;
    
    do {
        swapped = 0;
        current = *head;
        while (current->next != NULL) {
            nextNode = current->next;
            if (current->ticketID > nextNode->ticketID) {
                temp = *current;
                current->ticketID = nextNode->ticketID;
                strcpy(current->passengerName, nextNode->passengerName);
                strcpy(current->fromStation, nextNode->fromStation);
                strcpy(current->toStation, nextNode->toStation);
                strcpy(current->tripTime, nextNode->tripTime);
                strcpy(current->paymentMethod, nextNode->paymentMethod);
                strcpy(current->issueDate, nextNode->issueDate);
                strcpy(current->status, nextNode->status);
                current->ticketPrice = nextNode->ticketPrice;
                
                nextNode->ticketID = temp.ticketID;
                strcpy(nextNode->passengerName, temp.passengerName);
                strcpy(nextNode->fromStation, temp.fromStation);
                strcpy(nextNode->toStation, temp.toStation);
                strcpy(nextNode->tripTime, temp.tripTime);
                strcpy(nextNode->paymentMethod, temp.paymentMethod);
                strcpy(nextNode->issueDate, temp.issueDate);
                strcpy(nextNode->status, temp.status);
                nextNode->ticketPrice = temp.ticketPrice;
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}

void displaySortedHistory() {
    if (!ticketHead) {
        printf("\nNo tickets found.\n");
        return;
    }
    printf("\n========== SORTED HISTORY (BY TICKET ID) ==========\n");
    Ticket *t = ticketHead;
    while (t) {
        printf("ID: %-10d | %s -> %s | PHP %.2f | %s\n", t->ticketID, t->fromStation, t->toStation, t->ticketPrice, t->status);
        t = t->next;
    }
}

//FILE HANDLING
void saveToFile() {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) return;
    Ticket *temp = ticketHead;
    while (temp != NULL) {
        fwrite(temp, sizeof(Ticket), 1, fp);
        temp = temp->next;
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return;
    Ticket *temp;
    while (1) {
        temp = (Ticket*)malloc(sizeof(Ticket));
        if (fread(temp, sizeof(Ticket), 1, fp) != 1) {
            free(temp);
            break;
        }
        temp->next = ticketHead;
        ticketHead = temp;
        totalTickets++;
    }
    fclose(fp);
}

//INITIALIZE SYSTEM
void initializeSystem() {
    char *names[12] = {"NAGA", "SAMPALOC", "BORABOD", "PAMPLONA", "MAMBULO", "MALANSAD", "RONGOS", "LIBMANAN", "CAMAMBUGAN", "MANTALISAY", "AWAYAN", "SIPOCOT"};
    char *sbTimes[12][3] = {
        {"4:29 AM", "9:49 AM", "3:14 PM"}, {"4:41 AM", "10:01 AM", "3:26 PM"},
        {"4:47 AM", "10:07 AM", "3:32 PM"}, {"4:53 AM", "10:13 AM", "3:38 PM"},
        {"4:59 AM", "10:19 AM", "3:44 PM"}, {"5:05 AM", "10:25 AM", "3:50 PM"},
        {"5:11 AM", "10:31 AM", "3:56 PM"}, {"5:18 AM", "10:38 AM", "4:03 PM"},
        {"5:24 AM", "10:44 AM", "4:09 PM"}, {"5:30 AM", "10:50 AM", "4:15 PM"},
        {"5:36 AM", "10:56 AM", "4:21 PM"}, {"5:42 AM", "11:02 AM", "4:27 PM"}};
    char *nbTimes[12][3] = {
        {"7:40 AM", "1:00 PM", "6:15 PM"}, {"7:28 AM", "12:48 PM", "6:03 PM"},
        {"7:22 AM", "12:42 PM", "5:57 PM"}, {"7:16 AM", "12:36 PM", "5:51 PM"},
        {"7:10 AM", "12:30 PM", "5:45 PM"}, {"7:04 AM", "12:24 PM", "5:39 PM"},
        {"6:58 AM", "12:18 PM", "5:33 PM"}, {"6:51 AM", "12:11 PM", "5:26 PM"},
        {"6:45 AM", "12:05 PM", "5:20 PM"}, {"6:39 AM", "11:59 AM", "5:14 PM"},
        {"6:33 AM", "11:53 AM", "5:08 PM"}, {"6:25 AM", "11:45 AM", "5:00 PM"}};
    float fares[] = {0, 15, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60};
    
    initializeQueue(&operationQueue);
    initializeStack(&operationHistory);
    
    for (int i = 0; i < 12; i++) {
        strcpy(stations[i].stationName, names[i]);
        stations[i].baseFare = fares[i];
        for (int j = 0; j < 3; j++) {
            strcpy(stations[i].southBound[j], sbTimes[i][j]);
            strcpy(stations[i].northBound[j], nbTimes[i][j]);
        }
    }
}

//LINEAR SEARCH
int searchStation(char *name) {
    for (int i = 0; i < 12; i++)
        if (strcmp(stations[i].stationName, name) == 0) return i;
    return -1;
}

void inspectTicket() {
    int id;
    Ticket *temp = ticketHead;
    int found = 0;
    printf("\n---------- INSPECTION OF TICKET ----------\n");
    printf("Enter Ticket ID: ");
    scanf("%d", &id);
    getchar();
    if (ticketHead == NULL) {
        printf("No tickets available.\n");
        return;
    }
    while (temp) {
        if (temp->ticketID == id) {
            printf("\n============================================================\n");
            printf("                 TICKET INSPECTION DETAILS                 \n");
            printf("============================================================\n");
            printf("ID: %d | Name: %s | From: %s | To: %s\n", temp->ticketID, temp->passengerName, temp->fromStation, temp->toStation);
            printf("Time: %s | Fare: PHP %.2f | Status: %s\n", temp->tripTime, temp->ticketPrice, temp->status);
            printf("============================================================\n");
            found = 1;
            break;
        }
        temp = temp->next;
    }
    if (!found) printf("\nNo matching ticket found.\n");
}

//PAYMENT
int processPayment(float amount, char *method) {
    int choice, type;
    float cash;
    char account[20];
    printf("\n--- PAYMENT ---\nAmount: PHP %.2f\n", amount);
    printf("1. Cash\n2. E-Wallet\nChoice: ");
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        strcpy(method, "CASH");
        printf("Cash: ");
        scanf("%f", &cash);
        getchar();
        if (cash < amount) return 0;
        printf("Change: PHP %.2f\n", cash - amount);
        printf("\nTicket Issued Successfully!\nTicket is Printing");
        for (int k = 0; k < 3; k++) {
            printf(".");
            fflush(stdout);
            Sleep(500);
        }
        printf("\nTicket Printed Successfully!\n");
    } else if (choice == 2) {
        printf("1. GCash\n2. PayMaya\nChoice: ");
        scanf("%d", &type);
        getchar();
        printf("Account: ");
        fgets(account, 20, stdin);
        account[strcspn(account, "\n")] = 0;
        sprintf(method, "%s (%s)", (type == 1 ? "GCASH" : "PAYMAYA"), account);
        printf("\nTicket Issued Successfully!\nTicket is Printing");
        for (int k = 0; k < 3; k++) {
            printf(".");
            fflush(stdout);
            Sleep(500);
        }
        printf("\nTicket Printed Successfully!\n");
    } else {
        printf("\nInvalid choice.\n");
        return 0;
    }
    return 1;
}

//ISSUE TICKET
void issueTicket() {
    int fIdx, tIdx, type, trip, hasID;
    char name[50], from[30], to[30];
    float price;
    char method[40];
    printf("\nName: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;
    printf("Origin: ");
    scanf("%s", from);
    getchar();
    printf("Destination: ");
    scanf("%s", to);
    getchar();
    for (int i = 0; from[i]; i++) from[i] = toupper(from[i]);
    for (int i = 0; to[i]; i++) to[i] = toupper(to[i]);
    fIdx = searchStation(from);
    tIdx = searchStation(to);
    if (fIdx == -1 || tIdx == -1 || fIdx == tIdx) {
        printf("\nInvalid route!\n");
        return;
    }
    printf("Trip (1-3): ");
    scanf("%d", &trip);
    getchar();
    printf("Type (0-Regular, 1-Student, 2-PWD, 3-Senior): ");
    scanf("%d", &type);
    getchar();
    if (type == STUDENT || type == PWD || type == SENIOR) {
        printf("\nDoes passenger have valid ID? (1-Yes / 0-No): ");
        scanf("%d", &hasID);
        getchar();
        printf("\nVerifying");
        for (int i = 0; i < 5; i++) {
            printf(".");
            fflush(stdout);
            Sleep(400);
        }
        if (hasID == 1) printf("\nVerification Successful!\n");
        else {
            printf("\nVerification Failed!\nSwitching to REGULAR fare.\n");
            type = REGULAR;
        }
    }
    price = (float)abs((int)stations[fIdx].baseFare - (int)stations[tIdx].baseFare);
    if (price < 15) price = 15;
    if (type == STUDENT) price *= 0.8f;
    else if (type == PWD || type == SENIOR) price *= 0.7f;
    if (!processPayment(price, method)) {
        printf("Payment failed!\n");
        return;
    }
    Ticket *newTicket = (Ticket *)malloc(sizeof(Ticket));
    if (newTicket == NULL) return;
    newTicket->ticketID = 202600 + totalTickets;
    strcpy(newTicket->passengerName, name);
    strcpy(newTicket->fromStation, from);
    strcpy(newTicket->toStation, to);
    if (fIdx < tIdx) strcpy(newTicket->tripTime, stations[fIdx].southBound[trip - 1]);
    else strcpy(newTicket->tripTime, stations[fIdx].northBound[trip - 1]);
    strcpy(newTicket->paymentMethod, method);
    strcpy(newTicket->status, "Confirmed");
    newTicket->ticketPrice = price;
    time_t now = time(NULL);
    strftime(newTicket->issueDate, 20, "%Y-%m-%d %H:%M", localtime(&now));
    newTicket->next = ticketHead;
    ticketHead = newTicket;
    enqueue(&operationQueue, newTicket->ticketID, name, "TICKET_ISSUED");
    push(&operationHistory, newTicket->ticketID, "ISSUED", "TICKET_ISSUED");
    totalTickets++;
    saveToFile();
}

//HISTORY
void showHistory() {
    int choice;
    while (1) {
        if (!ticketHead) printf("\nNo tickets found.\n");
        else {
            Ticket *last = ticketHead;
            printf("\n============================================================\n");
            printf("         PHILIPPINE NATIONAL RAILWAYS (PNR)\n");
            printf("                   PASSENGER COPY\n");
            printf("============================================================\n");
            printf("ID: %d | Date: %s | Name: %s\n", last->ticketID, last->issueDate, last->passengerName);
            printf("Route: %s TO %s | Schedule: %s | Fare: PHP %.2f\n", last->fromStation, last->toStation, last->tripTime, last->ticketPrice);
            printf("============================================================\n");
            printf("\n=================== FULL HISTORY ===================\n");
            Ticket *t = ticketHead;
            while (t) {
                printf("ID: %-10d | %s -> %s | PHP %.2f | %s\n", t->ticketID, t->fromStation, t->toStation, t->ticketPrice, t->status);
                t = t->next;
            }
        }
        printf("\n------ OPTIONS ------\n");
        printf("1. View Confirmed  2. View Cancelled  3. View Queue\n");
        printf("4. View History    5. View Sorted     6. Reset\n");
        printf("7. Back to Main\nChoice: ");
        scanf("%d", &choice);
        getchar();
        if (choice == 1) filterStatus("Confirmed");
        else if (choice == 2) filterStatus("Cancelled");
        else if (choice == 3) displayQueue(&operationQueue);
        else if (choice == 4) displayStack(&operationHistory);
        else if (choice == 5) { bubbleSortTickets(&ticketHead); displaySortedHistory(); }
        else if (choice == 6) {
            if (ticketHead == NULL) { printf("\nNo records to delete.\n"); continue; }
            char confirm[10];
            printf("\nDelete all? (YES/NO): ");
            fgets(confirm, 10, stdin);
            confirm[strcspn(confirm, "\n")] = 0;
            if (strcmp(confirm, "YES") == 0) {
                Ticket *current = ticketHead;
                while (current != NULL) {
                    Ticket *temp = current;
                    current = current->next;
                    free(temp);
                }
                ticketHead = NULL;
                totalTickets = 0;
                initializeQueue(&operationQueue);
                initializeStack(&operationHistory);
                saveToFile();
                printf("All records deleted.\n");
            }
        }
        else if (choice == 7) break;
        else printf("Invalid choice.\n");
    }
}

//CANCEL TICKET
void cancelTicket() {
    int id;
    Ticket *temp = ticketHead;
    if (temp == NULL) {
        printf("\nEmpty Passenger List.\n");
        return;
    }
    printf("\nEnter Ticket ID: ");
    scanf("%d", &id);
    getchar();
    while (temp && temp->ticketID != id) temp = temp->next;
    if (!temp) {
        printf("Ticket not found.\n");
        return;
    }
    push(&operationHistory, temp->ticketID, temp->status, "TICKET_CANCELLED");
    strcpy(temp->status, "Cancelled");
    saveToFile();
    printf("Ticket cancelled successfully.\n");
}

void filterStatus(char status[]) {
    Ticket *temp = ticketHead;
    printf("\n==================== %s BOOKINGS ====================\n\n", status);
    if (temp == NULL) {
        printf("No %s tickets.\n", status);
        return;
    }
    while (temp != NULL) {
        if (strcmp(temp->status, status) == 0) {
            printf("ID: %-5d | Name: %-10s | Date: %-5s | Trip: %-5s\n", temp->ticketID, temp->passengerName, temp->issueDate, temp->tripTime);
        }
        temp = temp->next;
    }
}

//MAIN
int main() {
    initializeSystem();
    loadFromFile();
    int choice, i;
    while (1) {
        printf("\n---------- PNR TICKETING SYSTEM ----------\n");
        printf("1. View Schedule"
            "\n2. Issue Ticket"
            "\n3. Cancel Ticket"
            "\n4. Inspection of Ticket"
            "\n5. History"
            "\n6. Exit\n"
            "\nChoice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
            printf("\n===================== SOUTHBOUND SCHEDULE =====================\n");
            printf("%-12s | %-10s | %-10s | %-10s | %-10s\n",
                "STATION", "TRIP1", "TRIP2", "TRIP3", "FARE");
            
            for (i = 0; i < 12; i++) {
                printf("%-12s | %-10s | %-10s | %-10s | PHP %-5.2f\n",
                    stations[i].stationName,
                    stations[i].southBound[0],
                    stations[i].southBound[1],
                    stations[i].southBound[2],
                    stations[i].baseFare);
            }
            
            printf("\n===================== NORTHBOUND SCHEDULE =====================\n");
            printf("%-12s | %-10s | %-10s | %-10s | %-10s\n",
                "STATION", "TRIP1", "TRIP2", "TRIP3", "FARE");
            
            for (i = 11; i >= 0; i--) {
                printf("%-12s | %-10s | %-10s | %-10s | PHP %-5.2f\n",
                    stations[i].stationName,
                    stations[i].northBound[0],
                    stations[i].northBound[1],
                    stations[i].northBound[2],
                    stations[i].baseFare);
            }
            break;
        case 2: issueTicket(); break;
        case 3: cancelTicket(); break;
        case 4: inspectTicket(); break;
        case 5: showHistory(); break;
        case 6:
            saveToFile();
            printf("\nEXITING");
            for (i = 0; i < 5; i++) {
                printf(".");
                fflush(stdout);
                Sleep(400);
            }
            exit(0);
            break;
        default: printf("\nPlease choose from 1-6 only!\n");
        }
    }
    return 0;
}
