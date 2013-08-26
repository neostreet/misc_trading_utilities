use trading

select type,sum(points),sum(delta),count(*)
from trade
group by type
order by type;

quit
