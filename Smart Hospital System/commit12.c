#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS_PER_WARD 20

// Requirement 1: Pre-defined Lookup Tables
const char* SPECIALTY_NAMES[NUM_SPECIALTIES] = {
    "General Practice (OPD)",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

const double BASE_FEES[NUM_SPECIALTIES] = {
    1500.00, 2500.00, 4500.00, 5000.00
};

const int CONSULTATION_TIMES[NUM_SPECIALTIES] = {
    15, 20, 30, 30
};

const char* WARD_NAMES[NUM_WARDS] = {
    "General Ward",
    "Paediatric Ward",
    "Surgical Ward",
    "ICU (Intensive Care Unit)"
};

const double WARD_DAILY_RATES[NUM_WARDS] = {
    3000.00, 6000.00, 12000.00, 25000.00
};

const int WARD_CAPACITIES[NUM_WARDS] = {
    20, 10, 10, 5
};

// Requirement 1: 2D Bed Occupancy Matrix
int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD] = {0};

// Structure for Patient Record
typedef struct {
    int patient_id;
    char name[100];

    int age;
    int urgency_level;
    int specialty_id;

    int is_admitted;
    int ward_id;
    int days_admitted;
    int assigned_bed_no;

    // Billing Details
    double base_fee;
    double surcharge;
    double ward_cost;
    double gross_total;
    double discount;
    double final_amount;
    double wait_time;

} Patient;


// Global System Variables
Patient patients[MAX_PATIENTS];

int patient_count = 0;

int specialtyQueueCount[NUM_SPECIALTIES] = {0};


// Function Prototypes
void displayMenu();
void registerPatient();
void displayPriorityQueue();
void generateAnalytics();

void saveBedsStatus();
void loadBedsStatus();
void appendPatientToFile(Patient p);


int main()
{
    loadBedsStatus();

    int choice;

    do
    {
        displayMenu();

        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n'); // Clear invalid buffer

            printf("\nInvalid input! Please enter a number between 1 and 5.\n");

            continue;
        }


        switch (choice)
        {
            case 1:
                registerPatient();
                break;

            case 2:
                displayPriorityQueue();
                break;

            case 3:
                generateAnalytics();
                break;

            case 4:
                saveBedsStatus();

                printf("\nBed status successfully saved to 'beds_status.txt'!\n");

                break;

            case 5:
                saveBedsStatus();

                printf("\nExiting System. Thank you!\n");

                break;

            default:
                printf("Invalid choice! Please select 1-5.\n");
        }

    } while (choice != 5);


    return 0;
}


// Display Menu
void displayMenu()
{
    printf("\n========================================\n");
    printf("     SMART HOSPITAL ALLOCATION SYSTEM\n");
    printf("========================================\n");

    printf("1. Register New Patient & Calculate Bill\n");
    printf("2. View Priority Patient Queue (Triage)\n");
    printf("3. View Analytics & Performance Reports\n");
    printf("4. Save Bed Occupancy Status to File\n");
    printf("5. Exit\n");

    printf("----------------------------------------\n");
}


