#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

int main()
{
	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stmt;
	SQLRETURN ret;
	SQLCHAR outstr[1024];
	SQLINTEGER boolval;
	SQLLEN boolval_len;
	SQLSMALLINT outstrlen;
	SQLHSTMT     hstmt;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
	ret = SQLDriverConnect(dbc, NULL, "DSN=mariadb_13011", SQL_NTS,
			outstr, sizeof(outstr), &outstrlen,
			SQL_DRIVER_COMPLETE);
	SQLAllocHandle(SQL_HANDLE_STMT, dbc, &hstmt);
	
	if (SQL_SUCCEEDED(ret)) {
		ret = SQLExecDirect(hstmt, "DROP TABLE IF EXISTS bool_test", SQL_NTS);
		if (!SQL_SUCCEEDED(ret)) {
			fprintf(stderr, "Failed to drop\n");
			goto stop;
		}
		ret = SQLExecDirect(hstmt, "CREATE TABLE bool_test(id INT, b INT)", SQL_NTS);
		if (!SQL_SUCCEEDED(ret)) {
			fprintf(stderr, "Failed to create\n");
			goto stop;
		}
		ret = SQLExecDirect(hstmt, "INSERT INTO bool_test VALUES (1, 1), (2,1), (3,0)", SQL_NTS);
		if (!SQL_SUCCEEDED(ret)) {
			fprintf(stderr, "Failed to insert\n");
			goto stop;
		}
		ret = SQLExecDirect(hstmt, "SELECT cast(b as char) FROM bool_test where id=1", SQL_NTS);
		if (SQL_SUCCEEDED(ret)) {
			while(1) {
				ret = SQLFetch(hstmt);
				if (!SQL_SUCCEEDED(ret))
					goto stop;
				SQLGetData(hstmt, 1, SQL_C_SLONG, &boolval, 0, &boolval_len);
				printf("Returned value: %d, len: %ld\n", boolval, boolval_len);
			}
		}
	} else {
		fprintf(stderr, "Failed to connect\n");
		goto stop;
	}
	
stop:
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLFreeHandle(SQL_HANDLE_DBC, dbc);
	SQLFreeHandle(SQL_HANDLE_ENV, env);
}
