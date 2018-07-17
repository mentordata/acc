#MentorData 2017-2018
#Swap Inter Broker Arbitrage
#

siba.symbol_normalize <-function(x_data_in){
  
  x_data <-x_data_in
  
  #zachowanie oryginalnego symbolu brokera
  x_data$broker_symbol<-x_data$symbol
  
  # wywalenie znakow specjalnych
  x_data$symbol <- gsub("\\.f","",x_data$symbol,ignore.case = TRUE)
  x_data$symbol <- gsub("\\.stp","",x_data$symbol,ignore.case = TRUE)
  x_data$symbol <- gsub("[^a-zA-Z0-9]","",x_data$symbol)
  
  # ujednolicenie bulionu (bulion to metale szlachetne, podstawowe surowce, glowne indexy)
  x_data$symbol <- gsub("STOXX50.*","EU50",x_data$symbol)
  x_data$symbol <- gsub("SILVER.*","XAGUSD",x_data$symbol)
  x_data$symbol <- gsub("GOLD.*","XAUUSD",x_data$symbol)
  x_data$symbol <- gsub("USTEC.*","US100",x_data$symbol)
  x_data$symbol <- gsub("STOXX50","EU50",x_data$symbol)
  x_data$symbol <- gsub("WTI.*","WTI",x_data$symbol)
  x_data$symbol <- gsub("COPPER.*","COPPER",x_data$symbol)
  x_data$symbol <- gsub("COFFEEROB.*","ROBUSTA",x_data$symbol)
  x_data$symbol <- gsub("COFFEEARA.*","ARABICA",x_data$symbol)
  x_data$symbol <- gsub("COFF.*","ARABICA",x_data$symbol)
  x_data$symbol <- gsub("PLATIN.*","PLATINUM",x_data$symbol)
  
  x_data$symbol <- gsub("EUROBUND","EURBUND",x_data$symbol)
  x_data$symbol <- gsub("EUROBUXL","EURBUXL",x_data$symbol)
  x_data$symbol <- gsub("EUROSCHATZ","EURSCHA",x_data$symbol)
  x_data$symbol <- gsub("EUROBOBL","EURBOBL",x_data$symbol)
  x_data$symbol <- gsub("AUS.*","AUS200",x_data$symbol)
  x_data$symbol <- gsub("CANADA.*","CAD60",x_data$symbol)
  x_data$symbol <- gsub("EURO50","EU50",x_data$symbol)
  x_data$symbol <- gsub("FRANC.*","FRA40",x_data$symbol)
  x_data$symbol <- gsub("GERMANY30","DE30",x_data$symbol)
  x_data$symbol <- gsub("GERMANYM.*","DE50",x_data$symbol)
  x_data$symbol <- gsub("GERMANYT.*","DETEC30",x_data$symbol)        
  x_data$symbol <- gsub("HONGKONG.*","HK50",x_data$symbol)
  x_data$symbol <- gsub("HONGKONGCHINA.*","HKSH",x_data$symbol)
  x_data$symbol <- gsub("ITAL.*","ITA40",x_data$symbol)
  x_data$symbol <- gsub("JAPA.*","JP225",x_data$symbol)
  x_data$symbol <- gsub("NETHER.*","NLD25",x_data$symbol)
  x_data$symbol <- gsub("NORWA.*","NOR25",x_data$symbol)
  x_data$symbol <- gsub("SOUTHAFR.*","AFRICA40",x_data$symbol)
  x_data$symbol <- gsub("SPAIN.*","SPA35",x_data$symbol)
  x_data$symbol <- gsub("SWED.*","SWE30",x_data$symbol)
  x_data$symbol <- gsub("SWITZ.*","SUI20",x_data$symbol)
  x_data$symbol <- gsub("BRENT.*","BRENT",x_data$symbol)
  x_data$symbol <- gsub("ROPAB.*","BRENT",x_data$symbol)
  x_data$symbol <- gsub("ROPAC.*","WTI",x_data$symbol)    
  x_data$symbol <- gsub("COCOA(USA).*","COCOA.US",x_data$symbol)
  x_data$symbol <- gsub("COCOA(UK).*","COCOA.UK",x_data$symbol)
  x_data$symbol <- gsub("USNDAQ.*","US100",x_data$symbol)
  x_data$symbol <- gsub("USSMALLCAP.*","US2000",x_data$symbol)
  x_data$symbol <- gsub("USSPX.*","US500",x_data$symbol)
  x_data$symbol <- gsub("BUND.*","EURBUND",x_data$symbol)
  x_data$symbol <- gsub("LIVESTOCKL.*","CATTLE",x_data$symbol)
  x_data$symbol <- gsub("CH20","SUI20",x_data$symbol)
  x_data$symbol <- gsub("ES35","SPA35",x_data$symbol)
  x_data$symbol <- gsub("FR40","FRA40",x_data$symbol)
  x_data$symbol <- gsub("GASOLINE","PETROL",x_data$symbol)
  x_data$symbol <- gsub("IT40","ITA40",x_data$symbol)
  x_data$symbol <- gsub("NATURALGAS","NATGAS",x_data$symbol)
  x_data$symbol <- gsub("NL25","NLD25",x_data$symbol)
  x_data$symbol <- gsub("OILBRENT","BRENT",x_data$symbol)
  x_data$symbol <- gsub("OILs","BRENT",x_data$symbol)
  x_data$symbol <- gsub("OILWTI","WTI",x_data$symbol)
  x_data$symbol <- gsub("PALLAD","PALLADIUM",x_data$symbol)
  x_data$symbol <- gsub("PL20","PLN20",x_data$symbol)
  x_data$symbol <- gsub("SCHATZ","EURSCHA",x_data$symbol)
  x_data$symbol <- gsub("SE30","SWE30",x_data$symbol)
  x_data$symbol <- gsub("SOYAMEAL","SOYMEAL",x_data$symbol)
  x_data$symbol <- gsub("SOYAOIL","SOYOIL",x_data$symbol)
  x_data$symbol <- gsub("TNOTE","TNOTE10",x_data$symbol)
  x_data$symbol <- gsub("UST-NOTE10.*","TNOTE10",x_data$symbol)
  x_data$symbol <- gsub("UST-NOTE5.*","TNOTE5",x_data$symbol)
  x_data$symbol <- gsub("UST-NOTE2.*","TNOTE2",x_data$symbol)
  x_data$symbol <- gsub("SWISS.*","SUI20",x_data$symbol)
  x_data$symbol <- gsub("ESP35","SPA35",x_data$symbol)
  x_data$symbol <- gsub("SPX500","US500",x_data$symbol)
  x_data$symbol <- gsub("NAS100","US100",x_data$symbol)
  x_data$symbol <- gsub("EU50.*","EU50",x_data$symbol)
  x_data$symbol <- gsub("FRA40.*","FRA40",x_data$symbol)
  x_data$symbol <- gsub("FRA.40.*","FRA40",x_data$symbol)
  x_data$symbol <- gsub("GER30.*","GER30",x_data$symbol)
  x_data$symbol <- gsub("GRE20.*","GRE20",x_data$symbol)
  x_data$symbol <- gsub("HK50.*","HK50",x_data$symbol)
  x_data$symbol <- gsub("IT40.*","ITA40",x_data$symbol)
  x_data$symbol <- gsub("JP225.*","JP225",x_data$symbol)
  x_data$symbol <- gsub("NETH25.*","NLD25",x_data$symbol)
  x_data$symbol <- gsub("POL20.*","PLN20",x_data$symbol)
  x_data$symbol <- gsub("SING.*","SING",x_data$symbol)
  x_data$symbol <- gsub("SPAIN35.*","SPA35",x_data$symbol)
  x_data$symbol <- gsub("SWI20.*","SUI20",x_data$symbol)
  x_data$symbol <- gsub("UK100.*","UK100",x_data$symbol)
  x_data$symbol <- gsub("US100.*","US100",x_data$symbol)
  x_data$symbol <- gsub("US30.*","US30",x_data$symbol)
  x_data$symbol <- gsub("US500.*","US500",x_data$symbol)
  x_data$symbol <- gsub("SUI20.*","SUI20",x_data$symbol)
  x_data$symbol <- gsub("COTTONs","COTTON",x_data$symbol)
  x_data$symbol <- gsub("SUGARs","RAWSUGAR",x_data$symbol)
  
  
  
  
  x_data$symbol <- gsub("BUND10Y.*","EURBUND",x_data$symbol)
  
  # wywalenie zbednych symboli
  x_data_tmp <- x_data[!(x_data$symbol=="AA"),]
  x_data <- x_data_tmp[!(x_data_tmp$symbol=="RDSA"),]
  
  
  return(x_data)
}


