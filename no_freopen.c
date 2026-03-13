typedef struct __sFILE FILE;
extern FILE *freopen(const char *, const char *, FILE *);

static FILE *my_freopen(const char *path, const char *mode, FILE *stream) {
	return stream;
}

__attribute__((used)) static struct {
	const void *replacement;
	const void *replacee;
} interposers[] __attribute__((section("__DATA,__interpose"))) = {
	{ my_freopen, freopen }
};
