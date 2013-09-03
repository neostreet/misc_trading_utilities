use trading

select left(entry_filled,5),type,count(*),sum(points),sum(delta)
from trade
group by 1,2
order by 1,2;

quit
