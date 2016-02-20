typedef struct {
	const char *head;
	const char *text;
} notify_t;

void show(notify_t *msg);
void debug(const char *fmt, ...);
void error(const char *fmt, ...);