// Patient Registration
void registerPatient()
{
    // Input validation for Patient Age
    printf("Enter Patient Age: ");

    while (scanf("%d", &p.age) != 1 || p.age <= 0 || p.age > 120) {
        printf("Invalid age! Please enter a valid age (1-120): ");

        while (getchar() != '\n');  // Clear invalid input buffer
}

    // Input validation for Urgency Level
    printf("Emergency / Triage Level (1 = Normal, 2 = Urgent, 3 = Critical): ");

    while (scanf("%d", &p.urgency_level) != 1 ||p.urgency_level < 1 || p.urgency_level > 3) {

        printf("Invalid level! Enter 1 (Normal), 2 (Urgent), or 3 (Critical): ");

        while (getchar() != '\n');  // Clear invalid input buffer
}
    if (patient_count >= MAX_PATIENTS)
    {
        printf("\nSystem capacity reached! Cannot register more patients.\n");

        return;
    }


    Patient p;

    p.patient_id = 1001 + patient_count;


    printf("\n--- PATIENT INTAKE & REGISTRATION ---\n");


    printf("Enter Patient Name: ");

    getchar();

    fgets(p.name, sizeof(p.name), stdin);

    p.name[strcspn(p.name, "\n")] = '\0';


    printf("Enter Patient Age: ");

    scanf("%d", &p.age);


    printf("Emergency / Triage Level (1 = Normal, 2 = Urgent, 3 = Critical): ");

    scanf("%d", &p.urgency_level);


    if (p.urgency_level < 1 || p.urgency_level > 3)
    {
        p.urgency_level = 1;
    }


    printf("\nSelect Specialty ID:\n");


    for (int i = 0; i < NUM_SPECIALTIES; i++)
    {
        printf(" %d. %s (Fee: LKR %.2f, Avg Time: %d mins)\n",
               i + 1,
               SPECIALTY_NAMES[i],
               BASE_FEES[i],
               CONSULTATION_TIMES[i]);
    }


    printf("Enter Specialty ID (1-4): ");

    scanf("%d", &p.specialty_id);


    if (p.specialty_id < 1 ||
        p.specialty_id > NUM_SPECIALTIES)
    {
        p.specialty_id = 1;
    }


    int spec_idx = p.specialty_id - 1;


    // Requirement 3.1: Waiting Time
    p.wait_time =
        specialtyQueueCount[spec_idx] *
        CONSULTATION_TIMES[spec_idx];

    specialtyQueueCount[spec_idx]++;


    printf("\nIs Admitted to Ward? (1 = Yes, 0 = No): ");

    scanf("%d", &p.is_admitted);


    p.assigned_bed_no = 0;


    if (p.is_admitted == 1)
    {
        printf("\nSelect Ward ID:\n");


        for (int i = 0; i < NUM_WARDS; i++)
        {
            printf(" %d. %s (Daily Rate: LKR %.2f, Capacity: %d)\n",
                   i + 1,
                   WARD_NAMES[i],
                   WARD_DAILY_RATES[i],
                   WARD_CAPACITIES[i]);
        }


        printf("Enter Ward ID (1-4): ");

        scanf("%d", &p.ward_id);


        if (p.ward_id < 1 ||
            p.ward_id > NUM_WARDS)
        {
            p.ward_id = 1;
        }


        printf("Enter Days Admitted: ");

        scanf("%d", &p.days_admitted);


        int ward_idx = p.ward_id - 1;


        // Find Available Bed
        for (int b = 0;
             b < WARD_CAPACITIES[ward_idx];
             b++)
        {
            if (bedOccupancy[ward_idx][b] == 0)
            {
                bedOccupancy[ward_idx][b] = 1;

                p.assigned_bed_no = b + 1;

                break;
            }
        }
    }
    else
    {
        p.ward_id = 0;

        p.days_admitted = 0;
    }


    // Base Fee
    p.base_fee = BASE_FEES[spec_idx];


    // Emergency Surcharge
    if (p.urgency_level == 2)
    {
        p.surcharge = p.base_fee * 0.20;
    }
    else if (p.urgency_level == 3)
    {
        p.surcharge = p.base_fee * 0.50;
    }
    else
    {
        p.surcharge = 0.0;
    }


    // Ward Cost
    if (p.is_admitted == 1)
    {
        p.ward_cost =
            p.days_admitted *
            WARD_DAILY_RATES[p.ward_id - 1];
    }
    else
    {
        p.ward_cost = 0.0;
    }


    // Gross Total
    p.gross_total =
        p.base_fee +
        p.surcharge +
        p.ward_cost;


    // Age Subsidy
    if (p.age < 5 || p.age > 65)
    {
        p.discount = p.gross_total * 0.15;
    }
    else
    {
        p.discount = 0.0;
    }


    // Final Amount
    p.final_amount =
        p.gross_total -
        p.discount;


    patients[patient_count] = p;

    patient_count++;


    // Requirement 7:
    // Append patient billing record to file
    appendPatientToFile(p);


    // Receipt
    printf("\n====================================================\n");

    printf("          SMART HOSPITAL ADMISSION & BILL\n");

    printf("----------------------------------------------------\n");


    printf("Patient ID : PAT-%d\n",
           p.patient_id);


    printf("Patient Name : %s\n",
           p.name);


    printf("Age : %d Years %s\n",
           p.age,
           (p.age < 5 || p.age > 65)
           ? "(15%% Subsidy Eligible)"
           : "");


    printf("Specialty : %s\n",
           SPECIALTY_NAMES[spec_idx]);


    if (p.is_admitted == 1)
    {
        printf("Assigned Ward : %s (Bed #%02d)\n",
               WARD_NAMES[p.ward_id - 1],
               p.assigned_bed_no);
    }
    else
    {
        printf("Assigned Ward : Outpatient (OPD)\n");
    }


    printf("Urgency Level : Level %d (%s)\n",
           p.urgency_level,

           p.urgency_level == 3
           ? "Critical"
           : (p.urgency_level == 2
              ? "Urgent"
              : "Normal"));


    printf("----------------------------------------------------\n");


    printf("Base Fee : LKR %10.2f\n",
           p.base_fee);


    printf("Emergency Surcharge : LKR %10.2f (%s)\n",
           p.surcharge,

           p.urgency_level == 3
           ? "50%"
           : (p.urgency_level == 2
              ? "20%"
              : "0%"));


    printf("Ward Stay Cost : LKR %10.2f (%d Days)\n",
           p.ward_cost,
           p.days_admitted);


    printf("----------------------------------------------------\n");


    printf("Gross Total Bill : LKR %10.2f\n",
           p.gross_total);


    printf("Age Subsidy Discount : LKR %10.2f (15%%)\n",
           -p.discount);


    printf("----------------------------------------------------\n");


    printf("Final Payable Amount : LKR %10.2f\n",
           p.final_amount);


    printf("Estimated Waiting Time : %.2f mins %s\n",
           p.wait_time,

           p.wait_time == 0
           ? "(Immediate Attention)"
           : "");


    printf("====================================================\n");
}


