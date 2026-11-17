// 3. Naivedya Sharma 

#include "mediqueue.h"


/*FILE HANDLING FUNCTIONS*/
/*savePatients*/

int savePatients(void) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("\nError: Unable to open file '%s' for saving!\n", DATA_FILE);
        return 0;
    }

    int count = 0;
    struct Patient *curr = patientHead;
    while (curr != NULL) {
        fprintf(fp, "%d|%s|%d|%s|%s|%s|%d|%s|%s|%s|%s\n",
                curr->patientID,
                curr->name,
                curr->age,
                curr->address,
                curr->phone,
                curr->condition,
                curr->priority,
                curr->department,
                curr->doctorID,
                curr->bedID,
                curr->status);
        curr = curr->next;
        count++;
    }

    fclose(fp);
    return 1;
}

//  loadPatients 
int loadPatients(void) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf("No existing '%s' file found. Starting with a fresh database.\n", DATA_FILE);
        return 0;
    }

    char line[512];
    int loadedCount = 0;
    int maxID = 1000;

    while (fgets(line, sizeof(line), fp) != NULL) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
            len--;
        }

        if (len == 0) continue;

        /* Parse pipe-delimited fields */
        char *fields[11];
        int fieldIndex = 0;
        char *ptr = line;

        fields[fieldIndex++] = ptr;
        while (*ptr != '\0' && fieldIndex < 11) {
            if (*ptr == '|') {
                *ptr = '\0';
                fields[fieldIndex++] = ptr + 1;
            }
            ptr++;
        }

        if (fieldIndex == 11) {
            int id = atoi(fields[0]);
            char *name = fields[1];
            int age = atoi(fields[2]);
            char *address = fields[3];
            char *phone = fields[4];
            char *condition = fields[5];
            int priority = atoi(fields[6]);
            char *department = fields[7];
            char *doctorID = fields[8];
            char *bedID = fields[9];
            char *status = fields[10];

            struct Patient *p = createPatient(id, name, age, address, phone,
                                              condition, priority, department,
                                              doctorID, bedID, status);
            if (p != NULL) {
                addPatient(p);
                loadedCount++;

                if (id > maxID) {
                    maxID = id;
                }

                /* Reconstruct active doctor and bed availability */
                if (strcmp(status, STATUS_ADMITTED) == 0 ||
                    strcmp(status, STATUS_UNDER_TREATMENT) == 0) {
                    /* Mark doctor Busy */
                    if (strcmp(doctorID, "NONE") != 0 && strlen(doctorID) > 0) {
                        int docFound = 0;
                        for (int d = 0; d < MAX_DOCTORS; d++) {
                            if (strcmp(doctors[d].doctorID, doctorID) == 0) {
                                strcpy(doctors[d].status, STATUS_BUSY);
                                docFound = 1;
                                break;
                            }
                        }
                        if (!docFound) {
                            printf("Warning: Saved doctor ID '%s' in record %d not found in doctor list.\n", doctorID, id);
                        }
                    }

                    /* Mark bed Occupied */
                    if (strcmp(bedID, "NONE") != 0 && strlen(bedID) > 0) {
                        int bedFound = 0;
                        for (int b = 0; b < MAX_BEDS; b++) {
                            if (strcmp(beds[b].bedID, bedID) == 0) {
                                strcpy(beds[b].status, STATUS_OCCUPIED);
                                bedFound = 1;
                                break;
                            }
                        }
                        if (!bedFound) {
                            printf("Warning: Saved bed ID '%s' in record %d not found in bed list.\n", bedID, id);
                        }
                    }
                }
                /* Discharged patients: do not reserve any resources */
            }
        } else {
            printf("Warning: Skipping malformed record in '%s'.\n", DATA_FILE);
        }
    }

    fclose(fp);

    /* Set next patient ID based on loaded records */
    nextPatientID = maxID + 1;

    /* Rebuild waiting queues from loaded Waiting patients */
    rebuildQueues();

    printf("Database loaded: %d record(s) restored from '%s'.\n", loadedCount, DATA_FILE);
    printf("Next Patient ID set to: %d\n", nextPatientID);

    /* Run waiting-patient processing once if any patients are waiting */
    if (emergencyQueueCount > 0 || normalQueueCount > 0) {
        printf("\nStartup Queue Check: Attempting allocation for waiting patients...\n");
        processWaitingPatients();
    }

    return loadedCount;
}


