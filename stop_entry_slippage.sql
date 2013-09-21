use trading

select type,entry_entered,entry_stop,entry_filled_price,entry_filled_price - entry_stop
from trade
where entry_order_type = 'Stop Market' and
entry_stop != entry_filled_price
order by id;

quit
