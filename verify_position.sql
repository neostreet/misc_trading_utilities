use trading

select count(*),sum(ticks),
(count(*) * -2.4) + (sum(ticks) * 5),
sum(delta)
from trade;

quit
