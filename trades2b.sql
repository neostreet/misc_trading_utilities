use trading

select delta,type,entry_filled_price,exit_filled_price,ticks,entry_order_type,exit_order_type
from trade
order by id;

quit
