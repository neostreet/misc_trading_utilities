use trading

select sum(delta),count(*),sum(ticks),left(entry_filled,5)
from trade
group by 4
order by 1 desc,4;

quit
