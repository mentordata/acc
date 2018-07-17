loadopt<-function(name){

dir_path<-"d:\\acc\\tickmill_data\\synt_option."

x_option <- data.frame()

if(file.exists(paste(dir_path,name,".csv",sep="")))
 {
	x_option <- read.csv(paste(dir_path,name,".csv",sep=""))
	
 }

x_option$result <- x_option$loss + x_option$swap + x_option$comm + x_option$profit  

return(x_option)

};

option_list<-function(arg_name){

x_result <-data.frame()

 for(idx in 1:length(arg_name))
  { 

	name <- arg_name[idx]

	x_opt<-loadopt(name)
	

	if(length(x_opt)>0)	
	{
		x_result<-rbind(x_result,x_opt)
	}
  }

x_tmp<-x_result[x_result$status!="CLOSED",]

print(paste("floating P/L: ",sum(x_tmp$result),sep=""))

x_tmp<-x_result[x_result$status=="CLOSED",]

print(paste("closed P/L: ",sum(x_tmp$result),sep=""))


return(x_result)
};






load_data<-function(name,spread){

dir_path<-"d:\\acc\\synt_option\\"

#print(paste("namex:",paste(dir_path,name,".csv",sep=""),sep=""))

x_data<-read.csv(paste(dir_path,name,".csv",sep=""))

#print(paste("namex2:",paste(dir_path,name,".csv",sep=""),sep=""))

colnames(x_data) <-c("date","time","open","high","low","close","volume")

x_data$scope <- x_data$high - x_data$low



x_result <- data.frame(name,mean(x_data$high),mean(x_data$scope),(mean(x_data$scope)/mean(x_data$high)),(mean(x_data$scope)/spread))

colnames(x_result) <-c("name","price","scope","vol_price","vol_spread")




return(x_result)
};

load_full<-function(arg_name, arg_spread){
dir_path<-":"


x_result <-data.frame()

 for(idx in 1:length(arg_name))
  { 

	name <- arg_name[idx]
	spread <-arg_spread[idx]
	x_min <- load_data(paste(name,"1",sep=""),spread)

	x_hour <- load_data(paste(name,"60",sep=""),spread)

	x_day <- load_data(paste(name,"1440",sep=""),spread)

	x_row <-data.frame(name,spread,x_min$price,x_min$scope, x_min$vol_price, x_min$vol_spread,x_hour$price,x_hour$scope, x_hour$vol_price, x_hour$vol_spread,x_day$price,x_day$scope, x_day$vol_price, x_day$vol_spread)
	
	x_result<-rbind(x_result,x_row)

  }



colnames(x_result) <-c("name","spread","min_price","min_scope","min_vol_price","min_vol_spread","hour_price","hour_scope","hour_vol_price","hour_vol_spread","day_price","day_scope","day_vol_price","day_vol_spread")

return(x_result)

};


index_data<-function(arg_name){
x_data<-read.csv(arg_name)

colnames(x_data)<-c("date","time","open","high","low","close","vol")
x_data$scope<-(x_data$high-x_data$low)

factor(x_data$time)

x_min<-aggregate(x_data$scope,by=list(time=x_data$time),FUN=mean)
colnames(x_min)<-c("time","mean")

x_min$med<-aggregate(x_data$scope,by=list(time=x_data$time),FUN=median)$x
x_min$min<-aggregate(x_data$scope,by=list(time=x_data$time),FUN=min)$x
x_min$max<-aggregate(x_data$scope,by=list(time=x_data$time),FUN=max)$x
x_min$sd<-aggregate(x_data$scope,by=list(time=x_data$time), FUN=sd)$x

x_min$price_mean<-aggregate((x_data$open+x_data$close+x_data$low+x_data$high)/4,by=list(time=x_data$time), FUN=mean)$x
x_min$price_med<-aggregate((x_data$open+x_data$close+x_data$low+x_data$high)/4,by=list(time=x_data$time), FUN=median)$x


return(x_min)
};


index_plot<-function(arg_name){
# funkcja liczy/wyswietla zmiennosc

 for(idx in 1:length(arg_name))
  { 
	x_name<-arg_name[idx]

	x_min<-index_data(paste("d:\\acc\\synt_option\\",x_name,"1.csv",sep=""))

	dev.new()
	plot(x_min$mean,type="l",las=2,col=2,xaxt="n",xlab=x_name)
	axis(1, at=seq(0,length(x_min$time)-1, by=length(x_min$time)/24),labels=0:23,las=2,cex.lab=0.5)
	points(x_min$med,type="l",col=3)
  };

};


