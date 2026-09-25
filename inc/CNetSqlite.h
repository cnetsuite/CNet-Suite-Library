#pragma once

#ifndef CNET_SQLITE_H
#define CNET_SQLITE_H

#include "CNetGc.h"
#include "CNetDataSet.h"
#include "CNetApi.h"

typedef struct sSqliteConnection SqliteConnection;

/**
 * Open (or create) a SQLite database file.
 *
 * @param gc   Allocator that owns the returned connection.
 * @param path Wide path to the database file; use L":memory:" for a private,
 *             temporary in-memory database.
 * @return A newly allocated SqliteConnection, owned by gc, or NULL if the
 *         database feature is not licensed or sqlite3_open16 fails.
 */
CNET_API SqliteConnection* sqliteOpen(Gc* gc, const wchar_t* path);

/**
 * Close a database connection and release its underlying sqlite3 handle.
 *
 * @param conn Connection to close.
 * @return true on success, false if sqlite3_close failed.
 */
CNET_API bool sqliteClose(SqliteConnection* conn);

/**
 * Run a parameterized SQL query and collect the results.
 *
 * @param gc         Allocator that owns the returned DataSet.
 * @param conn       Connection to query.
 * @param sql        Wide SQL text; use "?" placeholders for parameters.
 * @param params     Array of paramCount values bound in order to the "?"
 *                    placeholders in sql; may be NULL when paramCount is 0.
 * @param paramCount Number of entries in params (and "?" placeholders in
 *                    sql).
 * @return A DataSet with one table holding the result rows, owned by gc,
 *         or NULL on error.
 */
CNET_API DataSet* sqliteQuery(Gc* gc, SqliteConnection* conn, const wchar_t* sql, const DataValue* params, int paramCount);

/**
 * Run a parameterized non-query statement (INSERT/UPDATE/DELETE/DDL).
 *
 * @param conn       Connection to execute against.
 * @param sql        Wide SQL text; use "?" placeholders for parameters.
 * @param params     Array of paramCount values bound in order to the "?"
 *                    placeholders in sql; may be NULL when paramCount is 0.
 * @param paramCount Number of entries in params (and "?" placeholders in
 *                    sql).
 * @return Number of rows changed by the statement, or -1 on error.
 */
CNET_API int sqliteExecute(SqliteConnection* conn, const wchar_t* sql, const DataValue* params, int paramCount);

#endif
