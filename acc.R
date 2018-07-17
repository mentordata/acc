acc<-function(name){

  if(name!="")
	{
	result<-read.csv(paste("d:\\acc\\acc_client.",name,".log",sep=""),header=FALSE)
	
#	colnames(result)<-c("seq","time","symbol","side","sign","result","a","b","c","d","min_loss","slip","sec","ticket","profit_slip","open","close","q","w","e","r")
	colnames(result)<-c("seq","time","symbol","side","sign","result","min","max","p0","p1","p2","p3","p4","p5","p6","p7","p8")

	result$mins<-(as.integer(substr(result$time,10,13)) +as.integer(substr(result$time,1,8))*10000)
	return(result)
	}
}
idx<-function(name){

  if(name!="")
	{
	result<-read.csv(paste("d:\\acc\\ana.1\\",name,".data",sep=""))
	
	colnames(result)<-c("seq","tick","time","symbol","bid","ask")

	result$minutes<-(as.integer(substr(result$time,10,12))*10 +as.integer(substr(result$time,1,8))*10000)
	return(result)
	}
}


acc.plot<-function(name1="", name2="", name3="", name4="", name5="", name6="", name7="", name8=""){


max_y<-max(cumsum(acc(name1)$result),cumsum(acc(name2)$result),cumsum(acc(name3)$result),cumsum(acc(name4)$result),cumsum(acc(name5)$result),cumsum(acc(name6)$result),cumsum(acc(name7)$result),cumsum(acc(name8)$result))
min_y<-min(cumsum(acc(name1)$result),cumsum(acc(name2)$result),cumsum(acc(name3)$result),cumsum(acc(name4)$result),cumsum(acc(name5)$result),cumsum(acc(name6)$result),cumsum(acc(name7)$result),cumsum(acc(name8)$result))
lim_x<-max(nrow(acc(name1)),nrow(acc(name2)),nrow(acc(name3)),nrow(acc(name4)),nrow(acc(name5)),nrow(acc(name6)),nrow(acc(name7)),nrow(acc(name8)))

plot(cumsum(acc(name1)$result), xlab="Transaction", ylab="CumResult", ylim=c(min_y-20, max_y+20),xlim=c(-5,lim_x), col=2,type="b",pch=1)

if(name2!="")
 points(cumsum(acc(name2)$result),col=3,type="b",pch=2)

if(name3!="")
 points(cumsum(acc(name3)$result),col=4,type="b", pch=3)

if(name4!="")
 points(cumsum(acc(name4)$result),col=6,type="b", pch=4)

if(name5!="")
 points(cumsum(acc(name5)$result),col=5,type="b",pch=5)

if(name6!="")
 points(cumsum(acc(name6)$result),col=8,type="b",pch=6)

if(name7!="")
 points(cumsum(acc(name7)$result),col=9,type="b",pch=7)

if(name8!="")
 points(cumsum(acc(name8)$result),col=10,type="b",pch=8)


legend(-5,0,c(name1,name2,name3,name4,name5,name6,name7,name8),text.col=c(2,3,4,6,5,8,9,10), pch=c(1,2,3,4,5,6,7,8))

abline(h=0)

}


acc.info<-function(name=""){

cat("SUMMARY for WIN\n    Min.  1stQu.   Median     Mean  3rdQu.     Max. \n")

tmp<-acc(name)

cat("seconds to close\n")
cat(summary(tmp[tmp$result>0,]$sec))

cat("\nresult\n")
cat(summary(tmp[tmp$result>0,]$result))

cat("\nmin_loss\n")
cat(summary(tmp[tmp$result>0,]$min_loss),"\n")



return(0)
}




kp.plot<-function(ds){
month<-unique(ds$month)


plot(ds[ds$status=="FOAM",]$number,x=month,xlab="Miesiac",ylab="Komplety", ylim=c(0,max(ds[ds$status!="DONE" & ds$status!="REJECTED",]$number)), col=1,pch=1,type="b")

points(ds[ds$status=="FRAME",]$number,x=month, col=2,pch=2,type="b")

points(ds[ds$status=="MONTAGE",]$number,x=month, col=3,pch=3,type="b")

#points(ds[ds$status=="REJECTED",]$number,x=month, col=4,pch=4,type="b")

points(ds[ds$status=="SEWING",]$number,x=month, col=5,pch=5,type="b")

points(ds[ds$status=="STOFF",]$number,x=month, col=6,pch=6,type="b")

points(ds[ds$status=="WAREHOUSE",]$number,x=month, col=8,pch=7,type="b")

points(ds[ds$status=="BROKEN",]$number,x=month, col=9,pch=8,type="b")

points(ds[ds$status=="CUTTER",]$number,x=month, col=10,pch=9,type="b")

legend(10,max(ds[ds$status!="DONE" & ds$status!="REJECTED",]$number),
  c("Pianka","Stelaz","Tapicerowanie","Produkcja anulowana","Szwalnia","Tkanina/Kolor","Magazyn","Uszkodzony","Zle wyciete"),text.col=c(1,2,3,4,5,6,8,9,10), pch=c(1,2,3,4,5,6,7,8,9))


return(0)
};


