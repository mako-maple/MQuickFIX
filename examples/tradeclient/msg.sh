#!/bin/bash
#// argv
if expr "$1" : '[0-9]*' > /dev/null ; 
then
  TAIL=$1
else
  TAIL=20
fi

#// MySQL - messages_log
mysql -u quickfix_user -pquickfix_pass quickfix << EOT  |  sort

set @@session.sql_mode='PIPES_AS_CONCAT';
select convert_tz(time, '+00:00', '+09:00')||'.'||right('000'||time_milliseconds,3) as time
     , right(sendercompid,4) as send
     , text   
  from messages_log
 order by id DESC
 limit ${TAIL}
;

EOT
