use trading

select left(entry_filled,5),count(*),sum(points),sum(delta)
from trade
group by 1
order by 1;

quit
