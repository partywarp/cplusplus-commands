#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <ext/stdio_filebuf.h>
#include <csignal>
#include <atomic>
#include <fstream>
#include <fcntl.h>
#include <cstdlib>
#include <glob.h>
#include "check.hpp"
static std::atomic<pid_t> child_pid;
static std::atomic<bool> timed_out;
void handle_alarm(int)
{
	timed_out.store(true);
	kill(child_pid.load(), SIGTERM);
}
void launch_test(char *target_file, char *test_file)
{
	std::string check_filename(test_file);
	check_filename += ".provided";
	std::ifstream check_file(check_filename);
	std::fprintf(stderr, "-- %s\n", test_file);
	int fd[2]; // read end, write end
	if (pipe(fd) == -1) {
		std::perror("pipe");
		return;
	}
	pid_t pid = fork();
	if (pid == -1) {
		std::perror("fork");
		return;
	}
	if (pid == 0) { // child
		close(fd[0]);
		if (check_file) {
			dup2(fd[1], STDOUT_FILENO);
		}
		close(fd[1]);
		int test = open(test_file, O_RDONLY);
		if (test == -1) {
			std::perror("open");
			std::exit(EXIT_FAILURE);
		}
		dup2(test, STDIN_FILENO);
		close(test);
		execl(target_file, target_file, NULL);
		std::perror("execl");
		std::exit(EXIT_FAILURE);
	}
	close(fd[1]);
	child_pid.store(pid);
	timed_out.store(false);
	std::signal(SIGALRM, handle_alarm);
	alarm(2);
	int status;
	waitpid(pid, &status, 0);
	alarm(0);
	if (timed_out.load()) {
		std::fprintf(stderr, "-- %s - time limit exceeded\n", test_file);
		return;
	}
	if (!WIFEXITED(status)) {
		std::fprintf(stderr, "-- %s - runtime error (#%d)\n", test_file,
			WTERMSIG(status));
		return;
	}
	if (check_file) {
		std::fputs("\n", stderr);
		__gnu_cxx::stdio_filebuf<char> process_buf(fd[0],
							   std::ios_base::in);
		std::istream is(&process_buf);
		if (!check(is, check_file)) {
			std::fprintf(stderr, "-- %s - wrong answer\n", test_file);
			return;
		}
	} else {
		// useless, might as well
		close(fd[0]);
	}
	std::fprintf(stderr, "-- %s - accepted\n", test_file);
}
int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::exit(EXIT_FAILURE);
	}
	char *test_target = argv[1];
	glob_t globbuf;
	glob("*_test.txt", 0, NULL, &globbuf);
	for (int i = 0; i < globbuf.gl_pathc; ++i) {
		launch_test(test_target, globbuf.gl_pathv[i]);
	}
	globfree(&globbuf);
}
