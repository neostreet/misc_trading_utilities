use trading

select exit_filled,ticks,delta,entry_order_type,exit_order_type
from trade
order by ticks desc,exit_filled desc;

quit
