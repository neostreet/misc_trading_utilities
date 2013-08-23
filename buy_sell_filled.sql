use trading

select type,count(*)
from tradestation_order
where order_status = 'Filled'
group by type
order by type;

quit
