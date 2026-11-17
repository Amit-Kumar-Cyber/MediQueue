#ifndef MEDIQUEUE_H
#define MEDIQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//      CONSTANTS
#define MAX_DOCTORS 8
#define MAX_BEDS 10
#define MAX_QUEUE 100

#define DATA_FILE "patients.txt"

#define STATUS_AVAILABLE "Available"
#define STATUS_BUSY "Busy"
#define STATUS_OCCUPIED "Occupied"

#define STATUS_WAITING "Waiting"
#define STATUS_ADMITTED "Admitted"
#define STATUS_UNDER_TREATMENT "Under Treatment"
#define STATUS_DISCHARGED "Discharged"

/* Department Names */
#define DEPT_EMERGENCY "Emergency"
#define DEPT_CARDIOLOGY "Cardiology"
#define DEPT_ORTHOPEDICS "Orthopedics"
#define DEPT_GENERAL_MEDICINE "General Medicine"


//          DATA STRUCTURES

/* Structure for Doctor record */
struct Doctor {
    char doctorID[10];       /* e.g., "D101" */
    char name[50];           /* Doctor's full name */
    char department[30];     /* Emergency, Cardiology, Orthopedics, General Medicine */
    char status[20];         /* "Available" or "Busy" */
};

/* Structure for Bed record */
struct Bed {
    char bedID[10];          /* e.g., "B101" to "B110" */
    char status[20];         /* "Available" or "Occupied" */
};

/* Structure for Patient linked-list node */
struct Patient {
    int patientID;           /* Unique ID, starts at 1001 */
    char name[50];           /* Patient full name */
    int age;                 /* Patient age */
    char address[100];       /* Residential address */
    char phone[16];          /* Contact phone number */
    char condition[100];     /* Reported symptoms / health complaint */
    int priority;            /* 1 = Critical, 2 = High, 3 = Moderate, 4 = Low */
    char department[30];     /* Assigned hospital department */
    char doctorID[10];       /* Assigned Doctor ID or "NONE" */
    char bedID[10];          /* Assigned Bed ID or "NONE" */
    char status[20];         /* Waiting, Admitted, Under Treatment, Discharged */
    struct Patient *next;    /* Pointer to next node in linked list */
};



//       GLOBAL VARIABLES (EXTERN DECLARATIONS)
/* Defined in queue_resource.c */
extern struct Doctor doctors[MAX_DOCTORS];
extern struct Bed beds[MAX_BEDS];
extern int emergencyQueue[MAX_QUEUE];
extern int emergencyQueueCount;
extern int normalQueue[MAX_QUEUE];
extern int normalQueueCount;

/* Defined in patient.c */
extern struct Patient *patientHead;
extern int nextPatientID;



//MODULE 1: patient.c 


void readLine(char *buffer, int size);
int readInteger(const char *prompt, int min, int max);
int caseInsensitiveEquals(const char *s1, const char *s2);

int containsKeyword(const char *text, const char *keyword);
int classifyPriority(const char *condition);
const char* assignDepartment(const char *condition);
const char* getPriorityName(int priority);

struct Patient* createPatient(int id, const char *name, int age,
                              const char *address, const char *phone,
                              const char *condition, int priority,
                              const char *department, const char *doctorID,
                              const char *bedID, const char *status);
void addPatient(struct Patient *newPatient);
struct Patient* searchPatientByID(int id);
int searchPatientByName(const char *name);
void displayPatientCard(const struct Patient *p);
void displayPatients(void);
void freePatientList(void);

void registerPatientUI(void);
void searchPatientUI(void);
void updatePatientUI(void);


// MODULE 2: queue_resource.c

void initializeDoctors(void);
void initializeBeds(void);
void displayDoctors(void);
void displayBeds(void);

int insertPriorityQueue(int patientID);
void removePriorityQueueAt(int index);
int removePriorityQueueByID(int patientID);

int insertNormalQueue(int patientID);
void removeNormalQueueAt(int index);
int removeNormalQueueByID(int patientID);

void enqueuePatient(int patientID);
void rebuildQueues(void);
void displayQueues(void);

int findAvailableDoctor(const char *department);
int findAvailableBed(void);
int allocateResources(struct Patient *p);
void releaseResources(struct Patient *p);
int processWaitingPatients(void);
void dischargePatientUI(void);


//MODULE 3: main.c

int savePatients(void);
int loadPatients(void);
void displayDashboard(void);
void sortPatientsByID(void);
void displayMainMenu(void);

#endif /* MEDIQUEUE_H */
