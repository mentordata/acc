soni<-function(swt,pairs_no=2,swap_level=0,x_currency=c()){


# swaptab:   id,short,long,swap
# example  tm.EURUSD_S,EUR,USD,+4.2
 
x_result<-c()

if(length(x_currency)==0)
	x_currency<-unique(swt$long)

#
#simple pair
#
if(pairs_no==2)
{
	for(x_first in x_currency)
	{
		for(x_second in x_currency)
		{
#print(paste(x_first,x_second))		
			x_norm <- swt[swt$long==x_first & swt$short==x_second,]$id
			x_opos <- swt[swt$short==x_first & swt$long==x_second,]$id
# i teraz mamy dwa wektory - zkrossowac je i podsumowac swapy i print			

#print(paste(x_first,x_second,x_norm,x_opos))		

			
			for(x_id_norm in x_norm)
			{
				for(x_id_opos in x_opos)
				{
# do the math

#print(paste(x_first,x_second,x_id_norm,x_id_opos))		

				x_swap_norm <-swt[swt$id==x_id_norm,]$swap
				x_swap_opos <-swt[swt$id==x_id_opos,]$swap
 
				x_delta <- x_swap_norm+x_swap_opos 				
				if(x_delta>=swap_level)
					{
					print(paste("SWAP ",x_first,x_second,x_id_norm,x_id_opos,round(x_delta,4),round(x_swap_norm,4),round(x_swap_opos,4)))
					if(length(x_result)!=0)
					{
					x_result<-rbind(x_result,data.frame(x_first,x_second,x_id_norm,x_id_opos,round(x_delta,4),round(x_swap_norm,4),round(x_swap_opos,4)))
					}
					else
					{
					x_result<-data.frame(x_first,x_second,x_id_norm,x_id_opos,round(x_delta,4),round(x_swap_norm,4),round(x_swap_opos,4))
					}
					}
				}
			}
			
		}

	}

colnames(x_result)<-c("first","second","id_norm","id_opos","delta","swap_norm","swap_opos")


}
# end of simple pairs

#############################################################################################################################################################################

#
#triangle
#
if(pairs_no==3)
{


 for(x_first in x_currency)
 {
  for(x_second in x_currency)
  {
   for(x_third in x_currency)
   {
   if((x_first!=x_second) & (x_first!=x_third) & (x_second!=x_third))
    {
	

#print(paste("#",x_first,x_second,x_third))		
	x_a <- swt[swt$short==x_first  & swt$long==x_second,]$id
	x_b <- swt[swt$short==x_second & swt$long==x_third,]$id
	x_c <- swt[swt$short==x_third  & swt$long==x_first,]$id

# i teraz mamy 3 wektory - zkrossowac je i podsumowac swapy i print			

#print(paste(x_first,x_second,x_norm,x_opos))		

			
	for(x_id_a in x_a)
	{
	for(x_id_b in x_b)
	{
	for(x_id_c in x_c)
	{


# do the math

#print(paste(x_first,x_second,x_id_norm,x_id_opos))		

	x_swap_a <-swt[swt$id==x_id_a,]$swap
	x_swap_b <-swt[swt$id==x_id_b,]$swap
	x_swap_c <-swt[swt$id==x_id_c,]$swap
 
	x_delta <- x_swap_a + x_swap_b + x_swap_c;
 				
	if(x_delta>=swap_level)
	{
		print(paste("SWAP ",x_first,x_second,x_third,x_id_a,x_id_b,x_id_c,round(x_delta,3),round(x_swap_a,3),round(x_swap_b,3),round(x_swap_c,3)))
		if(length(x_result)==0)
		{
			x_result<-data.frame(x_first,x_second,x_third,x_id_a,x_id_b,x_id_c,round(x_delta,4),round(x_swap_a,4),round(x_swap_b,3),round(x_swap_c,3))
		}
		else
		{
			x_result<-rbind(x_result,data.frame(x_first,x_second,x_third,x_id_a,x_id_b,x_id_c,round(x_delta,4),round(x_swap_a,4),round(x_swap_b,3),round(x_swap_c,3)))
		}			
	}

#end for x_id
	}}}

#end if		
	}

#end for x_first...
	}}}
colnames(x_result)<-c("first","second","third","id_a","id_b","id_c","delta","swap_a","swap_b","swap_c")
}
# end of triangle pairs


#############################################################################################################################################################################
#
#square
#
if(pairs_no==4)
{


 for(x_first in x_currency)
 {
  for(x_second in x_currency)
  {
   for(x_third in x_currency)
   {
   for(x_forth in x_currency)
   {
   if((x_first!=x_second) & (x_first!=x_third) & (x_first!=x_forth) & (x_second!=x_third) & (x_second!=x_forth) & (x_third!=x_forth))
    {
	

	x_a <- swt[swt$short==x_first  & swt$long==x_second,]$id
	x_b <- swt[swt$short==x_second & swt$long==x_third,]$id
	x_c <- swt[swt$short==x_third  & swt$long==x_forth,]$id
	x_d <- swt[swt$short==x_forth  & swt$long==x_first,]$id

# i teraz mamy 4 wektory - zkrossowac je i podsumowac swapy i print			

			
	for(x_id_a in x_a)
	{
	for(x_id_b in x_b)
	{
	for(x_id_c in x_c)
	{
	for(x_id_d in x_d)
	{


# do the math

	x_swap_a <-swt[swt$id==x_id_a,]$swap
	x_swap_b <-swt[swt$id==x_id_b,]$swap
	x_swap_c <-swt[swt$id==x_id_c,]$swap
	x_swap_d <-swt[swt$id==x_id_d,]$swap
 
	x_delta <- x_swap_a + x_swap_b + x_swap_c + x_swap_d;
 				
	if(x_delta>=swap_level)
	{
		print(paste("SWAP :",round(x_delta,3),x_first,x_second,x_third,x_forth,x_id_a,x_id_b,x_id_c,x_id_d,round(x_swap_a,3),round(x_swap_b,3),round(x_swap_c,3),round(x_swap_c,3),round(x_delta,3)))
		if(length(x_result)==0)
		{
			x_result<-data.frame(x_first,x_second,x_third,x_forth,x_id_a,x_id_b,x_id_c,x_id_d,round(x_swap_a,3),round(x_swap_b,3),round(x_swap_c,3),round(x_swap_c,3),round(x_delta,3))
		}
		else
		{
			x_result<-rbind(x_result,data.frame(x_first,x_second,x_third,x_forth,x_id_a,x_id_b,x_id_c,x_id_d,round(x_swap_a,3),round(x_swap_b,3),round(x_swap_c,3),round(x_swap_c,3),round(x_delta,3)))
		}			
	}

#end for x_id
	}}}}

#end if		
	}

#end for x_first...
	}}}}
colnames(x_result)<-c("first","second","third","forth","id_a","id_b","id_c","id_d","swap_a","swap_b","swap_c","swap_d","delta")
}
# end of square pairs


return(x_result)
};

