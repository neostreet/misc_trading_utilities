use trading

select order_status,count(*)
from tradestation_order
group by order_status
order by order_status;

quit
