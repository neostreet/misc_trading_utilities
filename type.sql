use trading

select type,count(*),sum(points),sum(delta)
from trade
group by type
order by type;

quit
