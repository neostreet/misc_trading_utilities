use trading

select composite_trade_type,count(*),sum(ticks),sum(delta)
from trade
group by composite_trade_type
order by 4 desc,1;

quit
