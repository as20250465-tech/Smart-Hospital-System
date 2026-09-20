#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS 20

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

int bedOccupancy[NUM_WARDS][MAX_BEDS] = {0};

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

    double base_fee;
    double surcharge;
    double ward_cost;
    double gross_total;
    double discount;
    double final_amount;
    double wait_time;
} Patient;

Patient patients[MAX_PATIENTS];
int patient_count = 0;

int specialtyQueueCount[NUM_SPECIALTIES] = {0};

void displayMenu();
void registerPatient();
void displayPriorityQueue();

int main()
{
    int choice;

    do {
        displayMenu();

        printf("Enter your choice (1-4): ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                registerPatient();
                break;

            case 2:
                displayPriorityQueue();
                break;

            case 3:
                printf("\n--- Analytics & Performance Reports ---\n");
                printf("Coming in the next step.\n");
                break;

            case 4:
                printf("\nExiting System. Thank you!\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 4);

    return 0;
}

void displayMenu()
{
    printf("\n========================================\n");
    printf("     SMART HOSPITAL ALLOCATION SYSTEM\n");
    printf("========================================\n");
    printf("1. Register New Patient & Calculate Bill\n");
    printf("2. View Priority Patient Queue\n");
    printf("3. View Analytics & Reports\n");
    printf("4. Exit\n");
    printf("----------------------------------------\n");
}

void registerPatient()
{
    if (patient_count >= MAX_PATIENTS) {
        printf("\nSystem capacity reached!\n");
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

    if (p.urgency_level < 1 || p.urgency_level > 3) {
        printf("Invalid urgency level. Setting to Normal.\n");
        p.urgency_level = 1;
    }

    printf("\nSelect Specialty ID:\n");

    for (int i = 0; i < NUM_SPECIALTIES; i++) {
        printf(
            " %d. %s (Fee: LKR %.2f, Avg Time: %d mins)\n",
            i + 1,
            SPECIALTY_NAMES[i],
            BASE_FEES[i],
            CONSULTATION_TIMES[i]
        );
    }

    printf("Enter Specialty ID (1-4): ");
    scanf("%d", &p.specialty_id);

    if (p.specialty_id < 1 || p.specialty_id > NUM_SPECIALTIES) {
        printf("Invalid specialty ID. Setting to General Practice.\n");
        p.specialty_id = 1;
    }

    int spec_idx = p.specialty_id - 1;

    p.wait_time =
        specialtyQueueCount[spec_idx] *
        CONSULTATION_TIMES[spec_idx];

    specialtyQueueCount[spec_idx]++;

    printf("\nIs Admitted to Ward? (1 = Yes, 0 = No): ");
    scanf("%d", &p.is_admitted);

    p.assigned_bed_no = 0;

    if (p.is_admitted == 1) {

        printf("\nSelect Ward ID:\n");

        for (int i = 0; i < NUM_WARDS; i++) {
            printf(
                " %d. %s (Daily Rate: LKR %.2f, Capacity: %d)\n",
                i + 1,
                WARD_NAMES[i],
                WARD_DAILY_RATES[i],
                WARD_CAPACITIES[i]
            );
        }

        printf("Enter Ward ID (1-4): ");
        scanf("%d", &p.ward_id);

        if (p.ward_id < 1 || p.ward_id > NUM_WARDS) {
            printf("Invalid ward ID. Setting to General Ward.\n");
            p.ward_id = 1;
        }

        printf("Enter Days Admitted: ");
        scanf("%d", &p.days_admitted);

        if (p.days_admitted < 1) {
            p.days_admitted = 1;
        }

        int ward_idx = p.ward_id - 1;

        for (int b = 0; b < WARD_CAPACITIES[ward_idx]; b++) {

            if (bedOccupancy[ward_idx][b] == 0) {
                bedOccupancy[ward_idx][b] = 1;
                p.assigned_bed_no = b + 1;
                break;
            }
        }

        if (p.assigned_bed_no == 0) {
            printf("\nSelected ward is full. Patient cannot be admitted.\n");
            return;
        }

    } else {
        p.is_admitted = 0;
        p.ward_id = 0;
        p.days_admitted = 0;
    }

    p.base_fee = BASE_FEES[spec_idx];

    if (p.urgency_level == 2) {
        p.surcharge = p.base_fee * 0.20;
    } else if (p.urgency_level == 3) {
        p.surcharge = p.base_fee * 0.50;
    } else {
        p.surcharge = 0.0;
    }

    if (p.is_admitted == 1) {
        p.ward_cost =
            p.days_admitted *
            WARD_DAILY_RATES[p.ward_id - 1];
    } else {
        p.ward_cost = 0.0;
    }

    p.gross_total =
        p.base_fee +
        p.surcharge +
        p.ward_cost;

    if (p.age < 5 || p.age > 65) {
        p.discount = p.gross_total * 0.15;
    } else {
        p.discount = 0.0;
    }

    p.final_amount =
        p.gross_total -
        p.discount;

    patients[patient_count] = p;
    patient_count++;

    printf("\n====================================================\n");
    printf("          SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------\n");

    printf("Patient ID           : PAT-%d\n", p.patient_id);
    printf("Patient Name         : %s\n", p.name);

    printf(
        "Age                  : %d Years %s\n",
        p.age,
        (p.age < 5 || p.age > 65)
        ? "(15%% Subsidy Eligible)"
        : ""
    );

    printf("Specialty            : %s\n",
           SPECIALTY_NAMES[spec_idx]);

    if (p.is_admitted == 1) {
        printf(
            "Assigned Ward        : %s (Bed #%02d)\n",
            WARD_NAMES[p.ward_id - 1],
            p.assigned_bed_no
        );
    } else {
        printf("Assigned Ward        : Outpatient (OPD)\n");
    }

    printf(
        "Urgency Level        : Level %d (%s)\n",
        p.urgency_level,
        p.urgency_level == 3
        ? "Critical"
        : (p.urgency_level == 2
           ? "Urgent"
           : "Normal")
    );

    printf("----------------------------------------------------\n");

    printf("Base Fee             : LKR %10.2f\n",
           p.base_fee);

    printf(
        "Emergency Surcharge : LKR %10.2f (%s)\n",
        p.surcharge,
        p.urgency_level == 3
        ? "50%"
        : (p.urgency_level == 2
           ? "20%"
           : "0%")
    );

    printf(
        "Ward Stay Cost       : LKR %10.2f (%d Days)\n",
        p.ward_cost,
        p.days_admitted
    );

    printf("----------------------------------------------------\n");

    printf("Gross Total Bill     : LKR %10.2f\n",
           p.gross_total);

    printf(
        "Age Subsidy Discount : LKR %10.2f\n",
        p.discount
    );

    printf("----------------------------------------------------\n");

    printf(
        "Final Payable Amount : LKR %10.2f\n",
        p.final_amount
    );

    printf(
        "Estimated Waiting Time : %.2f mins %s\n",
        p.wait_time,
        p.wait_time == 0
        ? "(Immediate Attention)"
        : ""
    );

    printf("====================================================\n");
}

void displayPriorityQueue()
{
    if (patient_count == 0) {
        printf("\nNo patients registered yet!\n");
        return;
    }

    Patient temp_list[MAX_PATIENTS];

    for (int i = 0; i < patient_count; i++) {
        temp_list[i] = patients[i];
    }

    for (int i = 0; i < patient_count - 1; i++) {

        int max_idx = i;

        for (int j = i + 1; j < patient_count; j++) {

            if (temp_list[j].urgency_level >
                temp_list[max_idx].urgency_level) {

                max_idx = j;
            }
        }

        if (max_idx != i) {
            Patient temp = temp_list[i];
            temp_list[i] = temp_list[max_idx];
            temp_list[max_idx] = temp;
        }
    }

    printf("\n==========================================================================\n");
    printf("                    PRIORITY PATIENT QUEUE\n");
    printf("==========================================================================\n");

    printf(
        "%-10s %-20s %-18s %-22s %-12s\n",
        "ID",
        "Name",
        "Urgency Level",
        "Specialty",
        "Est. Wait"
    );

    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < patient_count; i++) {

        char urg_str[20];

        if (temp_list[i].urgency_level == 3) {
            strcpy(urg_str, "Level 3 (Critical)");
        } else if (temp_list[i].urgency_level == 2) {
            strcpy(urg_str, "Level 2 (Urgent)");
        } else {
            strcpy(urg_str, "Level 1 (Normal)");
        }

        printf(
            "PAT-%-6d %-20s %-18s %-22s %.1f mins\n",
            temp_list[i].patient_id,
            temp_list[i].name,
            urg_str,
            SPECIALTY_NAMES[temp_list[i].specialty_id - 1],
            temp_list[i].wait_time
        );
    }

    printf("==========================================================================\n");
}
