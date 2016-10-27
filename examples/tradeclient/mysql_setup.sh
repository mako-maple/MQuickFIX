#!/bin/bash

ROOT_DB_USER=$1
ROOT_DB_PASS=$2

DB_USER=quickfix_user
DB_PASS=quickfix_pass

##================================================================================
## DROP DATABASE & USER
mysql -u${ROOT_DB_USER} -p${ROOT_DB_PASS} << EOT
  DROP USER ${DB_USER}@localhost;
  DROP DATABASE quickfix;
EOT

##================================================================================
## CREATE DATABASE & USER
mysql -u${ROOT_DB_USER} -p${ROOT_DB_PASS} << EOT
  CREATE DATABASE quickfix;
  GRANT ALL ON    quickfix.*  to  ${DB_USER}@localhost  IDENTIFIED BY '${DB_PASS}';
  FLUSH PRIVILEGES;
EOT

##================================================================================
## CREATE QuickFIX TABLES
for sql in sessions_table.sql messages_table.sql messages_log_table.sql event_log_table.sql
do
  mysql -u${DB_USER} -p${DB_PASS} --execute="source ../../src/sql/mysql/${sql}";
done