siba.load<-function(filename){
# laduj dane oraz je wyczysc z bzdur
  
x_data_in<-read.csv(paste("d:\\acc\\siba\\",filename,".info.csv",sep=""),stringsAsFactors=FALSE)

x_data<-siba.symbol_normalize(x_data_in)

x_data$broker<-filename



# sprowadzenie INTEREST do POINT

x_data$old_swap_long <- x_data$swap_long
x_data$old_swap_short <- x_data$swap_short

x_data[x_data$swap_type=="interest",]$swap_long  <- ((x_data[x_data$swap_type=="interest",]$swap_long/36500)  * x_data[x_data$swap_type=="interest",]$bid) / x_data[x_data$swap_type=="interest",]$point_size
x_data[x_data$swap_type=="interest",]$swap_short <- ((x_data[x_data$swap_type=="interest",]$swap_short/36500) * x_data[x_data$swap_type=="interest",]$bid) / x_data[x_data$swap_type=="interest",]$point_size

if(length(x_data[x_data$swap_type=="interest",]$swap_type)>=1)
  {
  x_data[x_data$swap_type=="interest",]$swap_type <- "points"
  };


return(x_data)  

  
}

siba.screen<-function(data_set){
# wyszukuj szczelin

  x_data <- data_set[((data_set$swap_long!=0) | (data_set$swap_short!=0)),] 
  
  x_data$swap_long_org<-x_data$swap_long
  x_data$swap_short_org<-x_data$swap_short

  x_data[x_data$swap_type=="points",]$swap_long  <-x_data[x_data$swap_type=="points",]$swap_long * x_data[x_data$swap_type=="points",]$point_size
  x_data[x_data$swap_type=="points",]$swap_short <-x_data[x_data$swap_type=="points",]$swap_short * x_data[x_data$swap_type=="points",]$point_size


  x_symbol<-unique(x_data$symbol)
  
  swap_tab <- data.frame()
  
  for(idx in 1:length(x_symbol)){
    
    x_name <- x_symbol[idx]
    x_brokers <- unique(x_data[x_data$symbol==x_name,]$broker)
    
# sprawdz pary symbolow dla kazdego brokera
# jesli dochodowy - dodaj oportunity
    for(a_brk_idx in 1:length(x_brokers))
    {
      a_brk  <- x_brokers[a_brk_idx]
      
# perpetum.swap - SIBA.0        
      if ((x_data[x_data$symbol==x_name & x_data$broker==a_brk,]$swap_long + x_data[x_data$symbol==x_name & x_data$broker==a_brk,]$swap_short)>0.0)
      {
        

        a_data <- x_data[x_data$symbol==x_name & x_data$broker==a_brk,]

        bid <-a_data$bid
        
        x_long <- a_data$swap_long
        x_short<- a_data$swap_short
        
        spread_long  <- a_data$spread * a_data$point_size
        spread_short <- a_data$spread * a_data$point_size
        
        spread <- spread_long+spread_short
        profit <- x_short+x_long

        zero_day <- round(spread/profit)
        roi <- (profit/bid)*10000
        

        x_row <- data.frame(x_name, "perp.0", a_brk, a_brk, x_long, x_short, profit, spread_long, spread_short, spread, zero_day, a_data$rollover, a_data$rollover, roi )
        colnames(x_row)<-c("symbol", "name", "broker_long", "broker_short", "swap_long", "swap_short", "profit", "spread_long", "spread_short", "spread", "zero_day", "rollover_long", "rollover_short","roi") 
        
        swap_tab <- rbind(swap_tab, x_row)
        
      }

for(b_brk_idx in 1:length(x_brokers))
      {
        b_brk  <- x_brokers[b_brk_idx]
        
        a_data <- x_data[x_data$symbol==x_name & x_data$broker==a_brk,]
        b_data <- x_data[x_data$symbol==x_name & x_data$broker==b_brk,]
        
        bid <-a_data$bid
        
        x_long <- a_data$swap_long
        x_short<- b_data$swap_short

        spread_long  <- a_data$spread * a_data$point_size
        spread_short <- b_data$spread * b_data$point_size
        spread <- spread_long+spread_short
        
# single.swap - SIBA.1        
if ((x_long+x_short)>0.0)
          {
          
          profit <- x_short+x_long
          zero_day <- round(spread/profit)
          roi <- (profit/bid)*10000
          
          x_row <- data.frame(x_name, "single.1", a_brk, b_brk, x_long, x_short, profit, spread_long, spread_short, spread, zero_day, a_data$rollover, b_data$rollover,roi )
          colnames(x_row)<-c("symbol", "name", "broker_long", "broker_short", "swap_long", "swap_short", "profit", "spread_long", "spread_short", "spread", "zero_day", "rollover_long", "rollover_short","roi") 
          
          swap_tab <- rbind(swap_tab, x_row)
          
        }
        
        
# rollover.swap - SIBA.2        
if (a_data$rollover != b_data$rollover)
        {

          if ((3*x_short + x_long) > spread)
          {
            profit <- 3*x_short+x_long-spread
          
            zero_day <- 0
            roi <- (profit/bid)*10000
            
            x_row <- data.frame(x_name, "roll.2", a_brk, b_brk, x_long, 3*x_short, profit, spread_long, spread_short, spread, zero_day, a_data$rollover, b_data$rollover, roi )
          
            colnames(x_row)<-c("symbol", "name", "broker_long", "broker_short", "swap_long", "swap_short", "profit", "spread_long", "spread_short", "spread", "zero_day", "rollover_long", "rollover_short","roi") 
            swap_tab <- rbind(swap_tab, x_row)
          }
          
          if ((x_short + 3*x_long) > spread)
          {
            profit <- x_short+3*x_long-spread
            
            zero_day <- 0
            roi <- (profit/bid)*10000
            
            x_row <- data.frame(x_name, "roll.2", a_brk, b_brk, 3*x_long, x_short, profit, spread_long, spread_short, spread, zero_day, a_data$rollover, b_data$rollover, roi )
            colnames(x_row)<-c("symbol", "name", "broker_long", "broker_short", "swap_long", "swap_short", "profit", "spread_long", "spread_short", "spread", "zero_day", "rollover_long", "rollover_short","roi") 
            
           swap_tab <- rbind(swap_tab, x_row)
          }

        }

      }
    }
  }
  
  swap_tab$roi <- round(swap_tab$roi,2)
  
  colnames(swap_tab)<-c("symbol", "name", "broker_long", "broker_short", "swap_long", "swap_short", "profit", "spread_long", "spread_short", "spread", "zero_day", "rollover_long", "rollover_short","roi") 

  return(swap_tab)
  
}

