# PNR Train Ticketing System

## Overview
The PNR Train Ticketing System is a console-based application designed to manage train ticket bookings and operations for the Philippine National Railways (PNR). The system allows users to view schedules, issue tickets, cancel tickets, inspect ticket details, and view ticket history with various filtering and sorting options.It is designed for use by train personnel, such as ticket tellers and onboard staff, to efficiently handle commuter transactions.

### Purpose
This system provides an efficient way to:
- Issue train tickets for passengers
- Manage ticket operations and history
- Track ticket status (Confirmed or Cancelled)
- Sort and filter ticket records
- Store ticket information persistently

## Data Structures Used

### 1. **Arrays**
- **Purpose**: Store station information (names, fares, and trip schedules)
- **Features Supported**:
  - Southbound and Northbound schedule storage
  - Fixed station list (12 stations maximum)
  - Quick access to station data by index

### 2. **Linked Lists**
- **Purpose**: Store ticket records dynamically
- **Features Supported**:
  - Dynamic ticket creation without fixed size limit
  - Easy insertion of new tickets
  - Efficient deletion of ticket records
  - Maintains ticket history in order

### 3. **Queue (FIFO - First In, First Out)**
- **Purpose**: Track pending ticket operations in order of processing
- **Features Supported**:
  - Add ticket operations to queue (Enqueue)
  - Process tickets in the order they were created
  - Display pending operations
  - Manage ticket processing workflow

### 4. **Stack (LIFO - Last In, First Out)**
- **Purpose**: Store operation history for undo functionality
- **Features Supported**:
  - Track ticket status changes
  - Record operation history with timestamps
  - Undo capability for ticket modifications
  - Maintain action history log

---

## Algorithms Used

### 1. **Linear Search**
- **Purpose**: Find stations and tickets quickly
- **Where Used**:
  - `searchStation()` - Find station by name
  - `inspectTicket()` - Find ticket by ID
  - `cancelTicket()` - Locate ticket to cancel

### 2. **Bubble Sort**
- **Purpose**: Sort tickets by Ticket ID in ascending order
- **Where Used**:
  - `bubbleSortTickets()` - Sort all tickets
  - Display sorted history feature
- **Feature Supported**: Organized ticket viewing

# File Handling Used
## Purpose
Save and load ticket data so information is not lost when the application closes.

## Two Main Functions
1. Save Function - saveToFile()
Saves all tickets to a file called tickets.dat
Runs automatically when:
A new ticket is created
A ticket is cancelled
User exits the application
Creates file if it doesn't exist

3. Load Function - loadFromFile()
Loads all saved tickets from tickets.dat when application starts
Restores all previous ticket data to memory
If file doesn't exist, starts with empty ticket list
File Details
File Name: tickets.dat
File Type: Binary file (compact and fast)
Location: Same folder as the program
Auto-created: Yes (first time a ticket is saved)


## How to Compile and Run

### Prerequisites
- C++ Compiler (GCC, MinGW, or Visual Studio)
- Windows Operating System (uses `windows.h` library)
- Text Editor or IDE

### Compilation Steps

#### Using GCC/MinGW (Windows):
```bash
gcc -o pnr_ticketing "PNR file.cpp"

### Run
- Open Dev-C++
- Select this .cpp file
- Click Compile & Run or press F11
