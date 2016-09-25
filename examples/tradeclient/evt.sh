#!/bin/bash
mysql -u quickfix_user -pquickfix_pass quickfix << EOT  |  tail -20

set @@session.sql_mode='PIPES_AS_CONCAT';

select time||','||right('000' || time_milliseconds, 3) as time
     , right( sendercompid, 4)  as send
     , text
  from event_log
 order by id;

EOT
