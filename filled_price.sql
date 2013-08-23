use trading

select type,filled_price
from tradestation_order
where order_status = 'Filled'
order by id;

quit