/*REPORTS & SORTING FUNCTIONS*/
//displayDashboard:

void displayDashboard(void) {
    int total = 0;
    int waiting = 0;
    int admitted = 0;
    int underTreatment = 0;
    int discharged = 0;

    int deptEmergency = 0;
    int deptCardiology = 0;
    int deptOrthopedics = 0;
    int deptGeneralMed = 0;

    struct Patient *curr = patientHead;
    while (curr != NULL) {
        total++;
        if (strcmp(curr->status, STATUS_WAITING) == 0) {
            waiting++;
        } else if (strcmp(curr->status, STATUS_ADMITTED) == 0) {
            admitted++;
        } else if (strcmp(curr->status, STATUS_UNDER_TREATMENT) == 0) {
            underTreatment++;
        } else if (strcmp(curr->status, STATUS_DISCHARGED) == 0) {
            discharged++;
        }

        if (caseInsensitiveEquals(curr->department, DEPT_EMERGENCY)) {
            deptEmergency++;
        } else if (caseInsensitiveEquals(curr->department, DEPT_CARDIOLOGY)) {
            deptCardiology++;
        } else if (caseInsensitiveEquals(curr->department, DEPT_ORTHOPEDICS)) {
            deptOrthopedics++;
        } else if (caseInsensitiveEquals(curr->department, DEPT_GENERAL_MEDICINE)) {
            deptGeneralMed++;
        }

        curr = curr->next;
    }

    int availDocs = 0, busyDocs = 0;
    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (strcmp(doctors[i].status, STATUS_AVAILABLE) == 0) {
            availDocs++;
        } else {
            busyDocs++;
        }
    }

    int availBeds = 0, occupiedBeds = 0;
    for (int i = 0; i < MAX_BEDS; i++) {
        if (strcmp(beds[i].status, STATUS_AVAILABLE) == 0) {
            availBeds++;
        } else {
            occupiedBeds++;
        }
    }

    printf("\n=======================================================================\n");
    printf("                       MEDIQUEUE HOSPITAL DASHBOARD                    \n");
    printf("=======================================================================\n");

    printf("\n[1] PATIENT CENSUS\n");
    printf("-----------------------------------------------------------------------\n");
    printf("  Total Registered Patients : %d\n", total);
    printf("  - Waiting in Queue        : %d\n", waiting);
    printf("  - Admitted (Bed Assigned) : %d\n", admitted);
    printf("  - Under Treatment         : %d\n", underTreatment);
    printf("  - Discharged              : %d\n", discharged);

    printf("\n[2] HOSPITAL RESOURCES\n");
    printf("-----------------------------------------------------------------------\n");
    printf("  Doctors (Total %d)         : %d Available  |  %d Busy\n", MAX_DOCTORS, availDocs, busyDocs);
    printf("  Beds    (Total %d)        : %d Available  |  %d Occupied\n", MAX_BEDS, availBeds, occupiedBeds);

    printf("\n[3] PATIENT DISTRIBUTION BY DEPARTMENT\n");
    printf("-----------------------------------------------------------------------\n");
    printf("  - Emergency               : %d\n", deptEmergency);
    printf("  - Cardiology              : %d\n", deptCardiology);
    printf("  - Orthopedics             : %d\n", deptOrthopedics);
    printf("  - General Medicine        : %d\n", deptGeneralMed);

    printf("\n[4] WAITING QUEUE METRICS\n");
    printf("-----------------------------------------------------------------------\n");
    printf("  - Emergency Priority Queue: %d waiting (Critical & High)\n", emergencyQueueCount);
    printf("  - Normal FIFO Queue       : %d waiting (Moderate & Low)\n", normalQueueCount);

    printf("=======================================================================\n");
}