siba.load_all<-function(){
  

# gi broker simulation -0.015% swap
#  x_gi<-x_tm
  
  x_tm<-siba.load("tmill")
  x_xtbplus<-siba.load("xtbplus")
  x_xtbpro<-siba.load("xtbpro")
  x_mbank<-siba.load("mbank")
  x_admiral<-siba.load("admiral")
  x_xm<-siba.load("xm")
  x_noble<-siba.load("noble")
  x_tmspro<-siba.load("tmspro")
  
  x_data<-rbind(x_tm, x_xtbpro, x_xtbplus, x_mbank,x_admiral, x_xm, x_noble, x_tmspro);

  x_data$margin <- 1
  
  x_cmc<-siba.load("cmc")
  x_gi<-siba.load("gi")
  
  
  
  x_data<-rbind(x_data, x_cmc, x_gi)
  
  
  


  return(x_data)  
}

siba.go<-function(){
  

  x_data<-siba.load_all()
  
  x_ret<-siba.screen(x_data)
  
  x_ret<-x_ret[x_ret$profit>0.0,]
  
  return(x_ret)  
}


siba.cor<-function(x_data_in){
  

  
  x_data<-x_data_in
  
#  siba.symbol_normalize(x_data_in)
  
  x_symbols<-unique(x_data$symbol)

  x_ret<-data.frame(matrix(0,length(x_symbols), length(x_symbols)))
  
  colnames(x_ret)<-x_symbols
    
  for(a_symbol_idx in 1:length(x_symbols))
  {

    a_symbol<-x_symbols[a_symbol_idx]

    for(b_symbol_idx in 1:length(x_symbols))
    {
    b_symbol<-x_symbols[b_symbol_idx]

    a_tabx<-x_data[x_data$symbol==a_symbol,]$high
    b_tabx<-x_data[x_data$symbol==b_symbol,]$high
    
#    ab_time<-a_tabx[a_tabx$time, b_tabx$time,)
      
#    a_tab<-a_tabx[a_tabx$time]
        
    x_cor<-cor(a_tabx,b_tabx)
    
    x_ret[a_symbol_idx,b_symbol_idx]<-round(x_cor*100,2)
      
      
    }
  }
  
  rownames(x_ret)<-x_symbols
  
return(x_ret)  
  
}


