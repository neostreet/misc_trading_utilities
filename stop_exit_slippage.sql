use trading

select type,exit_entered,exit_stop,exit_filled_price,exit_filled_price - exit_stop
from trade
where exit_order_type = 'Stop Market' and
exit_stop != exit_filled_price
order by id;

quit
