# PNR-TICKETING-SYSTEM_2F
PNR Ticketing System is a console-based railway ticketing system developed using the C programming language. It is designed for use by train personnel, such as ticket tellers and onboard staff, to efficiently handle commuter transactions.

## Data Structures & Algorithms

## Data Structure

Data Structure | Description
Array      | Used in stations[MAX_STATIONS] to store station names, base fares, and schedules (southbound and northbound trips). This allows fast indexed access when displaying schedules and computing fares.
Linked-List  | Used in Ticket *ticketHead where each ticket is dynamically created and linked using next. This supports adding tickets (issue), deleting/canceling tickets, and traversing all records.
Structs | Organizes system data into structured formats for stations and passenger ticket information (name, route, fare, status, etc.). 

## Algorithm

Algorithm      | Description                         
Linear Search  | Searches station names by comparing user input with all stored station names in the array. Used to validate origin and destination inputs.
Traversal |Traverses all ticket nodes starting from ticketHead to display, search, filter, and update ticket records.      

## Feature


- **Add Ticket (Issue Ticket)**– Creates a full ticket transaction including passenger details, route selection, schedule assignment, fare computation, discount application, and payment processing.
- *Cancel Ticket* – Updates ticket status to “Cancelled” using Ticket ID without deleting records from the system.
- *Inspect Ticket* – Displays complete ticket information including passenger name, origin, destination, schedule, fare, payment method, issue date, and status.
- *View Ticket History* – Displays all ticket records using linked list traversal, including full history and latest ticket view.
- *View Schedule* – Displays southbound and northbound train schedules using array-based station data with trip times and fares.
- *Fare Calculation & Discount System* – Computes fare based on station distance and applies discounts for Student, PWD, and Senior with ID verification.
- *Payment Processing* – Handles Cash and E-Wallet (GCash/PayMaya), validates payment amount, calculates change, and confirms ticket issuance.
- *Station Search System* – Validates origin and destination input using linear search in station array.
- *File Saving & Loading* – Stores ticket records in a binary file (tickets.dat) and reloads them when the system starts for data persistence.

## How to Compile and Run

Requirements

- GCC-compatible compiler (MinGW included in Dev-C++)
### Run
- Open Dev-C++
- Select main.c file
- Click Compile & Run or press F11