//sortPatientsByID
void sortPatientsByID(void) {
    if (patientHead == NULL) {
        printf("\nNo patient records available to sort.\n");
        return;
    }

    /* 1. Count nodes in linked list */
    int count = 0;
    struct Patient *curr = patientHead;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    /* 2. Allocate temporary array of pointers */
    struct Patient **arr = (struct Patient **)malloc(count * sizeof(struct Patient *));
    if (arr == NULL) {
        printf("Error: Memory allocation failed for sorting array.\n");
        return;
    }

    /* 3. Populate pointer array */
    curr = patientHead;
    for (int i = 0; i < count; i++) {
        arr[i] = curr;
        curr = curr->next;
    }

    /* 4. Perform classic Bubble Sort on pointer array */
    for (int i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < count - i - 1; j++) {
            if (arr[j]->patientID > arr[j + 1]->patientID) {
                struct Patient *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break; /* Early termination optimization */
        }
    }

    /* 5. Display sorted records */
    printf("\n===================================================================================================================\n");
    printf("                           PATIENT RECORDS (SORTED BY ID ASCENDING - BUBBLE SORT)                                  \n");
    printf("===================================================================================================================\n");
    printf("%-6s | %-18s | %-4s | %-20s | %-4s | %-16s | %-6s | %-6s | %-14s\n",
           "ID", "Name", "Age", "Condition", "Pri", "Department", "Doctor", "Bed", "Status");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-6d | %-18.18s | %-4d | %-20.20s | %-4d | %-16.16s | %-6s | %-6s | %-14s\n",
               arr[i]->patientID,
               arr[i]->name,
               arr[i]->age,
               arr[i]->condition,
               arr[i]->priority,
               arr[i]->department,
               arr[i]->doctorID,
               arr[i]->bedID,
               arr[i]->status);
    }
    printf("===================================================================================================================\n");
    printf("Total records sorted: %d\n", count);
    printf("Note: Original singly linked-list structure and arrival order remain unchanged.\n");

    /* 6. Free temporary pointer array */
    free(arr);
}


/* MENU AND USER INTERFACE */
void displayMainMenu(void) {
    printf("\n========================================\n");
    printf("              MEDIQUEUE                 \n");
    printf("     Hospital Patient Allocation System \n");
    printf("========================================\n");
    printf("1. Register New Patient\n");
    printf("2. View All Patients\n");
    printf("3. Search Patient\n");
    printf("4. Update Patient Details\n");
    printf("5. View Patient Queues\n");
    printf("6. Process Waiting Patients\n");
    printf("7. View Doctors\n");
    printf("8. View Beds\n");
    printf("9. Discharge Patient\n");
    printf("10. View Hospital Dashboard\n");
    printf("11. Sort Patient Records\n");
    printf("12. Save and Exit\n");
    printf("========================================\n");
}


/* MAIN FUNCTION */


int main(void) {
    int choice = 0;
    int running = 1;

    /* Initialize hospital doctors and beds */
    initializeDoctors();
    initializeBeds();

    printf("=======================================================================\n");
    printf("                             MEDIQUEUE                                 \n");
    printf("           Hospital Patient Management & Allocation System             \n");
    printf("=======================================================================\n");

    /* Startup Reconstruction - Load patient records from patients.txt */
    loadPatients();

    /* Main interaction loop */
    while (running) {
        displayMainMenu();
        choice = readInteger("Enter your choice (1-12): ", 1, 12);

        switch (choice) {
            case 1:
                registerPatientUI();
                break;
            case 2:
                displayPatients();
                break;
            case 3:
                searchPatientUI();
                break;
            case 4:
                updatePatientUI();
                break;
            case 5:
                displayQueues();
                break;
            case 6:
                processWaitingPatients();
                break;
            case 7:
                displayDoctors();
                break;
            case 8:
                displayBeds();
                break;
            case 9:
                dischargePatientUI();
                break;
            case 10:
                displayDashboard();
                break;
            case 11:
                sortPatientsByID();
                break;
            case 12:
                printf("\nSaving patient records to '%s'...\n", DATA_FILE);
                if (savePatients()) {
                    printf("All records saved successfully!\n");
                } else {
                    printf("Warning: Failed to save patient records!\n");
                }
                printf("Cleaning up memory...\n");
                freePatientList();
                printf("Thank you for using MediQueue. Exiting program...\n");
                running = 0;
                break;
            default:
                printf("\nInvalid selection. Please choose a valid option (1-12).\n");
                break;
        }
    }

    return 0;
}
