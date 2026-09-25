#pragma once

#ifndef CNET_FILE_H
#define CNET_FILE_H

#include "CNetGc.h"
#include "CNetApi.h"
#include "CNetString.h"

typedef struct sFile File;

/**
 * Create a File reference for a path, without touching disk. Slashes are
 * normalized to '\\' and duplicate separators are collapsed.
 *
 * @param pathname Wide path (absolute or relative); borrowed, copied internally.
 * @return New File the caller must release with fileFree(), or NULL if the
 *         File license feature is not enabled.
 */
CNET_API File* fileCreate(const wchar_t* pathname);
/**
 * Create a File for a path formed by appending a child name/path to a
 * parent File's path (inserting a separator if needed).
 *
 * @param parent Parent File; borrowed.
 * @param child  Wide name or relative path to append; borrowed.
 * @return New File the caller must release with fileFree(), or NULL if the
 *         combined path exceeds the internal buffer or the license check fails.
 */
CNET_API File* fileCreateChild(const File* parent, const wchar_t* child);
/**
 * Release a File and everything allocated through it (e.g. strings and
 * arrays returned by its accessors).
 *
 * @param file File to free; ownership is taken, do not use it afterward.
 */
CNET_API void fileFree(File* file);

/**
 * Get the normalized path this File refers to.
 *
 * @param file File to query; borrowed.
 * @return Wide path owned by file; valid until fileFree() is called.
 */
CNET_API const wchar_t* fileGetPath(const File* file);
/**
 * Get the last component of the path (file or directory name).
 *
 * @param file File to query; borrowed.
 * @return Wide name owned by file; valid until fileFree() is called.
 */
CNET_API const wchar_t* fileGetName(const File* file);
/**
 * Get the parent directory's path.
 *
 * @param file File to query; borrowed.
 * @return Wide parent path owned by file, or NULL if the path has no
 *         parent component (e.g. a bare drive letter or root).
 */
CNET_API const wchar_t* fileGetParent(const File* file);
/**
 * Get the parent directory as a new File.
 *
 * @param file File to query; borrowed.
 * @return New File the caller must release with fileFree(), or NULL if
 *         the path has no parent.
 */
CNET_API File* fileGetParentFile(const File* file);
/**
 * Resolve the path to an absolute path (via GetFullPathName), without
 * requiring the file to exist.
 *
 * @param file File to query; borrowed.
 * @return Wide absolute path owned by file, or NULL on failure.
 */
CNET_API const wchar_t* fileGetAbsolutePath(const File* file);
/**
 * Resolve the path to an absolute path and wrap it as a new File.
 *
 * @param file File to query; borrowed.
 * @return New File the caller must release with fileFree(), or NULL on failure.
 */
CNET_API File* fileGetAbsoluteFile(const File* file);
/**
 * Check whether the path is absolute (a drive letter like "C:\\" or a
 * UNC path starting with "\\\\"), purely by inspecting the string.
 *
 * @param file File to query; borrowed.
 * @return 1 if the path is absolute, 0 otherwise.
 */
CNET_API int fileIsAbsolute(const File* file);

/**
 * Check whether the path refers to an existing file or directory on disk.
 *
 * @param file File to query; borrowed.
 * @return 1 if the path exists, 0 if it does not (or on error).
 */
CNET_API int fileExists(const File* file);
/**
 * Check whether the path refers to an existing directory.
 *
 * @param file File to query; borrowed.
 * @return 1 if the path exists and is a directory, 0 otherwise.
 */
CNET_API int fileIsDirectory(const File* file);
/**
 * Check whether the path refers to an existing, non-directory file.
 *
 * @param file File to query; borrowed.
 * @return 1 if the path exists and is not a directory, 0 otherwise
 *         (including when the path does not exist).
 */
CNET_API int fileIsFile(const File* file);
/**
 * Get the size of the file in bytes.
 *
 * @param file File to query; borrowed.
 * @return File size in bytes, or 0 if the path does not exist or is a directory.
 */
CNET_API long long fileLength(const File* file);
/**
 * Get the last-modified timestamp of the file or directory.
 *
 * @param file File to query; borrowed.
 * @return Milliseconds since the Unix epoch (UTC), or 0 if the path
 *         does not exist.
 */
CNET_API long long fileLastModified(const File* file);
/**
 * Check whether the file can be read, i.e. whether it exists.
 *
 * @param file File to query; borrowed.
 * @return 1 if the file exists, 0 otherwise.
 */
CNET_API int fileCanRead(const File* file);
/**
 * Check whether the file can be written to (exists and is not read-only).
 *
 * @param file File to query; borrowed.
 * @return 1 if the file exists and is writable, 0 otherwise.
 */
