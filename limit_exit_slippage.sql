use trading

select type,exit_entered,exit_order_limit,exit_filled_price,exit_filled_price - exit_order_limit
from trade
where exit_order_type = 'Limit' and
exit_order_limit != exit_filled_price
order by id;

quit
