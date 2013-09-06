use trading

select composite_trade_type,count(*)
from trade
group by composite_trade_type
order by 2 desc,1;

quit
