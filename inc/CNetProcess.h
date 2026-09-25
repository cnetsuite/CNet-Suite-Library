#pragma once

#ifndef CNET_PROCESS_H
#define CNET_PROCESS_H

#include "CNetApi.h"

typedef struct sProcess Process;

/**
 * Launch a child process and start capturing its stdout/stderr.
 *
 * commandLine is passed to CreateProcess verbatim (e.g. "cmd.exe /c dir");
 * workingDirectory may be NULL to inherit the parent's current directory.
 * input may be NULL; if non-NULL, its bytes are written to the child's
 * stdin on a background thread and the pipe is then closed (EOF), so
 * commands that read stdin to completion (e.g. findstr) still terminate.
 * The child's stdout/stderr are always captured via pipes drained on
 * background reader threads, avoiding deadlock while the child is still
 * running.
 *
 * @param commandLine      Full command line, passed to CreateProcess
 *                          unmodified; must be non-empty; borrowed.
 * @param workingDirectory Directory to launch in, or NULL to inherit the
 *                          caller's current directory; borrowed.
 * @param input             Bytes to write to the child's stdin then close
 *                          it, or NULL to send no input and close stdin
 *                          immediately; borrowed, must outlive the write
 *                          (until the process is waited on or freed).
 * @return New Process the caller must release with processFree() (after
 *         it has exited), or NULL if the Process license feature is not
 *         enabled or the process could not be spawned (asserts on spawn
 *         failure in debug builds).
 */
CNET_API Process* processStart(const wchar_t* commandLine, const wchar_t* workingDirectory, const char* input);
/**
 * Release a process and its resources (pipe handles, reader threads,
 * output buffers).
 *
 * @param process Process to free; ownership is taken, do not use it
 *                afterward. Must have already exited (see processWait()/
 *                processKill()).
 * @return true on success, false if the process has not exited yet (nothing is freed).
 */
CNET_API bool processFree(Process* process);

/**
 * Block until the process exits, then finalize its exit code and captured
 * output.
 *
 * @param process Process to wait for; borrowed.
 */
CNET_API void processWait(Process* process);
/**
 * Block until the process exits or a timeout elapses.
 *
 * @param process      Process to wait for; borrowed.
 * @param timeoutMillis Maximum time to wait, in milliseconds.
 * @return 1 if the process exited (and its exit code/output were
 *         finalized), 0 if the timeout elapsed while it was still running.
 */
CNET_API int processWaitFor(Process* process, unsigned long timeoutMillis);
/**
 * Check whether the process is still running, without blocking.
 *
 * @param process Process to check; borrowed.
 * @return 1 if still running, 0 if it has exited.
 */
CNET_API int processIsRunning(const Process* process);
/**
 * Forcibly terminate the process (exit code 1) and wait for it to finish
 * exiting.
 *
 * @param process Process to kill; borrowed.
 */
CNET_API void processKill(Process* process);

/**
 * Get the OS process ID.
 *
 * @param process Process to query; borrowed.
 * @return Process ID assigned by the OS at launch.
 */
CNET_API unsigned long processGetId(const Process* process);
/**
 * Get the process's exit code.
 *
 * @param process Process to query; borrowed. Must have already exited.
 * @return Exit code reported by the OS, or 0 if the process has not exited yet.
 */
CNET_API int processGetExitCode(const Process* process);
/**
 * Get everything the process wrote to stdout.
 *
 * @param process Process to query; borrowed. Must have already exited.
 * @return NUL-terminated buffer owned by process (valid until
 *         processFree()), or NULL if the process has not exited yet.
 */
CNET_API const char* processGetOutput(const Process* process);
/**
 * Get everything the process wrote to stderr.
 *
 * @param process Process to query; borrowed. Must have already exited.
 * @return NUL-terminated buffer owned by process (valid until
 *         processFree()), or NULL if the process has not exited yet.
 */
CNET_API const char* processGetError(const Process* process);

#endif
