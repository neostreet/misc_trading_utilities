use trading

select count(*)
from trade
where type = 'L'
and delta > 0;

select count(*)
from trade
where type = 'L'
and delta < 0;

select count(*)
from trade
where type = 'S'
and delta > 0;

select count(*)
from trade
where type = 'S'
and delta < 0;

quit