soni.pos<-function(){

btm<-read.csv("C:\\Users\\Admin\\AppData\\Roaming\\MetaQuotes\\Terminal\\FBD1ABB2416ED44D582300B93B9AA1B7\\MQL4\\Files\\ACC.999007692.pos.csv")
bx1<-read.csv("C:\\Users\\Admin\\AppData\\Roaming\\MetaQuotes\\Terminal\\52055CD7446FBBA46EE1D618765CC74A\\MQL4\\Files\\ACC.899456.pos.csv")


#tx1
res1<-data.frame("tx.1","[bund-buxl swap @tm]",
	sum(btm[btm$symbol=="#EURBUND" & btm$size<20,]$swap), sum(btm[btm$symbol=="#EURBUND" & btm$size<20,]$commission), sum(btm[btm$symbol=="#EURBUND" & btm$size<20,]$profit),
	sum(btm[btm$symbol=="#EURBUXL" & btm$size<20,]$swap), sum(btm[btm$symbol=="#EURBUXL" & btm$size<20,]$commission), sum(btm[btm$symbol=="#EURBUXL" & btm$size<20,]$profit)
)


#tx2
res2<-data.frame("tx.2","[bund swap+arb @tm-xtb]",
           sum(btm[btm$symbol=="#EURBUND" & btm$size>80,]$swap),  sum(btm[btm$symbol=="#EURBUND" & btm$size>80,]$commission), sum(btm[btm$symbol=="#EURBUND" & btm$size>80,]$profit),
           sum(bx1[bx1$symbol=="BUND10Y.." & bx1$size<1,]$swap), sum(bx1[bx1$symbol=="BUND10Y.." & bx1$size<1,]$commission), sum(bx1[bx1$symbol=="BUND10Y.." & bx1$size<1,]$profit)
)

colnames(res1) <- c("tx","desc","a.swap","a.comm","a.profit","b.swap","b.comm","b.profit")
colnames(res2) <- c("tx","desc","a.swap","a.comm","a.profit","b.swap","b.comm","b.profit")

res<-rbind(res1, res2)



res$swap<-res$a.swap+res$b.swap
res$comm<-res$a.comm+res$b.comm
res$profit<-res$a.profit+res$b.profit

res$a.result=res$a.swap+res$a.comm+res$a.profit
res$b.result=res$b.swap+res$b.comm+res$b.profit
res$result<-res$a.result + res$b.result


colnames(res) <- c("tx","desc","a.swap","a.comm","a.profit","b.swap","b.comm","b.profit","swap","comm","profit","a.result","b.result","result")




return (0)
};



