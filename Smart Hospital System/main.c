#include <stdio.h>
#include <string.h>

// Maximum limits
#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS 20

// Pre-defined Lookup Tables using Constant Arrays
const char* SPECIALTY_NAMES[NUM_SPECIALTIES] = {
    "General Practice (OPD)",
    "Paediatrics",
    "Cardiology",
    "Neurology"
};

const double BASE_FEES[NUM_SPECIALTIES] = {
    1500.00,
    2500.00,
    4500.00,
    5000.00
};

const int CONSULTATION_TIMES[NUM_SPECIALTIES] = {
    15,
    20,
    30,
    30
};

const char* WARD_NAMES[NUM_WARDS] = {
    "General Ward",
    "Paediatric Ward",
    "Surgical Ward",
    "ICU (Intensive Care Unit)"
};

const double WARD_DAILY_RATES[NUM_WARDS] = {
    3000.00,
    6000.00,
    12000.00,
    25000.00
};

const int WARD_CAPACITIES[NUM_WARDS] = {
    20,
    10,
    10,
    5
};

// 2D Bed Occupancy Matrix
// 0 = Available
// 1 = Occupied
int bedOccupancy[NUM_WARDS][MAX_BEDS] = {0};

// Queue counter per specialty
int specialtyQueueCount[NUM_SPECIALTIES] = {0};

// Function prototypes
void displayMenu();
void registerPatient();


// ======================================================
// MAIN FUNCTION
// ======================================================

