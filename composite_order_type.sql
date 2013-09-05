use trading

select composite_order_type,count(*)
from trade
group by composite_order_type
order by 2 desc,1;

quit
