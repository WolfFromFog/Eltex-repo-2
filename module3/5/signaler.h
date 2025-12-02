#ifndef SIGNALER

extern int c_wait;
extern int in_counter;
extern int sig_counter;
extern int log_fd;

void listener_SIGINT(int sig);
void listener_SIGQUIT(int sig);

#endif