// Requirement 4: Priority Queue
void displayPriorityQueue()
{
    if (patient_count == 0)
    {
        printf("\nNo patients registered yet!\n");

        return;
    }


    Patient temp_list[MAX_PATIENTS];


    // Copy Patient Records
    for (int i = 0;
         i < patient_count;
         i++)
    {
        temp_list[i] = patients[i];
    }


    // Selection Sort
    for (int i = 0;
         i < patient_count - 1;
         i++)
    {
        int max_idx = i;


        for (int j = i + 1;
             j < patient_count;
             j++)
        {
            if (temp_list[j].urgency_level >
                temp_list[max_idx].urgency_level)
            {
                max_idx = j;
            }
        }


        if (max_idx != i)
        {
            Patient temp = temp_list[i];

            temp_list[i] = temp_list[max_idx];

            temp_list[max_idx] = temp;
        }
    }


    printf("\n===============================================================================\n");

    printf("                    PRIORITY PATIENT QUEUE (TRIAGE SORT)\n");

    printf("===============================================================================\n");


    printf("%-10s %-20s %-16s %-22s %-12s\n",
           "ID",
           "Name",
           "Urgency Level",
           "Specialty",
           "Est. Wait");


    printf("-------------------------------------------------------------------------------\n");


    for (int i = 0;
         i < patient_count;
         i++)
    {
        char urg_str[25];


        if (temp_list[i].urgency_level == 3)
        {
            strcpy(urg_str, "Level 3 (Crit)");
        }
        else if (temp_list[i].urgency_level == 2)
        {
            strcpy(urg_str, "Level 2 (Urg)");
        }
        else
        {
            strcpy(urg_str, "Level 1 (Norm)");
        }


        printf("PAT-%-6d %-20s %-16s %-22s %.1f mins\n",
               temp_list[i].patient_id,
               temp_list[i].name,
               urg_str,
               SPECIALTY_NAMES[
                   temp_list[i].specialty_id - 1
               ],
               temp_list[i].wait_time);
    }


    printf("===============================================================================\n");
}


