#ifndef GEVREK_LOG_H
#define GEVREK_LOG_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define GEVREK_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
#define GEVREK_CAT_IMPL(a, b) a##b
#define GEVREK_CAT(a, b) GEVREK_CAT_IMPL(a, b)
#define GEVREK_STATIC_ASSERT(cond, msg) \
    typedef char GEVREK_CAT(gevrek_static_assert_t_, __LINE__)[(cond) ? 1 : -1]
#endif

typedef enum GevrekError {
    GEVREK_ERROR_NONE = 0,
    GEVREK_ERROR_OUT_OF_MEMORY,
    GEVREK_ERROR_FILE_IO,
    GEVREK_ERROR_INVALID_PARAMETER,
    GEVREK_ERROR_DEVICE_FAILURE,
    GEVREK_ERROR_UNKNOWN
} GevrekError;

void        GevrekSetError(GevrekError error, const char* message);
GevrekError GevrekGetLastError(void);
const char* GevrekGetErrorString(GevrekError error);
const char* GevrekGetLastErrorMessage(void);

typedef enum GevrekLogSeverity {
    GEVREK_LOG_INFO = 0,
    GEVREK_LOG_WARNING,
    GEVREK_LOG_ERROR,
    GEVREK_LOG_FATAL
} GevrekLogSeverity;

#ifdef GEVREK_DEBUG

void GevrekSetLogLevel(GevrekLogSeverity level);
GevrekLogSeverity GevrekGetLogLevel(void);

typedef void (*GevrekLogCallback)(GevrekLogSeverity severity, const char* message, void* userContext);
void GevrekLogSetCallback(GevrekLogCallback callback, void* userContext);

void GevrekLogInternal(GevrekLogSeverity severity, const char* file, int line, const char* fmt, ...);
#define GEVREK_LOG(severity, ...) GevrekLogInternal(severity, __FILE__, __LINE__, __VA_ARGS__)

#else

static inline void GevrekSetLogLevel(GevrekLogSeverity level) { (void)level; }
static inline GevrekLogSeverity GevrekGetLogLevel(void) { return GEVREK_LOG_FATAL; }

typedef void (*GevrekLogCallback)(GevrekLogSeverity severity, const char* message, void* userContext);
static inline void GevrekLogSetCallback(GevrekLogCallback callback, void* userContext) {
    (void)callback;
    (void)userContext;
}

#define GEVREK_LOG(severity, ...) ((void)0)

#endif

#endif // GEVREK_LOG_H