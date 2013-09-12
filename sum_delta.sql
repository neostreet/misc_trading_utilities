use trading

select left(entry_filled,5),count(*),sum(ticks),sum(delta)
from trade
group by 1
order by 1;

select count(*),sum(ticks),sum(delta)
from trade;

quit
