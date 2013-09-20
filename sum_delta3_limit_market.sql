use trading

select sum(delta),count(*),sum(ticks),left(entry_filled,5)
from trade
where entry_order_type = 'Limit' and exit_order_type = 'Market'
group by 4
order by 4;

quit
