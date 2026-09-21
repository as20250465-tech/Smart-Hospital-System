\# Smart Hospital \& Resource Allocation System



CSC 1012 Introduction to Computer Programming - Individual Assignment 2026  

Faculty of Applied Sciences, University of Sri Jayewardenepura



\## Project Overview



A modular C-based hospital resource management system developed to handle patient registration, triage queue sorting, bed allocation, and automated billing calculation.



\## Core Features



\- \*\*Lookup Data\*\*: Doctor specialty fees and hospital ward bed rates using constant arrays

\- \*\*Patient Intake\*\*: Interactive registration with age-based discounts (15%) and emergency surcharges

\- \*\*Triage Priority Queue\*\*: Selection sort algorithm to prioritize critical patients (Level 3 -> Level 1)

\- \*\*Hospital Analytics\*\*: Revenue breakdown, discount tracking, and bed occupancy percentages

\- \*\*File Persistence\*\*: Saving bed availability status (`beds\_status.txt`) and appending billing logs (`patient\_records.txt`)



\## How to Compile \& Run



1\. Open the `Smart-Hospital-System` directory in Code::Blocks.

2\. Build and run `main.c` (Shortcut: `F9`).

3\. Alternatively, compile using GCC via CLI:



```bash

gcc main.c -o SmartHospitalSystem

./SmartHospitalSystem

