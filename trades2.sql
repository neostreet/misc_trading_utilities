use trading

select type,entry_filled,entry_filled_price,exit_filled,exit_filled_price,points,delta
from trade
order by id;

quit
