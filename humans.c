#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>

#define NUM_FORKS 5

static volatile sig_atomic_t running = 1;
static void sighandler(int signum)
{
	syslog(LOG_INFO, "You killed a human with signal %d!", signum);
	running = 0;
}
int main(void)
{
	pid_t pid;
	int i;
	struct sigaction sa;
	openlog(NULL, LOG_PID, LOG_USER);
	sa.sa_handler = sighandler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGTERM, &sa, NULL) == -1) {
		syslog(LOG_ERR, "couldn't handle SIGTERM");
	}
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		syslog(LOG_ERR, "couldn't handler SIGINT");
	}
	pid = fork();
	if (pid > 0) {
		return 0;
	}
	if (pid < 0) {
		syslog(LOG_ERR, "coudn't fork");
		return -1;
	}
	for (i = 0; pid == 0 && i < NUM_FORKS - 1; i++) {
		if (fork() < 0) {
			syslog(LOG_ERR, "couldn't fork");
			return -1;
		}
	}
	while (running) {
		sleep(60);
	}
	return 0;
}
