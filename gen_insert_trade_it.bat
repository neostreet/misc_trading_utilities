@echo off
gen_insert_trade 5 1.2 %1.txt > %1.gen_insert_trade.sql
call mydl sql
