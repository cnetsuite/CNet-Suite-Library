#pragma once

#ifndef CNET_DATA_SET_H
#define CNET_DATA_SET_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef enum {
    DATA_TYPE_NULL,    /**< No value; column/parameter is SQL NULL. */
    DATA_TYPE_INT,     /**< 32-bit signed integer, stored in value.intValue. */
    DATA_TYPE_INT64,   /**< 64-bit signed integer, stored in value.int64Value. */
    DATA_TYPE_DOUBLE,  /**< Double-precision float, stored in value.doubleValue. */
    DATA_TYPE_TEXT,    /**< Wide NUL-terminated string, stored in value.textValue. */
    DATA_TYPE_BLOB     /**< Raw binary data, stored in value.blobValue (data + length). */
} DataType;

typedef struct {
    wchar_t* name; /**< Column name; owned by the DataTable/Gc, do not free directly. */
    DataType type; /**< Declared type of every value stored in this column. */
} DataColumn;

typedef struct {
    DataType type; /**< Which member of the value union is active. */
    union {
        int intValue;
        long long int64Value;
        double doubleValue;
        const wchar_t* textValue; /**< Borrowed pointer; dataValueText() does not copy, dataRowSetText() does. */
        struct {
            const unsigned char* data;
            int length;
        } blobValue;
    } value;
} DataValue;

/**
 * Build a DataValue representing SQL NULL.
 *
 * @return A DataValue with type DATA_TYPE_NULL.
 */
CNET_API DataValue dataValueNull(void);

/**
 * Build a DataValue wrapping a 32-bit integer.
 *
 * @param value The integer to wrap.
 * @return A DataValue with type DATA_TYPE_INT.
 */
CNET_API DataValue dataValueInt(int value);

/**
 * Build a DataValue wrapping a 64-bit integer.
 *
 * @param value The integer to wrap.
 * @return A DataValue with type DATA_TYPE_INT64.
 */
CNET_API DataValue dataValueInt64(long long value);

/**
 * Build a DataValue wrapping a double.
 *
 * @param value The value to wrap.
 * @return A DataValue with type DATA_TYPE_DOUBLE.
 */
CNET_API DataValue dataValueDouble(double value);

/**
 * Build a DataValue wrapping a text string.
 *
 * @param value NUL-terminated wide string; borrowed by the returned
 *              DataValue (not copied), so it must stay valid until the
 *              DataValue is consumed (e.g. passed to a query/execute call).
 * @return A DataValue with type DATA_TYPE_TEXT.
 */
CNET_API DataValue dataValueText(const wchar_t* value);

/**
 * Build a DataValue wrapping raw binary data.
 *
 * @param data   Bytes to wrap; borrowed by the returned DataValue (not
 *               copied), so it must stay valid until the DataValue is
 *               consumed.
 * @param length Number of bytes in data.
 * @return A DataValue with type DATA_TYPE_BLOB.
 */
CNET_API DataValue dataValueBlob(const unsigned char* data, int length);

typedef struct sDataRow DataRow;
typedef struct sDataTable DataTable;
typedef struct sDataSet DataSet;

/**
 * Create an empty table with no columns or rows.
 *
 * @param gc   Allocator that owns the table and everything added to it;
 *             the table is freed when the Gc is freed.
 * @param name Table name; copied into the Gc, caller retains ownership
 *             of the string passed in.
 * @return A newly allocated DataTable, owned by gc.
 */
CNET_API DataTable* dataTableCreate(Gc* gc, const wchar_t* name);

/**
 * Append a column definition to a table.
 *
 * @param table Table to modify; must have no rows yet.
 * @param name  Column name; copied into the table's Gc.
 * @param type  Declared type for every value stored in this column.
 * @return Zero-based index of the new column, or -1 if the table already
 *         has rows (columns cannot be added after rows exist).
 */
CNET_API int dataTableAddColumn(DataTable* table, const wchar_t* name, DataType type);

/**
 * Number of columns defined on a table.
 *
 * @param table Table to inspect.
 * @return Column count.
 */
CNET_API int dataTableColumnCount(const DataTable* table);

/**
 * Fetch a column definition by index.
 *
 * @param table Table to inspect.
 * @param index Zero-based column index.
 * @return Pointer to the column definition (owned by table, do not
 *         free), or NULL if index is out of range.
 */
CNET_API const DataColumn* dataTableGetColumn(const DataTable* table, int index);

/**
 * Look up a column's index by name.
 *
 * @param table Table to inspect.
 * @param name  Column name to search for (case-sensitive exact match).
 * @return Zero-based column index, or -1 if no column has that name.
 */
CNET_API int dataTableColumnIndex(const DataTable* table, const wchar_t* name);

/**
 * Append a new row to a table, with every cell initialized to NULL.
 *
 * @param table Table to modify.
 * @return Pointer to the new row (owned by table, do not free); use the
 *         dataRowSet* functions to fill in values.
 */
CNET_API DataRow* dataTableAddRow(DataTable* table);

/**
 * Number of rows currently in a table.
 *
 * @param table Table to inspect.
 * @return Row count.
 */
CNET_API int dataTableRowCount(const DataTable* table);

