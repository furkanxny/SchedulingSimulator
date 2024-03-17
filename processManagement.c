/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.c to edit this template
 */

#include "processManagement.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

processStructure processArry[NPROC];
int PID = 100;
int lastRunningProcessIndex = -1;
int counter = 0;


void markProcessAndDescendantsAsZombie(int processID) {
    for(int i = 0; i < NPROC; i++) {
        if(processArry[i].processID == processID) {
            processArry[i].state = ZOMBIE;
            for(int j = 0; j < NPROC; j++) {
                if(processArry[j].parentID == processID) {
                    markProcessAndDescendantsAsZombie(processArry[j].processID);
                }
            }
            break;
        }
    }
}


void scheduleNextProcess() {
    int found = -1;
    int startIndex = (lastRunningProcessIndex + 1) % NPROC;
    for (int i = 0; i < NPROC; i++) {
        int currentIndex = (startIndex + i) % NPROC;
        if (processArry[currentIndex].state == RUNNABLE) {
            processArry[currentIndex].state = RUNNING;
            lastRunningProcessIndex = currentIndex;
            found = 0;
            break;
        }
    }
    if (found == -1) {
        exit;
    }
}

int processTimerInterrupt(processStructure* ps) {
    int runningProcessID = 0;
    int indexRunning = 0;
    for(int i = 0; i < NPROC; i++){
        if(processArry[i].state == RUNNING){
            processArry[i].state = RUNNABLE;
            runningProcessID = processArry[i].processID;
            indexRunning = i;
            break;
        }
    }
    
    int startIndex = (lastRunningProcessIndex + 1) % NPROC;
    int returnID = -1;
    
    for(int i = 0; i < NPROC; i++){
        int currentIndex = (startIndex + i) % NPROC;
        if(processArry[currentIndex].state == RUNNABLE && currentIndex != indexRunning){
            processArry[currentIndex].state = RUNNING;
            lastRunningProcessIndex = currentIndex;
            returnID = processArry[currentIndex].processID;
            break;
        }
    }
    
    if(returnID == -1){
        processArry[indexRunning].state = RUNNING;
        returnID = processArry[indexRunning].processID;
    }
    
    return returnID;
}
    
void processScheduleInitialize(processStructure* ps) {
    if (ps != NULL) {
        ps->processID = -1;
        ps->parentID = -1;
        ps->state = UNUSED;
        ps->waitingFor = -1;
    }
}

int processAdd(processStructure* ps, int parent) {
    if (PID < NPROC + 100) {
        ps->state = (parent == 0) ? RUNNING : RUNNABLE;
        ps->processID = PID++;
        ps->parentID = parent;
        if (counter < NPROC) {
            processArry[counter++] = *ps;
            return ps->processID;
        }
    }
    return -1;
}


int processGetRunningPID(processStructure* ps) {
    for(int i = 0; i < NPROC; i++) {    
        if(processArry[i].state == RUNNING) {
            return processArry[i].processID;
        }
    }
    return -1;
}

int processIORequest(processStructure* ps){
    for(int i = 0; i < NPROC; i++) {
        if(processArry[i].state == RUNNING){
            processArry[i].state = BLOCKED;
            scheduleNextProcess();
            return 0;
        }
    }
    return -1;
}

int processIODone(processStructure* ps, int processID){
    for(int i = 0; i < NPROC; i ++){
        if(processArry[i].processID == processID && processArry[i].state == BLOCKED){
            processArry[i].state = READY;
            return 0;
        }
    }
    return -1;
}

int processWaitFor(processStructure* ps, int processID){
    int isOK = -1;
    int waitingforID = -1;
    
    for(int i = 0; i < NPROC; i++){
        if(processArry[i].state == RUNNING){
            waitingforID = processArry[i].processID;
            break;
        }
    }
    
    for(int i = 0; i < NPROC; i++){
        if(processArry[i].processID == processID && processArry[i].state != ZOMBIE){
            if(waitingforID != -1){
                processArry[i].waitingFor = waitingforID;
            }
            isOK = 0;
            break;
        }
    }
    
    int currentRunningID = -1;
    int currentRunning = -1;
    if(isOK == 0){
       for(int i = 0; i < NPROC; i++){
           if(processArry[i].state == RUNNING){
               processArry[i].state = BLOCKED;
               currentRunningID = processArry[i].processID;
               currentRunning = i;
               break;
           }
       }
    }
    
    scheduleNextProcess();
    
    return isOK;
}


int processExit(processStructure* ps, int processID) {
    for(int i = 0; i < NPROC; i++){
        if(processArry[i].processID == processID){
            markProcessAndDescendantsAsZombie(processID);
            if(processArry[i].waitingFor != -1){
                processArry[processArry[i].waitingFor - 100].state = READY;
                processArry[i].waitingFor = -1;
            }
            scheduleNextProcess();
            return 0;
        }
    }
    return -1;
}

processState getProcessStatus(processStructure* ps, int processID) {
    for (int i = 0; i < NPROC; i++) {
        if (processArry[i].processID == processID) {
            return processArry[i].state;
        }
    }
    return UNUSED;
}