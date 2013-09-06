use trading

select composite_order_type,count(*),sum(ticks),sum(delta)
from trade
group by composite_order_type
order by 4 desc,1;

quit
