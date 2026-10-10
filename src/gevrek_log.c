#include "gevrek_log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define GEVREK_THREAD_LOCAL _Thread_local
#else
#define GEVREK_THREAD_LOCAL
#endif

static GEVREK_THREAD_LOCAL GevrekError s_lastError = GEVREK_ERROR_NONE;
static GEVREK_THREAD_LOCAL char        s_lastErrorMsg[256] = "No error";

void GevrekSetError(GevrekError error, const char* message) {
    s_lastError = error;
    if (message) {
        int i = 0;
        while (message[i] != '\0' && i < (int)(sizeof(s_lastErrorMsg) - 1)) {
            s_lastErrorMsg[i] = message[i];
            i++;
        }
        s_lastErrorMsg[i] = '\0';
    }
    else {
        s_lastErrorMsg[0] = '\0';
    }
}

GevrekError GevrekGetLastError(void) {
    return s_lastError;
}

const char* GevrekGetErrorString(GevrekError error) {
    switch (error) {
    case GEVREK_ERROR_NONE:
        return "No error";
    case GEVREK_ERROR_OUT_OF_MEMORY:
        return "Out of memory / Memory arena capacity exceeded";
    case GEVREK_ERROR_FILE_IO:
        return "File I/O read or write failure";
    case GEVREK_ERROR_INVALID_PARAMETER:
        return "Invalid parameter or null pointer passed";
    case GEVREK_ERROR_DEVICE_FAILURE:
        return "Graphics or hardware device creation fault";
    case GEVREK_ERROR_UNKNOWN:
    default:
        return "Unknown or unhandled engine error";
    }
}

const char* GevrekGetLastErrorMessage(void) {
    return s_lastErrorMsg;
}

#ifdef GEVREK_DEBUG

static GevrekLogSeverity s_currentLogLevel = GEVREK_LOG_INFO;
static GevrekLogCallback s_logCallback = NULL;
static void* s_logUserContext = NULL;

void GevrekSetLogLevel(GevrekLogSeverity level) {
    s_currentLogLevel = level;
}

GevrekLogSeverity GevrekGetLogLevel(void) {
    return s_currentLogLevel;
}

void GevrekLogSetCallback(GevrekLogCallback callback, void* userContext) {
    s_logCallback = callback;
    s_logUserContext = userContext;
}

void GevrekLogInternal(GevrekLogSeverity severity, const char* file, int line, const char* fmt, ...) {
    if (severity < s_currentLogLevel) {
        return;
    }

    char messageBuffer[1024];
    va_list args;

    va_start(args, fmt);
    vsnprintf(messageBuffer, sizeof(messageBuffer), fmt, args);
    va_end(args);

    if (s_logCallback) {
        s_logCallback(severity, messageBuffer, s_logUserContext);
        return;
    }

    const char* severityStr = "INFO";
    FILE* outputStream = stdout;

    switch (severity) {
    case GEVREK_LOG_INFO:    severityStr = "INFO";    break;
    case GEVREK_LOG_WARNING: severityStr = "WARN";    break;
    case GEVREK_LOG_ERROR:   severityStr = "ERROR";   outputStream = stderr; break;
    case GEVREK_LOG_FATAL:   severityStr = "FATAL";   outputStream = stderr; break;
    }

    fprintf(outputStream, "[%s] [%s:%d] %s\n", severityStr, file, line, messageBuffer);
}

#endif // GEVREK_DEBUG
