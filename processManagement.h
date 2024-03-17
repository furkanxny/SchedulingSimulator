/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   processManagement.h
 * Author: ubuntu
 *
 * Created on March 16, 2024, 10:58 PM
 */

#ifndef PROCESSMANAGEMENT_H
#define PROCESSMANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

    
       /* From xv6 */
    typedef enum procState {
                UNUSED,
                EMBRYO,     /* still being set up */
                SLEEPING,   /* What your book calls "blocked" on Ch. 4 p.6 */
                BLOCKED = SLEEPING,
                RUNNABLE,   /* What your book calls "ready" on Ch. 4 p.6 */
                READY = RUNNABLE,
                RUNNING,    
                ZOMBIE      /* KILLED, but not yet cleaned up */
    } processState ;
    
#define NPROC 64 /* Maximum number of concurrent processes */
    
    typedef struct {
        int processID;
        int parentID;
        int waitingFor;
        //int childArry[10];
        processState state;
        /* you should not use global variables. Therefore 
         * you use this structure to retain any context/state your
         * simulator needs between calls. Please add anything you need, and
         * the test code will pass this to your functions
         */
    } processStructure;
    
    
    
    
    /* function prototypes */
    /* processScheduleInitialize()
     * Initialize the data structure
     */
    void processScheduleInitialize(processStructure*);
    /* processAdd()
     * Adds a new process. Returns the process id
     * or 0 if impossible
     */
    int processAdd(processStructure*, int parent);
    /* processTimerInterrupt()
     * Tells the system that an interrupt occurred. 
     * Returns the PID of the new running process
     * or -1 if none are running
     */
    int processTimerInterrupt(processStructure* ps);
    /* processIORequest()
     * Indicates that the current process has requested IO and will
     * block until it completes (see below) 
     * Returns 0 if OK, -1 otherwise.
     */
    int processIORequest(processStructure*);
    /* processIODDone()
     * Indicates that the prior request for IO By the process 
     * indicated has completed. 
     */
    int processIODone(processStructure*, int processID);
    /* processWaitFor()
     * The current process is requesting to wait for the listed 
     * process.(aka waitPid()). Returns 0 if ok, -1 unable 
     * (e.g., no such process 
     */
    
    int processWaitFor(processStructure*, int processID);
    /* processEXIT
     * Indicates that the indicated process has exited. Make sure to 
     * clean up and to kill all descendant processes. 
     * Returns 0 for success, -1 otherwise
     */
    int processExit(processStructure*, int processID);
    /* processGetRunningPID()
     * Returns the PID of the currently running process. 
     */
    int processGetRunningPID(processStructure*);
    /* returns UNUSED if no such process */
    processState getProcessStatus(processStructure*, int processID);
    






#ifdef __cplusplus
}
#endif

#endif /* PROCESSMANAGEMENT_H */

