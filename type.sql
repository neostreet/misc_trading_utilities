use trading

select type,count(*),sum(ticks),sum(delta)
from trade
group by type
order by type;

quit