idx.plot<-function(name1="", name2="", name3="", name4="", name5="", name6="", name7="", name8=""){


max_y<-max(name1$ask,name2$ask,name3$ask,name4$ask,name5$ask,name6$ask,name7$ask,name8$ask)
min_y<-max(name1$bid,name2$bid,name3$bid,name4$bid,name5$bid,name6$bid,name7$bid,name8$bid)
plot(name1$ask, xlab="Seq", ylab="price", ylim=c(min_y-20, max_y+20),xlim=c(-5,lim_x), col=2,type="l")

if(name2!="")
 points(name2$ask,col=3,type="l")

if(name2!="")
 points(name2$ask,col=3,type="l")

if(name2!="")
 points(name2$ask,col=3,type="l")

if(name2!="")
 points(name2$ask,col=3,type="l")

if(name2!="")
 points(name2$ask,col=3,type="l")

if(name2!="")
 points(name2$ask,col=3,type="l")

if(name2!="")
 points(name2$ask,col=3,type="l")

legend(-5,0,c(name1,name2,name3,name4,name5,name6,name7,name8),text.col=c(2,3,4,6,5,8,9,10))

abline(h=0)

}

mary<-function(){

	result<-read.csv("d:\\acc\\mary.00\\mary00.csv",sep=" ")

result$result<-result$profit+result$commission+result$swap

temp<-result[order(result$close,result$timeclose),]
temp$cum<-cumsum(temp$result)
return (temp)

}


mary.day<-function(){

result<-read.csv("d:\\acc\\mary.00\\mary00.csv",sep=" ")

result$result<-result$profit+result$commission+result$swap

temp<-result[order(result$close,result$timeclose),]

temp2_day<-aggregate(temp$result,by=list(temp$close),FUN=sum)
temp2_day$percent<-round((temp2_day$x/9500)*100,1)
colnames(temp2_day)<-c("date","result","roi")
#temp2_day$trades<-nrow(temp[temp$close == temp2_day$date,])
temp2_day$cum<-cumsum(temp2_day$result)
#plot(temp2_day$cum,col=2,pch=3, type="b")
return (temp2_day)

}

mary.plot<-function(){

plot(cumsum(mary()$result), col=2, type="b")
abline(h=c(-1000,-100,0,100,1000),col=3)
stats <- c(mean(mary.day()$roi)/sd(mary.day()$roi),sum(mary()$result),(sum(mary()$result)/9500)*100)
names(stats)<-c("sharpe","total","roi")
return(round(stats,2))
}


lisa.find<-function(name1,name2){

first<-read.csv(paste("d:\\acc\\lisa.00\\",name1,".csv",sep=""))
second<-read.csv(paste("d:\\acc\\lisa.00\\",name2,".csv",sep=""))

# make sd
sd1<-sd(first$close)
sd2<-sd(second$close)

first$cnorm<-first$close/sd1
second$cnorm<-second$close/sd2

# what is a mean ?
mean1<-mean(first$close)
mean2<-mean(second$close)

first$cnorm<-first$close-mean1
second$cnorm<-second$close-mean2

max_y<-max(max(first$cnorm),max(second$cnorm))

min_y<-min(min(first$cnorm),min(second$cnorm))

plot(first$cnorm,ylim=c(min_y, max_y),col=3,type="l")
points(second$cnorm,col=4,type="l")

legend(-5,0,c(name1,name2),text.col=c(3,4))

return(0)
}




moni<-function(){

	result<-read.csv("d:\\acc\\ana.1\\moni.2018.csv",sep=",")

temp<-result[order(result$date),]
temp$cum<-cumsum(temp$result)
return (temp)

}


moni.day<-function(){

result<-read.csv("d:\\acc\\ana.1\\moni.2018.csv",sep=",")

temp<-result[order(result$date),]

temp2_day<-aggregate(temp$result,by=list(temp$date),FUN=sum)
temp2_day$percent<-round((temp2_day$x/7200)*100,1)
colnames(temp2_day)<-c("date","result","roi")
#temp2_day$trades<-nrow(temp[temp$date == temp2_day$date,])
temp2_day$cum<-cumsum(temp2_day$result)

temp2_day$cumroi<-cumsum(temp2_day$roi)

#plot(temp2_day$cum,col=2,pch=3, type="b")
return (temp2_day)

}

moni.plot<-function(){

plot(cumsum(moni()$result), col=2, type="b")
abline(h=c(-1000,-100,0,100,1000),col=3)
stats <- c(mean(moni.day()$roi)/sd(moni.day()$roi),sum(moni()$result),(sum(moni()$result)/7200)*100)
names(stats)<-c("sharpe","total","roi")
return(round(stats,2))
}

