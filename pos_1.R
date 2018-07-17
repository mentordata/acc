pos<-read.fwf("d:\\acc\\run.5\\r_5.txt",widths=c(10,-4,4,-2,8,-3,6,-5,10,10,10,10,8,8,8,8))
names(pos)<-c("seq","side","broker","symbol","open","opened","close","closed","comm","profit","swap","result")
