/*
Answer for homework 8.26, p.529
Author: Qingchao Kong
*/

/* $begin shellmain */
#include "csapp.h"
#define MAXARGS 128
#define MAXJOBS 100 // max jobs

// state code description
#define RUNNING 0
#define STOPPED 1
#define INTERRUPTED 2
#define NORMALEXIT 3
#define TERMINATED 4

/* function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

struct Job
{
    int JID;               // job id
    int PID;               // process id
    int PGID;              // process group id
    int state_code;        // 1 for running, 0 for stopped
    int bg;                // background or not
    char cmdline[MAXLINE]; // argument list for this job
    char **argv;           // the parsed argv
};

struct Job job_list[MAXJOBS];
int jobcount = 0;

/*
    front_jid takes two values:
    1) job id: currently running front job ID
    2) 0: the shell
*/
int front_jid; // current front process index of array job_list

char *get_state_string(int state_code)
{
    switch (state_code)
    {
    case RUNNING:
        return "Running";
    case STOPPED:
        return "Stopped";
    case INTERRUPTED:
        return "Interrupt";
    case NORMALEXIT:
        return "Normal Exit";
    case TERMINATED:
        return "Terminated";
    default:
        return "(Unknown status code)";
    }
}

/*
    Check status state
    @jid: front job id
*/
void check_process_state()
{
    printf("Check process status:\n");
    int i;
    for (i = 1; i < jobcount; i++)
    {
        int status;
        struct Job *pjob = &(job_list[i]);
        int pid = pjob->PID;
        int ret;
        // only check jobs which is previously running, backgroud or front jobs
        if (pjob->state_code != RUNNING)
            continue;
        if ((ret = waitpid(pid, &status, WNOHANG | WUNTRACED)) < 0)
        {
            printf("Error when waiting pid: %d\n", pid);
            unix_error("waitfg: waitpid error");
            continue;
        }
        // bg process is still running
        if (ret == 0)
            continue;

        if (WIFEXITED(status))
        {
            pjob->state_code = NORMALEXIT;
            printf("Job %d normal exit: Normalexit\n", pid);
        }
        else if (WIFSIGNALED(status))
        {
            // the process can be terminated or have been previously interrupted
            // if it is terminated, it must be running just now
            pjob->state_code = TERMINATED;
            printf("Job %d terminated by signal: Terminated\n", pid);
        }
        else
        {
            printf("Impossilbe process state! Error!");
        }
    }
}

/*
    When child process exitted or stopped, parent process receive SIGCHLD signal
*/
void handler_process_exit(int sig)
{
    //check_process_state();
}

/*
    user type ctrl-c or ctrl-z
*/
void handler_keyboard(int sig)
{
    if (front_jid == 0)
    {
        // the shell, simply exit
        exit(0);
    }
    else
    {
        // currently a child process is running front
        int cpid = job_list[front_jid].PID;
        if (sig == SIGINT)
        {
            //printf("PID: %d, SIGINT\n", getpid());
            //Kill(cpid, SIGUSR1);
            Kill(cpid, SIGINT);
            job_list[front_jid].state_code = INTERRUPTED;
            //printf("Job %d terminated by signal: Interrupt\n", cpid);
        }
        else if (sig == SIGTSTP)
        {
            //printf("PID: %d SIGTSTP\n", getpid());
            Kill(cpid, SIGTSTP);
            job_list[front_jid].state_code = STOPPED;
            //printf("Job [%d] %d stopped by signal: Stopped\n", front_jid, cpid);
        }
    }
}

/*
    when child process receives ctrl-c, make it appear it normally exited , but with 
    different exit code.
    This is used to differiate between SIGTERM and SIGINT, both of which belongs to uncaught signal
*/
/*
void handler_user1(int sig){
    printf("handler_user1\n");
    //exit(INTERRUPTED);
}
void handler_nothing(int sig){
    printf("Do nothing and return.\n");
}
*/

