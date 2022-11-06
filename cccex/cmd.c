#include "csapp.h"

#define MAXARGS 128
#define MAXJOBS 100

// state code description
#define RUNNING 0
#define STOPPED 1
#define INTERRUPTED 2
#define NORMALEXIT 3
#define TERMINATED 4

struct Job
{
    int situation;         //state code
    int bg;                //is on background
    char **argv;           //pointer to pointers of the argv
    char cmdline[MAXLINE]; //original cmdline
    int jid;               //job id = the index where it is putted into the array
    int pid;               //given by the gcc
    int group_pid;         //given by gcc in case is in background to be the asme as the id via setpid
};

int parseline(char *buf, char **argv);
int builtin_command(char **argv);

void eval(char *cmdline);
void list_all_background_jobs();
void restartJobBG(char *pid);
void restartJobFG(char *pid);
int find_jid(char *arr);
void handler_stop(pid_t pid);

struct Job job_list[MAXJOBS];
int total_jobs = 0;
int foreground_process = 0; //index of the process running in the foreground

int main()
{

    struct Job job;
    job.jid = total_jobs;
    job.pid = getpid();
    job.group_pid = getpgrp();
    job.situation = 0;
    strcpy(job.cmdline, "Root");
    job.argv = NULL;
    job_list[total_jobs++] = job;
    foreground_process = 0; // set root process as the current job

    char cmdline[MAXLINE];

    if (signal(SIGSTOP, handler_stop) < 0){
        printf("Error while registering the signal");
        exit(0);
    }

        while (1)
        {
            printf("> ");
            Fgets(cmdline, MAXLINE, stdin);
            if (feof(stdin))
            {
                exit(0);
            }
            eval(cmdline);
        }
}

void eval(char *cmdline)
{
    //to parse the line first
    pid_t pid;
    char *argv[MAXLINE];
    char buf[MAXLINE];
    strcpy(buf, cmdline);
    char *abc = cmdline;
    int bg = 0; //means it should be runned in the background

    bg = parseline(buf, argv);

    if (argv[0] == NULL)
    {
        return;
    }

    if (!builtin_command(argv))
    {
        if ((pid = fork()) == 0)
        { //to be run in the background
            setpgid(0, 0);
            printf("Child process group id: %d, PID: %d\n", getpgrp(), getpid());
            while (execve(argv[0], argv, environ) < 0)
            {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }
    }

    //add the pid to the array;

    struct Job job;
    job.jid = total_jobs;
    job.pid = pid;
    job.group_pid = pid;
    strcpy(job.cmdline, cmdline);
    job.argv = argv;
    job.bg = bg;
    job.situation = RUNNING;

    job_list[total_jobs++] = job;
    int status;

    if (bg == 0)
    { //must wait for actual process
        foreground_process = total_jobs - 1;
        while (waitpid(pid, &status, WUNTRACED) > 0)
        {
            if (WIFEXITED(status))
            {
                job.situation = NORMALEXIT;
            }
        }
    }
    else
    {
        printf("Running in the background pid %d with index_nr %d", pid, --total_jobs);
        total_jobs += 1;
        foreground_process = 0;
    }
}

int find_jid(char *arr)
{
    int number;
    int is_jid = 0;

    if (arr[0] == '%')
    {
        number = atoi(arr[1]);
        return number;
    }
    else
    {
        number = atoi(arr[0]);
        for (int i = 0; i < total_jobs; i++)
        {
            struct Job search_job = job_list[i];
            if (search_job.pid == number)
            {
                return i;
            }
        }
    }
    return -1;
}

void restartJobBG(char *pid)
{
    int number = find_jid(pid);
    if (number < 0)
    {
        return;
    }
    //means foreground means that the grpid must be changed
    if (job_list[number].bg == 0)
    {
        setpgid(job_list[number].pid, job_list[number].pid);
        job_list[number].group_pid = job_list[number].pid;
    }
    job_list[number].bg = 1;
    job_list[number].situation = RUNNING;
    kill(job_list[number].pid, SIGCONT);
}

void restartJobFG(char *pid)
{
    int status;
    int number = find_jid(pid);
    if (number < 0)
    {
        return;
    }

    if (job_list[number].bg == 1)
    {
        setpgid(job_list[number].pid, job_list[0].group_pid);
        job_list[number].group_pid = job_list[0].group_pid;
    }
    job_list[number].bg = 0;
    foreground_process = number;
    kill(job_list[number].pid, SIGCONT);
    job_list[number].situation = RUNNING;
    if (waitpid(job_list[number].pid, &status, WUNTRACED) < -1)
    {
        printf("fucking error has been catched");
    }
}
//1 means bg , 0 means fg
int parseline(char *buf, char **argv)
{

    int arg_nr = 0;
    char *occurence;
    buf[strlen(buf) - 1] == ' ';

    while (*buf && (*buf == ' '))
    {
        buf++;
    }
    while ((occurence = strchr(*buf, ' ')) != NULL)
    {

        *occurence = "/0";
        argv[arg_nr++] = buf;
        buf = occurence + 1;

        while (*buf && (*buf == ' '))
        {
            buf++;
        }
    }
    if (arg_nr == 0)
    {
        return 0;
    }
    argv[arg_nr] = NULL;
    if (argv[--arg_nr] == '&')
    {
        return 1;
    }
    return 0;
}

//return 1 built in command
//return 0 not build in command
int builtin_command(char **argv)
{
    char *check = argv;

    if (strcmp(check, "quit"))
    {
        exit(0);
    }
    else if (strcmp(check, "jobs"))
    {
        list_all_background_jobs();
    }
    else if (strcmp(check, "bg"))
    {
        restartJobBG(argv[1]);
    }
    else if (strcmp(check, "fg"))
    {
        restartJobFG(argv[1]);
    }
    else //run built in command with following arguments
    {
        return 0;
    }
    return 1;
}

void list_all_background_jobs()
{

    for (int i = 1; i < total_jobs; i++)
    {
        struct Job job = job_list[i];
        printf("%d,%d,state:%s,cmd:%s \n", job.pid, job.jid, get_situation(job.situation), job.cmdline);
    }
}

char *get_situation(int situation)
{
    switch (situation)
    {
    case RUNNING:
        /* code */
        return "Running";
    case STOPPED:
        /* code */
        return "Stopped";
    case INTERRUPTED:
        /* code */
        return "Intrerrupted";
    case NORMALEXIT:
        /* code */
        return "NormalExit";
    case TERMINATED:
        /* code */
        return "Terminated";

    default:
        return "Running";
    }
}

void handler_stop(pid_t pid)
{
    if (foreground_process == 0)
    {
        int *status;
        waitpid(-1, status, WUNTRACED);
        exit(0);
    }
    else
    {
        int number = find_jid((char *)&pid);
        if (number == foreground_process)
        {
            kill(job_list[number].pid, SIGINT);
        }
        foreground_process = 0;
    }
}