siba.var<-function(x_data_in, x_swap_in, x_name){
  
  
  
  x_data<-x_data_in
  
  #  siba.symbol_normalize(x_data_in)
  
  x_symbols<-unique(x_data$symbol)
  
  x_ret<-data.frame(matrix(0,length(x_symbols), 10))
  
  colnames(x_ret)<-c("symbol","cor","sd","mean","vol","swap_short","swap_long","swap_short_prc","swap_long_prc","spread")
  
    x_symbol_idx<- 1
    a_symbol<-x_name
    
    for(b_symbol_idx in 1:length(x_symbols))
    {
      b_symbol<-x_symbols[b_symbol_idx]
      
      a_tabx<-x_data[x_data$symbol==a_symbol,]$high
      b_tabx<-x_data[x_data$symbol==b_symbol,]$high
      
      #    ab_time<-a_tabx[a_tabx$time, b_tabx$time,)
      
      #    a_tab<-a_tabx[a_tabx$time]
      
      x_ret[b_symbol_idx,]$symbol<-x_symbols[b_symbol_idx]
      
      x_ret[b_symbol_idx,]$cor<-round(cor(a_tabx,b_tabx)*100,2)
      x_ret[b_symbol_idx,]$sd<-round(sd(b_tabx),5)
      x_ret[b_symbol_idx,]$mean<-round(mean(b_tabx),5)
      
      x_ret[b_symbol_idx,]$vol<-round((sd(b_tabx)/mean(b_tabx))*100,2)

      x_ret[b_symbol_idx,]$swap_short <- x_swap_in[x_swap_in$broker_symbol==b_symbol,]$swap_short
      x_ret[b_symbol_idx,]$swap_long <- x_swap_in[x_swap_in$broker_symbol==b_symbol,]$swap_long
      x_ret[b_symbol_idx,]$spread     <- round(x_swap_in[x_swap_in$broker_symbol==b_symbol,]$spread*x_swap_in[x_swap_in$broker_symbol==b_symbol,]$point_size,5)
      
      x_ret[b_symbol_idx,]$swap_short_prc <- round(((x_ret[b_symbol_idx,]$swap_short*x_swap_in[x_swap_in$broker_symbol==b_symbol,]$point_size)/x_ret[b_symbol_idx,]$mean)*100,3)
      x_ret[b_symbol_idx,]$swap_long_prc <- round(((x_ret[b_symbol_idx,]$swap_long*x_swap_in[x_swap_in$broker_symbol==b_symbol,]$point_size)/x_ret[b_symbol_idx,]$mean)*100,3)
      
    }

  return(x_ret)  
  
}