CNET_API int fileCanWrite(const File* file);
/**
 * Attempt to mark the file readable. Only setting readable to true is
 * meaningful (there is no Windows "unreadable" file attribute this maps
 * to); passing false always fails without changing anything. On
 * Linux/macOS this same "false always fails" contract is kept deliberately
 * even though chmod() could clear the owner-read bit — see File.posix.c —
 * to preserve identical behavior across platforms rather than a
 * capability POSIX happens to have.
 *
 * @param file     File to modify; borrowed.
 * @param readable Nonzero to mark readable; 0 is always a no-op failure.
 * @return 1 if readable was nonzero and the file exists, 0 otherwise.
 */
CNET_API int fileSetReadable(File* file, int readable);
/**
 * Set or clear the file's read-only attribute (FILE_ATTRIBUTE_READONLY).
 *
 * @param file     File to modify; borrowed.
 * @param writable Nonzero to clear read-only (allow writes), 0 to set
 *                 read-only.
 * @return 1 on success, 0 if the file does not exist or the attribute
 *         change failed.
 */
CNET_API int fileSetWritable(File* file, int writable);
/**
 * Mark the file read-only. Equivalent to fileSetWritable(file, 0).
 *
 * @param file File to modify; borrowed.
 * @return 1 on success, 0 on failure.
 */
CNET_API int fileSetReadOnly(File* file);

/**
 * Atomically create a new, empty file, failing if it already exists.
 *
 * @param file File describing the path to create; borrowed.
 * @return 1 if the file was created, 0 if it already existed or creation failed.
 */
CNET_API int fileCreateNewFile(File* file);
/**
 * Create the directory named by this path (its parent must already exist).
 *
 * @param file File describing the directory to create; borrowed.
 * @return 1 on success, 0 on failure.
 */
CNET_API int fileMkdir(File* file);
/**
 * Create the directory named by this path along with any missing parent
 * directories.
 *
 * @param file File describing the directory to create; borrowed.
 * @return 0 if the directory already exists (no work done), otherwise the
 *         result (1 success / 0 failure) of creating the final directory
 *         component after ensuring its parents exist.
 */
CNET_API int fileMkdirs(File* file);
/**
 * Delete the file or (empty) directory from disk.
 *
 * @param file File to delete; borrowed.
 * @return 1 on success, 0 on failure (e.g. path missing, directory not empty).
 */
CNET_API int fileDelete(File* file);
/**
 * Rename or move the file/directory to a new path.
 *
 * @param file File to rename; borrowed.
 * @param dest File describing the destination path; borrowed.
 * @return 1 on success, 0 on failure.
 */
CNET_API int fileRenameTo(File* file, const File* dest);

/**
 * List the names of entries directly inside a directory (excluding "."
 * and "..").
 *
 * @param file     Directory to list; borrowed. If not a directory, no
 *                  entries are returned.
 * @param outCount Set to the number of names returned (0 on failure).
 * @return Newly malloc'd array of malloc'd wide name strings (not full
 *         paths) that the caller must free (each string, then the array),
 *         or NULL if the path is not a directory or could not be opened.
 */
CNET_API wchar_t** fileList(const File* file, int* outCount);
/**
 * List the entries directly inside a directory as child Files.
 *
 * @param file     Directory to list; borrowed.
 * @param outCount Set to the number of Files returned (0 on failure).
 * @return Newly malloc'd array of File* the caller must free (fileFree()
 *         each entry, then free() the array), or NULL if the path is not
 *         a directory or could not be opened.
 */
CNET_API File** fileListFiles(const File* file, int* outCount);

/**
 * Read the entire contents of the file into memory.
 *
 * @param file      File to read; borrowed.
 * @param outLength Set to the number of bytes read on success.
 * @return Buffer owned by file's internal allocator (do not free()
 *         directly; it is released when the file is freed with
 *         fileFree()), or NULL on failure (missing file, short read, etc.).
 */
CNET_API char* fileReadAllBytes(const File* file, int* outLength);
/**
 * Overwrite the file with the given bytes (creating it if needed,
 * truncating any existing content).
 *
 * @param file   File to write; borrowed.
 * @param data   Bytes to write; borrowed.
 * @param length Number of bytes in data.
 * @return 1 on success, 0 on failure.
 */
CNET_API int fileWriteAllBytes(File* file, const char* data, int length);
/**
 * Read the entire file and decode it as text.
 *
 * @param file File to read; borrowed.
 * @return New String the caller must release with stringFree(), or NULL on failure.
 */
CNET_API String* fileReadAllText(const File* file);
/**
 * Overwrite the file with the UTF-8 encoding of a String.
 *
 * @param file File to write; borrowed.
 * @param text Text to write; borrowed.
 * @return 1 on success, 0 on failure.
 */
CNET_API int fileWriteAllText(File* file, const String* text);

#endif
