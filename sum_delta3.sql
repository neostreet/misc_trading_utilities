use trading

select sum(delta),sum(ticks),left(entry_filled,5),count(*)
from trade
group by 3
order by 3;

quit
