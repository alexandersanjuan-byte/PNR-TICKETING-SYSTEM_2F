#PNR Train Ticketing System
##Overview
The PNR Train Ticketing System is a C-based console application designed to manage commuter transactions for the Philippine National Railways (PNR). This system automates ticket issuance, fare calculation, and real-time trip validation for travel between Naga and Sipocot. It is intended for use by train personnel to efficiently handle passenger data and maintain transaction logs.

##Features
Real-Time Trip Validation: Automatically detects which trains have already departed based on the current system time.

Dynamic Fare Calculation: Supports Regular and Discount (20% off) pricing with built-in ID verification logic.

Electronic Payment Integration: Simulates payments via Cash, GCash, or PayMaya.

Data Persistence: Saves and loads all ticket data using binary file handling to ensure records are not lost.

Transaction Logging: Tracks the five most recent system operations using a Queue system.

###Data Structures Used
1. 2D Array (Station Database)
Purpose: Stores the fixed names, base fares, and Northbound/Southbound trip schedules for 12 stations.

Implementation: A Station struct utilizes 2D arrays to map specific trip times to their corresponding station indices.

2. Singly Linked List (Ticket Records)
Purpose: Manages the main database of issued tickets dynamically.

Functions: Allows the system to store an unlimited number of records during runtime without pre-defining a maximum capacity in memory.

3. Queue (Transaction Log)
Purpose: Tracks recent system actions (e.g., TICKET_ISSUED, TICKET_CANCELLED) in chronological order.

Implementation: A circular-style array queue limited to 5 entries to provide a quick snapshot of the latest activity.

###Algorithms Used
1. Linear Search
Purpose: Used to locate a specific ticket within the database.

Implementation: The system traverses the Linked List from the head, comparing the user-input ID against each ticket's unique ID until a match is found.

2. Bubble Sort
Purpose: Organizes the ticket history by Ticket ID in ascending order.

Implementation: When viewing sorted history, the system creates a copy of the list and repeatedly swaps adjacent elements that are out of order.


##Other Code Features
###Real-Time Conversion
Purpose: Converts string-based trip times (e.g., "4:29 AM") into total minutes from midnight for mathematical comparison against the current time.

###File Handling
The system utilizes Binary File I/O for high-speed data storage:

File Name: tickets.txt

Auto-Save: The system automatically updates the file whenever a ticket is issued or cancelled.

Auto-Load: On startup, the system restores all previous records and finds the last used Ticket ID to prevent duplicates.

###Color Handling
The system utilizes the windows.h library to implement a setColor function, which manages terminal text colors such as Cyan, Green, and Red. These colors are applied to the console's SetConsoleTextAttribute to visually distinguish between different menu sections, transaction logs, and error messages.

###How to Compile and Run
Prerequisites
OS: Windows (Required for windows.h and Sleep() functions).

Compiler: Any C-compatible compiler (GCC/MinGW).

Compilation
Using GCC:

Bash
gcc "PNR TICKETING SYSTEM (1).c" -o PNR_System.exe
Execution
Run PNR_System.exe.

Follow the on-screen menu to view schedules or issue tickets.

Ensure the console window is wide enough to properly display the ASCII tables.

###Citation
Base Logic: Inspired by Techno Greek (2026).

Time Handling: Syntaxes for real-time trip detection developed with assistance from Gemini AI.
