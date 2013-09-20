use trading

select entry_order_type,exit_order_type,count(*)
from trade
group by entry_order_type,exit_order_type
order by 3 desc,1,2;

quit
