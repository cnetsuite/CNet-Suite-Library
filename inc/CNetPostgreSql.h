#pragma once

#ifndef CNET_POSTGRESQL_H
#define CNET_POSTGRESQL_H

#include "CNetGc.h"
#include "CNetDataSet.h"
#include "CNetApi.h"

typedef struct sPostgreSqlConnection PostgreSqlConnection;

/**
 * Open a connection to PostgreSQL via ODBC (psqlODBC).
 *
 * @param gc               Allocator that owns the returned connection.
 * @param connectionString Wide ODBC connection string, e.g.
 *                          L"Driver={PostgreSQL Unicode};Server=localhost;".
 * @return A newly allocated PostgreSqlConnection, owned by gc, or NULL if
 *         the database feature is not licensed or the ODBC connect call
 *         fails.
 */
CNET_API PostgreSqlConnection* postgreSqlOpen(Gc* gc, const wchar_t* connectionString);

/**
 * Disconnect and release the ODBC handles held by a connection.
 *
 * @param conn Connection to close.
 */
CNET_API void postgreSqlClose(PostgreSqlConnection* conn);

/**
 * Run a parameterized SQL query and collect the results.
 *
 * @param gc         Allocator that owns the returned DataSet.
 * @param conn       Connection to query.
 * @param sql        Wide SQL text; use "?" placeholders for parameters.
 * @param params     Array of paramCount values bound in order to the "?"
 *                    placeholders in sql; may be NULL when paramCount is 0.
 *                    Text values are bound directly as UTF-16 for the driver.
 * @param paramCount Number of entries in params (and "?" placeholders in
 *                    sql); must not exceed the driver's internal parameter
 *                    limit (32).
 * @return A DataSet with one table holding the result rows, owned by gc,
 *         or NULL on error.
 */
CNET_API DataSet* postgreSqlQuery(Gc* gc, PostgreSqlConnection* conn, const wchar_t* sql, const DataValue* params, int paramCount);

/**
 * Run a parameterized non-query statement (INSERT/UPDATE/DELETE/DDL).
 *
 * @param conn       Connection to execute against.
 * @param sql        Wide SQL text; use "?" placeholders for parameters.
 * @param params     Array of paramCount values bound in order to the "?"
 *                    placeholders in sql; may be NULL when paramCount is 0.
 *                    Text values are bound directly as UTF-16 for the driver.
 * @param paramCount Number of entries in params (and "?" placeholders in
 *                    sql); must not exceed the driver's internal parameter
 *                    limit (32).
 * @return Number of rows affected by the statement (0 if none matched);
 *         also 0 if the call failed, so this cannot be used alone to
 *         detect errors.
 */
CNET_API int postgreSqlExecute(PostgreSqlConnection* conn, const wchar_t* sql, const DataValue* params, int paramCount);

#endif
