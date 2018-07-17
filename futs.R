futs<-function(cash,future,expiration,swap_long, swap_short,leverage){


days<-as.numeric(difftime(as.Date(expiration),Sys.time(),units="days"))

delta <- (cash-future)/cash

delta_year <- delta*100*(365/days)
print("=====================================================================")
print(paste("days                  : ",round(days,1)))
print(paste("arb in days (year) %  : ",round(delta*100.0,3), "(",round(delta_year,2),")"))
print(paste("swap long day (year) %: ",round(swap_long/365.0,3),"(",round(swap_long,2),")"))
print(paste("swap short day (year)%: ",round(swap_short/365.0,3),"(",round(swap_short,2),")"))

print("=====================================================================")

if(delta>=0)
{
#backwardation

# proper arb, short cash/swap, profit on arb, loss/profit on swap
	if((delta_year + swap_short) > 0) 
		{
		rate <- round((delta_year+swap_short),2)
		print("---------------------------------------------------------------------")
		print("YES, YES, it's arbitrage !!!")
		print("proper arb, short cash/swap, profit on arb, loss/profit on swap")
		print(paste("ARB+SWAP day (year) %      : ",round(rate/365,3),"(",round(rate,2),")"))
		print(paste("profit short+long*margin % : ", round(rate/365,3)*(leverage/2.0),"(",round(rate,2)*(leverage/2.0),")"))
			print("ACTION                     : BUY futures, SELL cash")
		}

# reverse arb, long cash/swap. Loss on arb, profit on swap
	if((-delta_year + swap_long) > 0) 
		{
		rate <- round((delta_year+swap_long),2)
		print("---------------------------------------------------------------------")
		print("YES, YES, it's arbitrage !!!")
		print("reverse arb, long cash/swap. Loss on arb, profit on swap")
		print(paste("ARB+SWAP day (year) %      : ",round(rate/365,3),"(",round(rate,2),")"))

		print(paste("profit short+long*margin % : ", round(rate/365,3)*(leverage/2.0),"(",round(rate,2)*(leverage/2.0),")"))
			print("ACTION                     : SELL futures, BUY cash")
		}
}

if(delta <= 0)
{
#contango _/
delta_year <- (-delta_year)

# proper arb, long cash/swap, profit on arb, loss/profit on swap
	if((delta_year + swap_long) > 0) 
		{
		rate <- round((-delta_year+swap_long),2)

		print("---------------------------------------------------------------------")
		print("YES, YES, it's arbitrage !!!")
		print("proper arb, long cash/swap, profit on arb, loss/profit on swap")
		print(paste("ARB+SWAP day (year) %      : ",round(rate/365,3),"(",round(rate,2),")"))

		print(paste("profit short+long*margin % : ", round(rate/365,3)*(leverage/2.0),"(",round(rate,2)*(leverage/2.0),")"))
			print("ACTION                     : SELL futures, BUY cash")
		}

# reverse arb, short cash/swap. Loss on arb, profit on swap
	if((-delta_year + swap_short) > 0) 
		{
		rate <- round((-delta_year+swap_short),2)

		print("---------------------------------------------------------------------")
		print("YES, YES, it's arbitrage !!!")
		print("reverse arb, short cash/swap. Loss on arb, profit on swap")
		print(paste("ARB+SWAP day (year) %      : ",round(rate/365,3),"(",round(rate,2),")"))
		print(paste("profit short+long*margin % : ", round(rate/365,3)*(leverage/2.0),"(",round(rate,2)*(leverage/2.0),")"))
			print("ACTION                     : BUY futures, SELL cash")
		}

};








};