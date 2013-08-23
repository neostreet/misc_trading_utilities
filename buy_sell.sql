use trading

select type,count(*)
from tradestation_order
group by type
order by type;

quit
