#!/bin/bash

php -r '$connection = odbc_connect("Driver={MariaDB ODBC 3.0 Driver};Server=127.0.0.1;Port=13011;Uid=root;Database=test", "root", ""); $r = odbc_exec($connection, "select b from foo where id=1"); odbc_fetch_row($r); var_dump(odbc_result($r, "b"));'

