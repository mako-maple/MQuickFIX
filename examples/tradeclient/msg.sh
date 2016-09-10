#!/bin/bash
mysql -u quickfix -pquickfix quickfix << EOT  |  tail -20

set @@session.sql_mode='PIPES_AS_CONCAT';
select time||'.'||right('000'||time_milliseconds,3) as time
     , right(sendercompid,4) as send
     , text   
  from messages_log;

EOT