/**
 * Fetch a row by index.
 *
 * @param table Table to inspect.
 * @param index Zero-based row index.
 * @return Pointer to the row (owned by table, do not free), or NULL if
 *         index is out of range.
 */
CNET_API DataRow* dataTableGetRow(const DataTable* table, int index);

/**
 * Set a cell to SQL NULL.
 *
 * @param row         Row to modify.
 * @param columnIndex Zero-based column index within the row.
 */
CNET_API void dataRowSetNull(DataRow* row, int columnIndex);

/**
 * Set a cell to a 32-bit integer value.
 *
 * @param row         Row to modify.
 * @param columnIndex Zero-based column index within the row.
 * @param value       Value to store.
 */
CNET_API void dataRowSetInt(DataRow* row, int columnIndex, int value);

/**
 * Set a cell to a 64-bit integer value.
 *
 * @param row         Row to modify.
 * @param columnIndex Zero-based column index within the row.
 * @param value       Value to store.
 */
CNET_API void dataRowSetInt64(DataRow* row, int columnIndex, long long value);

/**
 * Set a cell to a double value.
 *
 * @param row         Row to modify.
 * @param columnIndex Zero-based column index within the row.
 * @param value       Value to store.
 */
CNET_API void dataRowSetDouble(DataRow* row, int columnIndex, double value);

/**
 * Set a cell to a text value.
 *
 * @param row         Row to modify.
 * @param columnIndex Zero-based column index within the row.
 * @param value       NUL-terminated wide string; copied into the row's
 *                     Gc, caller retains ownership of the string passed in.
 */
CNET_API void dataRowSetText(DataRow* row, int columnIndex, const wchar_t* value);

/**
 * Set a cell to a binary blob value.
 *
 * @param row         Row to modify.
 * @param columnIndex Zero-based column index within the row.
 * @param data         Bytes to store; copied into the row's Gc, caller
 *                     retains ownership of the buffer passed in.
 * @param length       Number of bytes in data.
 */
CNET_API void dataRowSetBlob(DataRow* row, int columnIndex, const unsigned char* data, int length);

/**
 * Check whether a cell holds SQL NULL.
 *
 * @param row         Row to inspect.
 * @param columnIndex Zero-based column index within the row.
 * @return Non-zero if the cell is NULL, zero otherwise.
 */
CNET_API int dataRowIsNull(const DataRow* row, int columnIndex);

/**
 * Read a cell as a 32-bit integer.
 *
 * @param row         Row to inspect.
 * @param columnIndex Zero-based column index within the row.
 * @return The stored value, or 0 if the cell is not DATA_TYPE_INT.
 */
CNET_API int dataRowGetInt(const DataRow* row, int columnIndex);

/**
 * Read a cell as a 64-bit integer.
 *
 * @param row         Row to inspect.
 * @param columnIndex Zero-based column index within the row.
 * @return The stored value, or 0 if the cell is not DATA_TYPE_INT64.
 */
CNET_API long long dataRowGetInt64(const DataRow* row, int columnIndex);

/**
 * Read a cell as a double.
 *
 * @param row         Row to inspect.
 * @param columnIndex Zero-based column index within the row.
 * @return The stored value, or 0.0 if the cell is not DATA_TYPE_DOUBLE.
 */
CNET_API double dataRowGetDouble(const DataRow* row, int columnIndex);

/**
 * Read a cell as text.
 *
 * @param row         Row to inspect.
 * @param columnIndex Zero-based column index within the row.
 * @return Pointer to the stored wide string, owned by the row (do not
 *         free), or NULL if the cell is not DATA_TYPE_TEXT.
 */
CNET_API const wchar_t* dataRowGetText(const DataRow* row, int columnIndex);

/**
 * Read a cell as a binary blob.
 *
 * @param row         Row to inspect.
 * @param columnIndex Zero-based column index within the row.
 * @param outLength   Set to the blob's length in bytes (or 0 on error);
 *                     caller-owned storage, must not be NULL.
 * @return Pointer to the stored bytes, owned by the row (do not free),
 *         or NULL if the cell is not DATA_TYPE_BLOB.
 */
CNET_API const unsigned char* dataRowGetBlob(const DataRow* row, int columnIndex, int* outLength);

/**
 * Create an empty result set with no tables.
 *
 * @param gc Allocator that owns the data set; freed when the Gc is freed.
 * @return A newly allocated DataSet, owned by gc.
 */
CNET_API DataSet* dataSetCreate(Gc* gc);

/**
 * Append a table to a result set.
 *
 * @param dataSet Data set to modify.
 * @param table   Table to add; ownership stays with whichever Gc created
 *                it (typically the same Gc as dataSet).
 */
CNET_API void dataSetAddTable(DataSet* dataSet, DataTable* table);

/**
 * Number of tables in a result set.
 *
 * @param dataSet Data set to inspect.
 * @return Table count.
 */
CNET_API int dataSetTableCount(const DataSet* dataSet);

/**
 * Fetch a table by index.
 *
 * @param dataSet Data set to inspect.
 * @param index   Zero-based table index.
 * @return Pointer to the table (owned by dataSet's Gc, do not free), or
 *         NULL if index is out of range.
 */
CNET_API DataTable* dataSetGetTable(const DataSet* dataSet, int index);

#endif
