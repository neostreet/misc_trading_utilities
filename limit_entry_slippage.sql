use trading

select type,entry_entered,entry_order_limit,entry_filled_price,entry_filled_price - entry_order_limit
from trade
where entry_order_type = 'Limit' and
entry_order_limit != entry_filled_price
order by id;

quit
