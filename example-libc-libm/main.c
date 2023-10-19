#include <stdio.h>
#include <swilib.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <dirent.h>
#include <stdbool.h>

static void init_stdio(void) {
	__setup_stdout_fd(open("0:\\Misc\\stdout.txt", O_CREAT | O_TRUNC | O_WRONLY));
	__setup_stderr_fd(open("0:\\Misc\\stderr.txt", O_CREAT | O_TRUNC | O_WRONLY));
}

static void test_date_time(void) {
	char str[1024];
	
	time_t now = time(NULL);
	struct tm tm, tm_gm;
	localtime_r(&now, &tm);
	gmtime_r(&now, &tm_gm);
	
	printf("Testing Date and Time:\n");
	
	printf(
		"unix time: %d\n"
		"local time: %d-%02d-%02d %02d:%02d:%02d\n"
		"utc time: %d-%02d-%02d %02d:%02d:%02d\n",
		now,
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
		tm_gm.tm_year + 1900, tm_gm.tm_mon + 1, tm_gm.tm_mday, tm_gm.tm_hour, tm_gm.tm_min, tm_gm.tm_sec
	);
	
	strftime(str, sizeof(str) - 1, "%x - %I:%M%p", &tm);
	printf("strftime: %s\n", str);
	
	printf("-----------\n");
}

static void test_readdir(void) {
	const char *folder = "0:\\Misc\\*"; // '*' is important, wtf?
	
	printf("Files in %s:\n", folder);
	
	DIR *dh = opendir(folder);
	if (dh != NULL) {
		int files = 0;
		struct dirent *entry;
		while ((entry = readdir(dh))) {
			files++;
			printf("File %3d: %s\n", files, entry->d_name);
		}
		closedir(dh);
	}
	printf("-----------\n");
}

static void test_libm(void) {
	printf("cos: %f\n", cosf(1234.0f));
	printf("sin: %f\n", sinf(1234.0f));
	printf("atan: %f\n", atanf(1234.0f));
	printf("-----------\n");
}

int main() {
	init_stdio();
	test_date_time();
	test_readdir();
	test_libm();
	
	ShowMSG(0, (int) "See logs!");
	
	fflush(stdout);
	fflush(stderr);
	fclose(stdout);
	fclose(stderr);
	
	kill_elf();
    
    return 0;
}