int main()
{
    int choice;

    do
    {
        displayMenu();

        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                registerPatient();
                break;

            case 2:
                printf("\n--- Sorted Patient Queue (Coming in Next Step) ---\n");
                break;

            case 3:
                printf("\n--- Analytics & Performance Reports (Coming Soon) ---\n");
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


// ======================================================
// DISPLAY MENU
// ======================================================

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


// ======================================================
// REGISTER PATIENT
// ======================================================

void registerPatient()
{
    char name[100];

    int age;
    int urgency_level;
    int specialty_id;
    int is_admitted;

    int ward_id = 0;
    int days_admitted = 0;

    static int patient_counter = 1001;

    int assigned_bed_no = 0;


    // --------------------------------------------------
    // PATIENT INTAKE
    // --------------------------------------------------

    printf("\n--- PATIENT INTAKE & REGISTRATION ---\n");

    printf("Enter Patient Name: ");

    // Clear input buffer
    getchar();

    fgets(name, sizeof(name), stdin);

    // Remove newline from name
    name[strcspn(name, "\n")] = '\0';


    // --------------------------------------------------
    // AGE
    // --------------------------------------------------

    printf("Enter Patient Age: ");
    scanf("%d", &age);


    // --------------------------------------------------
    // URGENCY LEVEL
    // --------------------------------------------------

    printf("\nEmergency / Triage Level\n");
    printf("1 = Normal\n");
    printf("2 = Urgent\n");
    printf("3 = Critical\n");

    printf("Enter Urgency Level: ");
    scanf("%d", &urgency_level);


    // Validate urgency level
    if (urgency_level < 1 || urgency_level > 3)
    {
        printf("Invalid urgency level!\n");
        return;
    }


    // --------------------------------------------------
    // SPECIALTY SELECTION
    // --------------------------------------------------

    printf("\nSelect Specialty ID:\n");

    for (int i = 0; i < NUM_SPECIALTIES; i++)
    {
        printf(
            " %d. %s (Fee: LKR %.2f, Avg Time: %d mins)\n",
            i + 1,
            SPECIALTY_NAMES[i],
            BASE_FEES[i],
            CONSULTATION_TIMES[i]
        );
    }

    printf("Enter Specialty ID (1-4): ");
    scanf("%d", &specialty_id);


    // Validate specialty ID
    if (specialty_id < 1 || specialty_id > NUM_SPECIALTIES)
    {
        printf("Invalid specialty ID!\n");
        return;
    }


    // Convert specialty ID to array index
    int spec_idx = specialty_id - 1;


    // --------------------------------------------------
    // REQUIREMENT 3.1
    // WAITING TIME CALCULATION
    // --------------------------------------------------

    double wait_time =
        specialtyQueueCount[spec_idx] *
        CONSULTATION_TIMES[spec_idx];

    // Increase queue count
    specialtyQueueCount[spec_idx]++;


    // --------------------------------------------------
    // WARD ADMISSION
    // --------------------------------------------------

    printf("\nIs Admitted to Ward? (1 = Yes, 0 = No): ");
    scanf("%d", &is_admitted);


    if (is_admitted == 1)
    {
        // --------------------------------------------------
        // SELECT WARD
        // --------------------------------------------------

        printf("\nSelect Ward ID:\n");

        for (int i = 0; i < NUM_WARDS; i++)
        {
            printf(
                " %d. %s (Daily Rate: LKR %.2f, Capacity: %d)\n",
                i + 1,
                WARD_NAMES[i],
                WARD_DAILY_RATES[i],
                WARD_CAPACITIES[i]
            );
        }

        printf("Enter Ward ID (1-4): ");
        scanf("%d", &ward_id);


        // Validate ward ID
        if (ward_id < 1 || ward_id > NUM_WARDS)
        {
            printf("Invalid ward ID!\n");
            return;
        }


        printf("Enter Days Admitted: ");
        scanf("%d", &days_admitted);


        if (days_admitted <= 0)
        {
            printf("Invalid number of days!\n");
            return;
        }


        // --------------------------------------------------
        // ASSIGN FIRST AVAILABLE BED
        // --------------------------------------------------

        int ward_idx = ward_id - 1;

        for (int b = 0;
             b < WARD_CAPACITIES[ward_idx];
             b++)
        {
            if (bedOccupancy[ward_idx][b] == 0)
            {
                bedOccupancy[ward_idx][b] = 1;

                assigned_bed_no = b + 1;

                break;
            }
        }


        // Check if ward is full
        if (assigned_bed_no == 0)
        {
            printf("\nSorry! The selected ward is full.\n");
            return;
        }
    }


    // --------------------------------------------------
    // BILLING CALCULATIONS
    // --------------------------------------------------

    double base_fee = BASE_FEES[spec_idx];


    // --------------------------------------------------
    // REQUIREMENT 3.2
    // EMERGENCY SURCHARGE
    // --------------------------------------------------

    double surcharge = 0.0;

    if (urgency_level == 2)
    {
        surcharge = base_fee * 0.20;
    }
    else if (urgency_level == 3)
    {
        surcharge = base_fee * 0.50;
    }


    // --------------------------------------------------
    // REQUIREMENT 3.3
    // TOTAL WARD STAY COST
    // --------------------------------------------------

    double ward_cost = 0.0;

    if (is_admitted == 1)
    {
        ward_cost =
            days_admitted *
            WARD_DAILY_RATES[ward_id - 1];
    }


    // --------------------------------------------------
    // REQUIREMENT 3.4
    // GROSS TOTAL BILL
    // --------------------------------------------------

    double gross_total =
        base_fee +
        surcharge +
        ward_cost;


    // --------------------------------------------------
    // REQUIREMENT 3.5
    // AGE SUBSIDY DISCOUNT
    // 15% if Age < 5 or Age > 65
    // --------------------------------------------------

    double discount = 0.0;

    if (age < 5 || age > 65)
    {
        discount = gross_total * 0.15;
    }


    // --------------------------------------------------
    // REQUIREMENT 3.6
    // FINAL AMOUNT PAYABLE
    // --------------------------------------------------

    double final_amount =
        gross_total -
        discount;


    // --------------------------------------------------
    // REQUIREMENT 5
    // FORMATTED RECEIPT
    // --------------------------------------------------

    printf("\n====================================================\n");
    printf("        SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------\n");

    printf("Patient ID       : PAT-%d\n", patient_counter++);

    printf("Patient Name     : %s\n", name);

    printf(
        "Age              : %d Years %s\n",
        age,
        (age < 5 || age > 65)
        ? "(15% Subsidy Eligible)"
        : ""
    );

    printf(
        "Specialty        : %s\n",
        SPECIALTY_NAMES[spec_idx]
    );


    // Ward information
    if (is_admitted == 1)
    {
        printf(
            "Assigned Ward   : %s (Bed #%02d)\n",
            WARD_NAMES[ward_id - 1],
            assigned_bed_no
        );
    }
    else
    {
        printf(
            "Assigned Ward   : Outpatient (OPD)\n"
        );
    }


    // Urgency information
    printf(
        "Urgency Level    : Level %d (%s)\n",
        urgency_level,
        urgency_level == 3
        ? "Critical"
        : (urgency_level == 2
           ? "Urgent"
           : "Normal")
    );


    printf("----------------------------------------------------\n");


    // Billing details
    printf(
        "Base Fee         : LKR %10.2f\n",
        base_fee
    );

    printf(
        "Emergency Surcharge : LKR %10.2f (%s)\n",
        surcharge,
        urgency_level == 3
        ? "50%"
        : (urgency_level == 2
           ? "20%"
           : "0%")
    );

    printf(
        "Ward Stay Cost   : LKR %10.2f (%d Days)\n",
        ward_cost,
        days_admitted
    );


    printf("----------------------------------------------------\n");


    printf(
        "Gross Total Bill : LKR %10.2f\n",
        gross_total
    );

    printf(
        "Age Subsidy Discount : LKR %10.2f (15%%)\n",
        -discount
    );


    printf("----------------------------------------------------\n");


    printf(
        "Final Payable Amount : LKR %10.2f\n",
        final_amount
    );

    printf(
        "Estimated Waiting Time : %.2f mins %s\n",
        wait_time,
        wait_time == 0
        ? "(Immediate Attention)"
        : ""
    );


    printf("====================================================\n");
}
