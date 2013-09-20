use trading

select sum(delta),count(*),sum(ticks),left(entry_filled,5)
from trade
where composite_trade_type = 'Limit - Market'
group by 4
order by 4;

quit