int main()
{
    char cmdline[MAXLINE]; /* Command line */

    struct Job job;
    job.JID = jobcount;
    job.PID = getpid();
    job.PGID = getpgrp();
    job.state_code = 0;
    strcpy(job.cmdline, "Root");
    job.argv = NULL;

    job_list[jobcount++] = job;

    front_jid = 0; // set root process as the current job

    Signal(SIGINT, handler_keyboard);
    Signal(SIGTSTP, handler_keyboard);
    Signal(SIGCHLD, handler_process_exit);

    printf("Shell pid: %d\n", getpid());

    while (1)
    {
        /* Read */
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);

        /* Evaluate */
        eval(cmdline);

        // check every child process when executing one cmd
        check_process_state();
    }
}
/* $end shellmain */

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return; /* Ignore empty lines */

    if (!builtin_command(argv))
    {
        if ((pid = Fork()) == 0)
        { /* Child runs user job */
            // set process group id
            setpgid(getpid(), getpid());
            printf("Child process group id: %d, PID: %d\n", getpgrp(), getpid());

            // when running execve, all signal settings have been reset to default
            if (execve(argv[0], argv, environ) < 0)
            {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        // parent process
        // record child process info
        struct Job job;
        job.JID = jobcount;
        job.PID = pid;
        job.PGID = pid;
        job.state_code = 0;
        job.bg = bg;
        strcpy(job.cmdline, cmdline);
        job.argv = argv;

        job_list[jobcount++] = job;

        /* Parent waits for foreground job to terminate */
        if (!bg)
        {
            front_jid = jobcount - 1; // set the current job index

            // shell is blocked, waiting child process to exit or interrupted
            int status;
            if (waitpid(pid, &status, WUNTRACED) < 0)
                unix_error("waitfg: waitpid error");
            // if normal exit,then change its state
            if (WIFEXITED(status))
            {
                job_list[front_jid].state_code = NORMALEXIT;
            }
        }
        else
            printf("[%d] %d %s \t %s", job.JID, job.PID, get_state_string(job.state_code), job.cmdline);

        front_jid = 0;
    }

    return;
}

/*
    Input: %jid or pid string
    Output: jid
    Return: if jid or pid is invalid, return -1
*/
int find_jid(const char *ids)
{
    int jid;
    int pid;

    if (ids[0] == '%')
    {
        jid = atoi(ids + 1);
        printf("JID: %d\n", jid);

        if (jid <= 0 || jid >= jobcount)
            return -1;
        if (job_list[jid].state_code != STOPPED)
            return -1;

        return jid;
    }
    else
    {
        pid = atoi(ids);
        int i;
        for (i = 1; i < jobcount; i++)
        {
            struct Job job = job_list[i];
            if (job.PID == pid && job.state_code == STOPPED)
                return i;
        }

        return -1;
    }
}

void print_job_info(int jid)
{
    struct Job job = job_list[jid];
    printf("[%d] %d %s \t %s\n", job.JID, job.PID, get_state_string(job.state_code), job.cmdline);
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "&")) /* Ignore singleton & */
        return 1;

    // command: jobs
    if (!strcmp(argv[0], "jobs"))
    {
        int i;
        for (i = 0; i < jobcount; ++i)
        {
            print_job_info(i);
        }

        return 1;
    }

    if (!strcmp(argv[0], "bg"))
    {
        int jid = find_jid(argv[1]);
        if (jid < 0)
        {
            printf("%s: No such process\n", argv[1]);
            return 1;
        }

        job_list[jid].bg = 1;
        job_list[jid].state_code = RUNNING;

        Kill(job_list[jid].PID, SIGCONT);

        printf("Job continued: ");
        print_job_info(jid);
        return 1;
    }

    if (!strcmp(argv[0], "fg"))
    {
        int jid = find_jid(argv[1]);
        if (jid < 0)
        {
            printf("%s: No such process\n", argv[1]);
            return 1;
        }

        job_list[jid].bg = 0;
        job_list[jid].state_code = RUNNING;
        front_jid = jid;

        Kill(job_list[jid].PID, SIGCONT);

        printf("Job continued: ");
        print_job_info(jid);
        int status;
        if (waitpid(job_list[jid].PID, &status, WUNTRACED) < 0)
            unix_error("waitfg: waitpid error");

        return 1;
    }

    return 0; /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv)
{
    char *delim; /* Points to first space delimiter */
    int argc;    /* Number of args */
    int bg;      /* Background job? */

    buf[strlen(buf) - 1] = ' ';   /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' ')))
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0) /* Ignore blank line */
        return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}