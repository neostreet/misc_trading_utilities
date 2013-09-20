use trading

select entry_order_type,exit_order_type,count(*),sum(ticks),sum(delta)
from trade
group by entry_order_type,exit_order_type
order by 5 desc,1,2;

quit
