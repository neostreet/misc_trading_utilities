@echo off
gen_insert_trade 5 2.5 %1 > %1.gen_insert_trade.sql
call mydl sql
