/*
PNR TRAIN TICKETING SYSTEM
	Data Structures Used
	-2D Array
	-Singly Linked List
	-Queue
	
	Algorithms Used
	-Linear Search
	-Bubble Sort
	
	"This was Emphasized on README.md"
	
Citation
Techno Greek. “TRAIN TICKET RESERVATION SYSTEM in c PROGRAMMING with SOURCE CODE.” Youtu.be, 2026, youtu.be/fBP8R8rDw9s?si=dzbWH5R1B3uvaSqE. Accessed 10 May 2026.
Gemini Prompt: Create Sample Syntaxes on Time handling for our PNR Train Ticketing System to detect Train Trip Based on Real Time
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

//Decalare sizes
#define MAX_STATIONS 12
#define MAX_TRIPS 3
#define MAX_QUEUE 5
#define FILE_NAME "tickets.txt"

//Declare color
#define RESET 7
#define CYAN 11
#define GREEN 10
#define YELLOW 14
#define RED 12
#define MAGENTA 13

typedef enum { REGULAR = 0, DISCOUNT = 1 } FareType;

//2D Array
typedef struct {
    char stationName[30];
    float baseFare;
    char sipocotnaga[MAX_TRIPS][15];
    char nagasipocot[MAX_TRIPS][15];
} Station;

//Singly Linked List
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

//queue in array
typedef struct {
    int ticketID;
    char passengerName[50];
    char action[30];
} QueueNode;

typedef struct {
    QueueNode items[MAX_QUEUE];
    int front, rear;
} Queue;

//Global Variables
Station stations[MAX_STATIONS];
Ticket *ticketHead = NULL;
int lastIssuedID = -1;
int totalTickets = 0;
Queue operationQueue;

//Time handling: this block used AI tool Gemini
int timeToMinutes(const char *timeStr) {
    int h, m;
    char period[3];
    sscanf(timeStr, "%d:%d %s", &h, &m, period);
    if (strcmp(period, "PM") == 0 && h != 12) h += 12;
    if (strcmp(period, "AM") == 0 && h == 12) h = 0;
    return h * 60 + m;
}

//Void Declaration
void filterStatus(char status[]);
void bubbleSortTickets(Ticket **head);
void displaySortedHistory(Ticket * head);
void saveToFile();

//Color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Queue
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

//display current transaction log
void displayQueue(Queue *q) {
    int i;
    if (q->front > q->rear) {
        setColor(RED);
		printf("\nCurrent Transaction Log is empty.\n");
        return;
    }
    setColor(CYAN);
    printf("\n+----------+----------------------+--------------------------+\n");
    printf("|                CURRENT TRANSACTION LOG (QUEUE)  	       	  |\n");
    printf("+----------+----------------------+--------------------------+\n");
    setColor(RESET);
    printf("| TICKET # | PASSENGER            | ACTION                   |\n");
    setColor(CYAN);
    printf("+----------+----------------------+--------------------------+\n");
    setColor(RESET);
    for (i = q->front; i <= q->rear; i++) {
        printf("| %-8d | %-20s | %-24s |\n",
            q->items[i].ticketID,
            q->items[i].passengerName,
            q->items[i].action);
    }
    setColor(CYAN);
    printf("+----------+----------------------+--------------------------+\n");
    setColor(RESET);
}

Ticket* copyList(Ticket *head) {
    Ticket *newHead = NULL, *tail = NULL;

    while (head) {
        Ticket *node = (Ticket*)malloc(sizeof(Ticket));
        *node = *head;       
        node->next = NULL;   

        if (!newHead) {
            newHead = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        head = head->next;
    }

    return newHead;
}

//Bubble Sort
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
                current->ticketID    = nextNode->ticketID;
                current->ticketPrice = nextNode->ticketPrice;
                strcpy(current->passengerName, nextNode->passengerName);
                strcpy(current->fromStation,   nextNode->fromStation);
                strcpy(current->toStation,     nextNode->toStation);
                strcpy(current->tripTime,      nextNode->tripTime);
                strcpy(current->paymentMethod, nextNode->paymentMethod);
                strcpy(current->issueDate,     nextNode->issueDate);
                strcpy(current->status,        nextNode->status);
                nextNode->ticketID    = temp.ticketID;
                nextNode->ticketPrice = temp.ticketPrice;
                strcpy(nextNode->passengerName, temp.passengerName);
                strcpy(nextNode->fromStation,   temp.fromStation);
                strcpy(nextNode->toStation,     temp.toStation);
                strcpy(nextNode->tripTime,      temp.tripTime);
                strcpy(nextNode->paymentMethod, temp.paymentMethod);
                strcpy(nextNode->issueDate,     temp.issueDate);
                strcpy(nextNode->status,        temp.status);
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}

//sorted ticket history
void displaySortedHistory(Ticket * head) {
    if (!ticketHead) { printf("\nNo tickets found.\n"); return; }
    Ticket *t = head;
    setColor(YELLOW);
    printf("\n+------------------------------------------------------------------+\n");
    printf("|           SORTED TICKET HISTORY (Bubble Sort by ID)             |\n");
    printf("+------------+------------+------------+-----------+---------------+\n");
    setColor(RESET);
    printf("| TICKET #   | FROM       | TO         | FARE      | STATUS        |\n");
    setColor(YELLOW);
    printf("+------------+------------+------------+-----------+---------------+\n");
    setColor(RESET);
    while (t) {
        printf("| %-10d | %-10s | %-10s | PHP%-6.2f | %-13s |\n",
            t->ticketID, t->fromStation, t->toStation,
            t->ticketPrice, t->status);
        t = t->next;
    }
    setColor(YELLOW);
    printf("+------------+------------+------------+-----------+---------------+\n");
    setColor(RESET);
}

//File Handling
void saveToFile() {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) { printf("Error opening file!\n"); return; }
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
        if (fread(temp, sizeof(Ticket), 1, fp) != 1) { free(temp); break; }
        temp->next = ticketHead;
        ticketHead = temp;
        
        if (temp->ticketID > lastIssuedID) {
            lastIssuedID = temp->ticketID;
        }
        
        totalTickets++;
    }
    fclose(fp);
}

//Initialize Stations Information
void initializeSystem() {
    char *names[] = {"NAGA","SAMPALOC","BORABOD","PAMPLONA","MAMBULO","MALANSAD",
                     "RONGOS","LIBMANAN","CAMAMBUGAN","MANTALISAY","AWAYAN","SIPOCOT"};

    char *sbTimes[12][3] = {
        {"4:29 AM","9:49 AM","3:14 PM"},  {"4:41 AM","10:01 AM","3:26 PM"},
        {"4:47 AM","10:07 AM","3:32 PM"}, {"4:53 AM","10:13 AM","3:38 PM"},
        {"4:59 AM","10:19 AM","3:44 PM"}, {"5:05 AM","10:25 AM","3:50 PM"},
        {"5:11 AM","10:31 AM","3:56 PM"}, {"5:18 AM","10:38 AM","4:03 PM"},
        {"5:24 AM","10:44 AM","4:09 PM"}, {"5:30 AM","10:50 AM","4:15 PM"},
        {"5:36 AM","10:56 AM","4:21 PM"}, {"5:42 AM","11:02 AM","4:27 PM"}
    };

    char *nbTimes[12][3] = {
        {"7:40 AM","1:00 PM","6:15 PM"},  {"7:28 AM","12:48 PM","6:03 PM"},
        {"7:22 AM","12:42 PM","5:57 PM"}, {"7:16 AM","12:36 PM","5:51 PM"},
        {"7:10 AM","12:30 PM","5:45 PM"}, {"7:04 AM","12:24 PM","5:39 PM"},
        {"6:58 AM","12:18 PM","5:33 PM"}, {"6:51 AM","12:11 PM","5:26 PM"},
        {"6:45 AM","12:05 PM","5:20 PM"}, {"6:39 AM","11:59 AM","5:14 PM"},
        {"6:33 AM","11:53 AM","5:08 PM"}, {"6:25 AM","11:45 AM","5:00 PM"}
    };

    float fares[] = {0,15,15,20,25,30,35,40,45,50,55,60};
    int i, j;

    initializeQueue(&operationQueue);

    for (i = 0; i < 12; i++) {
        strcpy(stations[i].stationName, names[i]);
        stations[i].baseFare = fares[i];
        for (j = 0; j < 3; j++) {
            strcpy(stations[i].sipocotnaga[j], sbTimes[i][j]);
            strcpy(stations[i].nagasipocot[j], nbTimes[i][j]);
        }
    }
}

//Search Passenger
void searchPassenger() {
    int id;
    Ticket *temp = ticketHead;
    int found = 0;

    if (ticketHead == NULL) {
        printf("\n[!] No tickets in the system yet.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    setColor(MAGENTA);
    printf("\n+---------------------------------------+\n");
    printf("|          INSPECT PASSENGER ID          |\n");
    printf("+---------------------------------------+\n");
    setColor(RESET);
    printf("Enter Ticket ID: ");
    if (scanf("%d", &id) != 1) { 
        setColor(RED);
		printf("[!] Invalid input. Please enter a numeric ID.\n");
        while (getchar() != '\n'); 
        return;
    }
    getchar(); 

    while (temp != NULL) {
        if (temp->ticketID == id) {
            setColor(GREEN);
            printf("\n+============================================================+\n");
            printf("|        PHILIPPINE NATIONAL RAILWAYS (PNR)                 |\n");
            printf("|                    OFFICIAL TICKET                        |\n");
            printf("+============================================================+\n");
            setColor(RESET);
            printf("| ID      : %-46d |\n", temp->ticketID);
            printf("| Date    : %-46s |\n", temp->issueDate);
            printf("| Name    : %-46s |\n", temp->passengerName);
            printf("| Route   : %-20s TO %-22s |\n", temp->fromStation, temp->toStation);
            printf("| Sched   : %-46s |\n", temp->tripTime);
            printf("| Fare    : PHP %-42.2f |\n", temp->ticketPrice);
            printf("| Status  : %-46s |\n", temp->status);
            setColor(GREEN);
            printf("+============================================================+\n");
            setColor(RESET);
            found = 1;
            break; 
        }
        temp = temp->next;
    }
    if (!found) {
    	setColor(RED);
	    printf("\n[!] Ticket ID %d not found.\n", id);
	    setColor(RESET);
	    printf("\nPress Enter to return to menu...");
	    getchar();
	}
}

//Payment
int processPayment(float amount, char *method) {
    int choice, type, k, i;
    float cash;
    char account[20];

    while (1) {
        printf("\n--- PAYMENT ---\nAmount: PHP %.2f\n", amount);
        printf("1. Cash\n2. E-Wallet\nChoice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            strcpy(method, "CASH");

            printf("Cash: ");
            scanf("%f", &cash);
            getchar();

            if (cash < amount) {
            	setColor(RED);
                printf("[!] Insufficient cash!\n");
                setColor(RESET);
                return 0;
            }

            printf("Change: PHP %.2f\n", cash - amount);
            break;
        }
        else if (choice == 2) {

            while (1) {
                printf("1. GCash\n2. PayMaya\nChoice: ");
                scanf("%d", &type);
                getchar();

                if (type < 1 || type > 2) {
                    printf("\nInvalid choice.\n\n");
                    continue;
                }

                printf("Account: ");
                fgets(account, sizeof(account), stdin);
                account[strcspn(account, "\n")] = 0;

                if (strlen(account) != 11) {
                	setColor(RED);
                    printf("\n[!] Invalid account number. Must be 11 digits.\n\n");
                    setColor(RESET);
                    continue;
                }

                int valid = 1;
                for (i = 0; account[i]; i++) {
                    if (!isdigit(account[i])) {
                        valid = 0;
                        break;
                    }
                }

                if (!valid) {
                	setColor(RED);
                    printf("\n[!] Please input numbers only.\n\n");
                    setColor(RESET);
                    continue;
                }

                sprintf(method, "%s (%s)", (type == 1 ? "GCASH" : "PAYMAYA"), account);
                break;
            }

            break;
        }
        else {
        	setColor(RED);
            printf("\n[!] Invalid choice.\n");
            setColor(RESET);
        }
    }
    printf("\nTicket Issued Successfully!\nPrinting ticket");
    for (k = 0; k < 3; k++) {
        printf(".");
        fflush(stdout);
        Sleep(500);
    }
    printf("\nDone!\n");

    return 1;   
}

//issue ticket
void issueTicket() {
    int fIdx, tIdx, type, tripChoice, hasID, i;
    char name[50];
    float price;
    char method[40];

	// Real-time variables
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    int currentMinutes = lt->tm_hour * 60 + lt->tm_min;
    
    printf("\nName: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;

    // Station Selection by Number
    setColor(CYAN);
    printf("\n--- SELECT STATION ---\n");
    for(i=0; i < MAX_STATIONS; i++) {
        printf("[%d] %-15s", i+1, stations[i].stationName);
        if((i+1)%3 == 0) printf("\n");
    }
    setColor(RESET);

	while (1) {
	printf("\nEnter Origin # (1-12): ");
	if (scanf("%d", &fIdx) != 1) {
	    setColor(RED);
	    printf("\n[!] Invalid input. Please enter numbers only.\n");
		setColor(RESET);
	    while (getchar() != '\n'); 
	    continue;
	}
	fIdx--; 
	
	printf("Enter Destination # (1-12): ");
	if (scanf("%d", &tIdx) != 1) {
	    setColor(RED);
	    printf("\n[!] Invalid input. Please enter numbers only.\n");
		setColor(RESET);
	    while (getchar() != '\n');
	    continue;
	}
	tIdx--; 

	getchar();
	
	 if (fIdx < 0 || fIdx >= MAX_STATIONS || tIdx < 0 || tIdx >= MAX_STATIONS || fIdx == tIdx) {
    	setColor(RED);
        printf("\nInvalid route selection!\n");
        return;
    }
    break;
}

   // REAL-TIME TRIP VALIDATION
    printf("\n--- AVAILABLE TRIPS (Current Time: %02d:%02d) ---\n", lt->tm_hour, lt->tm_min);
    int availableCount = 0;
    for(i = 0; i < 3; i++) {
        char *tStr = (fIdx < tIdx) ? stations[fIdx].sipocotnaga[i] : stations[fIdx].nagasipocot[i];
        if (timeToMinutes(tStr) > currentMinutes) {
            printf("[%d] %-10s - AVAILABLE\n", i+1, tStr);
            availableCount++;
        } else {
            setColor(RED);
            printf("[%d] %-10s - DEPARTED\n", i+1, tStr);
            setColor(RESET);
        }
    }

    if (availableCount == 0) {
        printf("\n[!] No more trips available for this route today.\n");
        printf("Press Enter to return to menu...");
        getchar();
        return;
    }

    while(1) {
        printf("\nSelect Trip # (1-3): ");
        if (scanf("%d", &tripChoice) != 1) {
        setColor(RED);
	    printf("\n[!] Invalid input. Please choose from 1-3 only.\n");
		setColor(RESET);
            while(getchar() != '\n'); 
			continue;
        }
        getchar();
        if (tripChoice < 1 || tripChoice > 3) { 
        setColor(RED);
		printf("[!] Invalid choice. Please choose from 1-3 only.\n"); 
		setColor(RESET);
		continue; 
		}
        
        char *tStr = (fIdx < tIdx) ? stations[fIdx].sipocotnaga[tripChoice-1] : stations[fIdx].nagasipocot[tripChoice-1];
        if (timeToMinutes(tStr) <= currentMinutes) {
        	setColor(RED);
            printf("[!] This trip has already departed. Please pick an available one.\n");
            setColor(RESET);
        } else {
            break;
        }
    }

    printf("Type (0-Regular, 1-Discount): ");
    scanf("%d", &type);
    getchar();


    if (type == DISCOUNT) {
        printf("\nDoes the passenger have a valid ID? (1-Yes / 0-No): ");
        scanf("%d", &hasID);
        getchar();
        printf("\nVerifying");
        for (i = 0; i < 5; i++) { 
		printf("."); 
		fflush(stdout); 
		Sleep(400); 
		}
		
        if (hasID == 1) {
        	setColor(GREEN);
        	printf("\n[OK] Verification Successful!\n");
        	setColor(RESET);
		}
        else {
        	setColor(RED);
            printf("\n[!] Verification Failed!\n");
            printf("Discount removed. Switching to REGULAR fare.\n");
            setColor(RESET);
            type = REGULAR;
        }
    }

   price = (float)abs((int)stations[fIdx].baseFare - (int)stations[tIdx].baseFare);
    if (price < 15) price = 15;
    if (type != REGULAR) price *= 0.8f;

    if (!processPayment(price, method)) return;

    Ticket *newTicket = (Ticket *)malloc(sizeof(Ticket));
    newTicket->ticketID = 1000 + totalTickets;
    lastIssuedID = newTicket->ticketID; 
    
    strcpy(newTicket->passengerName, name);
    strcpy(newTicket->fromStation, stations[fIdx].stationName);
    strcpy(newTicket->toStation, stations[tIdx].stationName);

    if (fIdx < tIdx){
        strcpy(newTicket->tripTime, stations[fIdx].sipocotnaga[tripChoice - 1]);
    } else {
        strcpy(newTicket->tripTime, stations[fIdx].nagasipocot[tripChoice - 1]);
    }
    strcpy(newTicket->paymentMethod, method);
    strcpy(newTicket->status, "Confirmed");
    newTicket->ticketPrice = price;

    strftime(newTicket->issueDate, 20, "%Y-%m-%d %H:%M", lt);

    setColor(GREEN);
    printf("\n+============================================================+\n");
    printf("|        PHILIPPINE NATIONAL RAILWAYS (PNR)                 |\n");
    printf("|                    OFFICIAL TICKET                        |\n");
    printf("+============================================================+\n");
    setColor(RESET);
    printf("| ID      : %-46d |\n", newTicket->ticketID);
    printf("| Date    : %-46s |\n", newTicket->issueDate);
    printf("| Name    : %-46s |\n", newTicket->passengerName);
    printf("| Route   : %-20s TO %-22s |\n", newTicket->fromStation, newTicket->toStation);
    printf("| Sched   : %-46s |\n", newTicket->tripTime);
    printf("| Fare    : PHP %-42.2f |\n", newTicket->ticketPrice);
    printf("| Status  : %-46s |\n", newTicket->status);
    setColor(GREEN);
    printf("+============================================================+\n");
    setColor(RESET);

	newTicket->next = ticketHead;
	ticketHead = newTicket;

    enqueue(&operationQueue, newTicket->ticketID, name, "TICKET_ISSUED");
    totalTickets++;
    saveToFile();

    printf("\nPress Enter to return to menu..."); 
	getchar();
}

//history
void showHistory() {
    int choice;

    while (1) {
        if (!ticketHead) {
        	setColor(RED);
            printf("\nNo tickets found.\n");
            setColor(RESET);
        } else {
            Ticket *last = ticketHead;
            setColor(GREEN);
            printf("\n+============================================================+\n");
            printf("|        PHILIPPINE NATIONAL RAILWAYS (PNR)                  |\n");
            printf("|                    LAST TICKET                             |\n");
            printf("+============================================================+\n");
            setColor(RESET);
            printf("| ID      : %-48d |\n", last->ticketID);
            printf("| Date    : %-48s |\n", last->issueDate);
            printf("| Name    : %-48s |\n", last->passengerName);
            printf("| Route   : %-8s  TO  %-34s |\n", last->fromStation, last->toStation);
            printf("| Sched   : %-48s |\n", last->tripTime);
            printf("| Fare    : PHP %-44.2f |\n", last->ticketPrice);
            setColor(GREEN);
            printf("+============================================================+\n");

            setColor(CYAN);
            printf("\n   		 ========== FULL HISTORY ==========     \n");
            printf("\n+------+------------+------------+------------+-----------+---------------+\n");
            printf("| NO.  | TICKET #   | FROM       | TO         | FARE      | STATUS        |\n");
            printf("+------+------------+------------+------------+-----------+---------------+\n");
            setColor(RESET);
            Ticket *t = ticketHead;
            int no = 1;
            while (t) {
                printf("| %-4d | %-10d | %-10s | %-10s | PHP%-6.2f | %-13s |\n",
                    no++, t->ticketID, t->fromStation,
                    t->toStation, t->ticketPrice, t->status);
                t = t->next;
            }
            setColor(CYAN);
            printf("+------+------------+------------+------------+-----------+---------------+\n");
            setColor(RESET);
        }

        printf("\n+-------+-------------------------------+\n");
        printf("|  [1]  | View Confirmed Tickets        |\n");
        printf("|  [2]  | View Cancelled Tickets        |\n");
        printf("|  [3]  | View Sorted History           |\n");
        printf("|  [4]  | Reset All Records             |\n");
        printf("|  [5]  | Back to Main Menu             |\n");
        printf("+-------+-------------------------------+\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) filterStatus("Confirmed");
        else if (choice == 2) filterStatus("Cancelled");
        else if (choice == 3) {

		    Ticket *copy = copyList(ticketHead);   
		    bubbleSortTickets(&copy);             
		
		    displaySortedHistory(copy);           
		
		   
		    while (copy) {
		        Ticket *tmp = copy;
		        copy = copy->next;
		        free(tmp);
		    }
	}
 
        else if (choice == 4) {
            if (ticketHead == NULL) { printf("\nNo records to delete.\n"); continue; }
            char confirm[10];
            printf("\nAre you sure you want to delete all records?\n");
            printf("Type YES to confirm: ");
            fgets(confirm, 10, stdin);
            confirm[strcspn(confirm, "\n")] = 0;
            if (strcmp(confirm, "YES") == 0) {
                Ticket *current = ticketHead, *tmp;
                while (current != NULL) { tmp = current; current = current->next; free(tmp); }
                ticketHead = NULL;
                totalTickets = 0;
                initializeQueue(&operationQueue);
                saveToFile();
                printf("All records deleted successfully.\n");
            } else {
                printf("Reset cancelled.\n");
            }
        }
        else if (choice == 5) break;
        else printf("Invalid choice.\n");
    }
}

//Cancel Ticket
void cancelTicket() {
    int id;
    Ticket *temp = ticketHead;

    if (temp == NULL) { printf("\nEmpty Passenger List.\n"); return; }

    printf("\nEnter Ticket ID: ");
    scanf("%d", &id);
    getchar();

    while (temp && temp->ticketID != id)
        temp = temp->next;

    if (!temp) { printf("Ticket not found.\n"); return; }

    if (strcmp(temp->status, "Cancelled") == 0) {
        printf("\nThis ticket is already cancelled.\n");
        return;
    }

    strcpy(temp->status, "Cancelled");
    enqueue(&operationQueue, temp->ticketID, temp->passengerName, "TICKET_CANCELLED");
    saveToFile();
    printf("Ticket cancelled successfully.\n");
}

//Sort by status
void filterStatus(char status[]) {
    Ticket *temp = ticketHead;
    int found = 0;

    setColor(MAGENTA);
    printf("\n+------------------------------------------------------------------+\n");
    printf("|  %-6s Tickets						   |\n", status);
    printf("+----------+------------------+--------------------+---------------+\n");
    setColor(RESET);
    printf("| TICKET # | NAME             | DATE               | TRIP          |\n");
    setColor(MAGENTA);
    printf("+----------+------------------+--------------------+---------------+\n");
    setColor(RESET);

    while (temp != NULL) {
        if (strcmp(temp->status, status) == 0) {
            printf("| %-8d | %-16s | %-18s | %-13s |\n",
                temp->ticketID, temp->passengerName,
                temp->issueDate, temp->tripTime);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) printf("|  No %-61s|\n", "tickets found.");
    setColor(MAGENTA);
    printf("+----------+------------------+--------------------+---------------+\n");
    setColor(RESET);
}

//Main Menu
int main() {
    initializeSystem();
    loadFromFile();

    int choice, i;

    while (1) {
        setColor(YELLOW);
        printf("\n+---------------------------------------+\n");
        printf("|       PNR TICKETING SYSTEM	        |\n");
        printf("+-------+-------------------------------+\n");
        setColor(RESET);
        printf("|  [1]  | View Train Schedule           |\n");
        printf("|  [2]  | Issue New Ticket              |\n");
        printf("|  [3]  | Cancel a Ticket               |\n");
        printf("|  [4]  | Inspect Passenger             |\n");
        printf("|  [5]  | Current Transaction Log(Queue)|\n");
        printf("|  [6]  | Ticket History                |\n");
        printf("|  [7]  | Exit Program                  |\n");
        setColor(YELLOW);
        printf("+-------+-------------------------------+\n");
        setColor(RESET);
        printf("Enter choice: "); 
		scanf("%d", &choice); 
		getchar();
        switch (choice) {

        case 1:
            setColor(CYAN);
            printf("\n+=================================================================+\n");
            printf("|                   PNR TRAIN SCHEDULE                           |\n");
            printf("+=================================================================+\n");
            setColor(RESET);
            printf("\n--- SIPOCOT TO NAGA (Southbound) ---\n");
            printf("%-13s | %-10s | %-10s | %-10s | FARE\n","STATION","TRIP 1","TRIP 2","TRIP 3");
            printf("-----------------------------------------------------------------\n");
             for (i = 0; i < 12; i++) {
                printf("%-12s | %-10s | %-10s | %-10s | PHP %-5.2f\n",
                    stations[i].stationName,
                    stations[i].sipocotnaga[0],
                    stations[i].sipocotnaga[1],
                    stations[i].sipocotnaga[2],
                    stations[i].baseFare);
            }
            printf("\n--- NAGA TO SIPOCOT (Northbound) ---\n");
            printf("%-13s | %-10s | %-10s | %-10s | FARE\n","STATION","TRIP 1","TRIP 2","TRIP 3");
            printf("-----------------------------------------------------------------\n");
            for (i = 11; i >= 0; i--) {
                printf("%-12s | %-10s | %-10s | %-10s | PHP %-5.2f\n",
                    stations[i].stationName,
                    stations[i].nagasipocot[0],
                    stations[i].nagasipocot[1],
                    stations[i].nagasipocot[2],
                    stations[i].baseFare);
            }
            break;

        case 2: 
				issueTicket(); 
				break;
            case 3: 
				cancelTicket(); 
				break;
            case 4: 
				searchPassenger(); 
				break;
            case 5: 
				displayQueue(&operationQueue); 
				break;
            case 6: 
				showHistory(); 
				break;
            case 7: 
				saveToFile(); 
				exit(0);
            default:
            	setColor(RED);
			 	printf("\nInvalid choice.\n");
        }
    }

    return 0;
}