index2_plot<-function(arg_name){
# funkcja liczy/wyswietla cene

 for(idx in 1:length(arg_name))
  { 
	x_name<-arg_name[idx]

	x_min<-index_data(paste("d:\\acc\\synt_option\\",x_name,"1.csv",sep=""))

	dev.new()
	plot(x_min$price_mean,type="l",las=2,col=2,xaxt="n",xlab=x_name,ylab="mean+median",cex.main=0.5,ylim=range(x_min$price_mean, x_min$price_med))
	axis(1, at=seq(0,length(x_min$time)-1, by=length(x_min$time)/24),labels=0:23,las=2,cex.lab=0.5)
	points(x_min$price_med,type="l",col=3)

  };

};

###############################################################
indexw_data<-function(arg_name){

x_data<-read.csv(arg_name)

colnames(x_data)<-c("date","time","open","high","low","close","vol")
x_data$scope<-(x_data$high-x_data$low)

x_data$date<-gsub("\\.","-",x_data$date)
x_data$wday<-as.POSIXlt(x_data$date)$wday

x_data$wday_hour<-paste(x_data$wday,"_",substr(x_data$time,1,2),sep="")


factor(x_data$wday_hour)

x_min<-aggregate(x_data$scope,by=list(time=x_data$wday_hour),FUN=mean)
colnames(x_min)<-c("time","mean")

x_min$med<-aggregate(x_data$scope,by=list(time=x_data$wday_hour),FUN=median)$x
x_min$min<-aggregate(x_data$scope,by=list(time=x_data$wday_hour),FUN=min)$x
x_min$max<-aggregate(x_data$scope,by=list(time=x_data$wday_hour),FUN=max)$x
x_min$sd<-aggregate(x_data$scope,by=list(time=x_data$wday_hour), FUN=sd)$x
x_min$price<-aggregate((x_data$open+x_data$close+x_data$high+x_data$low)/4,by=list(time=x_data$wday_hour),FUN=mean)$x
x_min$price_med<-aggregate((x_data$open+x_data$close+x_data$high+x_data$low)/4,by=list(time=x_data$wday_hour),FUN=median)$x



return(x_min)
};


indexw_plot<-function(arg_name){

for(idx in 1:length(arg_name)){

	name<-arg_name[idx]
	x_min<-indexw_data(paste("d:\\acc\\synt_option\\",name,"60.csv",sep=""))

	dev.new()
	plot(x_min$price,type="o",pch=18,las=2,col=2,ylab="",yaxt="n", xaxt="n",xlab=name,ylim=range(x_min$price,x_min$price_med),xaxp=c(1,120,5),
	yaxp=c(substr(range(x_min$price,x_min$price_med)[1],1,7),substr(range(x_min$price,x_min$price_med)[2],1,7),9))

	

	abline(v=0:4*24,h=seq(range(x_min$price,x_min$price_med)[1],range(x_min$price,x_min$price_med)[2],length.out=10),  col="grey")
	axis(1, at=0:4*24,labels=c("pon","wto","sro","czw","pia"))
	axis(2,
		at=seq(range(x_min$price,x_min$price_med)[1],range(x_min$price,x_min$price_med)[2],length.out=10), 
		labels=substr(seq(range(x_min$price,x_min$price_med)[1],range(x_min$price,x_min$price_med)[2],length.out=10),1,7),
		las=2
		)

	points(x_min$price_med,type="o",pch=18,col=3)
}
};


get_trades<-function(){

x_data<-read.csv("d:\\acc\\tickmill_data\\trades.csv")

return(x_data)
};

get_stats<-function(){
x_data<-get_trades()
plot()
x_result<-aggregate(x_data$result,by=list(date=substr(x_data$time,1,10)),FUN=sum)
colnames(x_result)<-c("date","result")

x_result$mean<-round(aggregate(x_data$result,by=list(date=substr(x_data$time,1,10)),FUN=mean)$x,4)
x_result$sd<-round(aggregate(x_data$result,by=list(date=substr(x_data$time,1,10)),FUN=sd)$x,4)
x_result$count<-aggregate(x_data$result,by=list(date=substr(x_data$time,1,10)),FUN=length)$x
x_result$saldo<-cumsum(x_result$result)
x_result$sharpee<-round(x_result$mean/x_result$sd,2)


return(x_result)
}