// Requirement 6: Performance Reports & Analytics
void generateAnalytics()
{
    if (patient_count == 0)
    {
        printf("\nNo patient data available to generate analytics.\n");

        return;
    }


    int count_l1 = 0;
    int count_l2 = 0;
    int count_l3 = 0;

    double total_revenue = 0.0;
    double total_discounts = 0.0;

    int highest_paying_idx = 0;


    for (int i = 0;
         i < patient_count;
         i++)
    {
        if (patients[i].urgency_level == 1)
        {
            count_l1++;
        }
        else if (patients[i].urgency_level == 2)
        {
            count_l2++;
        }
        else if (patients[i].urgency_level == 3)
        {
            count_l3++;
        }


        total_revenue +=
            patients[i].final_amount;


        total_discounts +=
            patients[i].discount;


        if (patients[i].final_amount >
            patients[highest_paying_idx].final_amount)
        {
            highest_paying_idx = i;
        }
    }


    printf("\n====================================================\n");

    printf("             PERFORMANCE REPORTS & ANALYTICS\n");

    printf("====================================================\n");


    printf("1. Patient Breakdown by Urgency:\n");


    printf(" - Level 1 (Normal) : %d patients\n",
           count_l1);


    printf(" - Level 2 (Urgent) : %d patients\n",
           count_l2);


    printf(" - Level 3 (Critical): %d patients\n",
           count_l3);


    printf(" - Total Registered : %d patients\n",
           patient_count);


    printf("----------------------------------------------------\n");


    printf("2. Financial Summary:\n");


    printf(" - Total Revenue Earned : LKR %.2f\n",
           total_revenue);


    printf(" - Total Discounts Given : LKR %.2f\n",
           total_discounts);


    printf("----------------------------------------------------\n");


    printf("3. Bed Occupancy Percentage per Ward:\n");


    for (int w = 0;
         w < NUM_WARDS;
         w++)
    {
        int occupied = 0;


        for (int b = 0;
             b < WARD_CAPACITIES[w];
             b++)
        {
            if (bedOccupancy[w][b] == 1)
            {
                occupied++;
            }
        }


        double percentage =
            ((double)occupied /
             WARD_CAPACITIES[w]) * 100.0;


        printf(" - %-25s : %d/%d Beds (%.1f%% Occupied)\n",
               WARD_NAMES[w],
               occupied,
               WARD_CAPACITIES[w],
               percentage);
    }


    printf("----------------------------------------------------\n");


    printf("4. Highest Paying Patient:\n");


    printf(" - Patient Name : %s (PAT-%d)\n",
           patients[highest_paying_idx].name,
           patients[highest_paying_idx].patient_id);


    printf(" - Final Bill : LKR %.2f\n",
           patients[highest_paying_idx].final_amount);


    printf("====================================================\n");
}


// ====================================================
// Requirement 7: FILE HANDLING FUNCTIONS
// ====================================================


// Save Bed Occupancy Status
void saveBedsStatus()
{
    FILE* fp;

    fp = fopen("beds_status.txt", "w");


    if (fp == NULL)
    {
        printf("Error: Could not open beds_status.txt\n");

        return;
    }


    for (int w = 0;
         w < NUM_WARDS;
         w++)
    {
        for (int b = 0;
             b < WARD_CAPACITIES[w];
             b++)
        {
            fprintf(fp,
                    "%d ",
                    bedOccupancy[w][b]);
        }

        fprintf(fp, "\n");
    }


    fclose(fp);
}


// Load Bed Occupancy Status
void loadBedsStatus()
{
    FILE* fp;

    fp = fopen("beds_status.txt", "r");


    if (fp == NULL)
    {
        return;
    }


    for (int w = 0;
         w < NUM_WARDS;
         w++)
    {
        for (int b = 0;
             b < WARD_CAPACITIES[w];
             b++)
        {
            if (fscanf(fp,
                       "%d",
                       &bedOccupancy[w][b]) != 1)
            {
                bedOccupancy[w][b] = 0;
            }
        }
    }


    fclose(fp);
}


// Append Patient Record to File
void appendPatientToFile(Patient p)
{
    FILE* fp;

    fp = fopen("patient_records.txt", "a");


    if (fp == NULL)
    {
        printf("Error: Could not open patient_records.txt\n");

        return;
    }


    fprintf(fp,
            "PAT-%d | Name: %s | Age: %d | Level: %d | Bill: LKR %.2f\n",
            p.patient_id,
            p.name,
            p.age,
            p.urgency_level,
            p.final_amount);


    fclose(fp);
}
