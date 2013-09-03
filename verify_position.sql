use trading

select count(*),sum(points),
(count(*) * -2.4) + (sum(points) * 5),
sum(delta)
from trade;

quit
