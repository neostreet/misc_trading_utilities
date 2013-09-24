use trading

select type,entry_filled,entry_filled_price,exit_filled,exit_filled_price,ticks,delta,entry_order_type,exit_order_type
from trade
order by id;

quit
