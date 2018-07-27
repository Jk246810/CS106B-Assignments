/*
 * File: DoctorsWithoutOrders.cpp
 * Author: Jamee Krzanich
 * ----------------------
 * This file contains the logic for seeing if the doctors can see all given patients
 */
#include "DoctorsWithoutOrders.h"
#include "map.h"
#include <climits>
#include <iostream>
using namespace std;

bool patientHelper(Vector<Doctor> &doctors,
                   Vector<Patient> &patients,
                   Map<string, Set<string>>& schedule);
/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(Vector<Doctor> &doctors,
                          Vector<Patient> &patients,
                          Map<string, Set<string>>& schedule) {

    (void)(doctors, patients, schedule);
    Vector<Patient> patientCopy = patients;
    return patientHelper(doctors, patientCopy, schedule);
}

/**
 * @brief patientHelper: recursive function that tries to schedule patients for certain doctors
 * @param doctors: The list of the doctors available to work.
 * @param patients: The list of the patients that need to be seen.
 * @param schedule: map of doctors with scheduled patients
 * @return returns true if all patients can be scheduled and false if not
 */

bool patientHelper(Vector<Doctor> &doctors,
                   Vector<Patient> &patients,
                   Map<string, Set<string>>& schedule){
    if(patients.isEmpty()){
        return true;
    }
    int patientTime = patients.get(0).hoursNeeded;
    Patient patient = patients.get(0);
    patients.remove(0);
    for(int i=0; i<doctors.size();i++){
        int doctorTime = doctors.get(i).hoursFree;
        if(doctorTime >= patientTime) {
            //choose
            doctors[i].hoursFree -= patientTime;
            //explore
            if(patientHelper(doctors, patients, schedule)){
                doctors[i].hoursFree += patientTime;
                Set<string> dPatients = schedule.get(doctors[i].name);
                dPatients.add(patient.name);
                schedule.put(doctors[i].name, dPatients);
                return true;
            }
            //unchoose
            doctors[i].hoursFree += patientTime;

        }
    }
    patients.insert(0, patient);
    return false;
}
