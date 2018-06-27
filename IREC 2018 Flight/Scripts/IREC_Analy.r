# 
# File: IREC_Analy.r
# Date: Tuesday June 26, 2018
# Author(s):
#       Dylan R. Wagner (drw6528@rit.edu)
# Desc:
#       Script to normize data from the Hyperion Protocol
#

library(plyr)
library(data.table)
library(ggplot2)

filename <- "../IREC 2018 RF Data/IREC-2018_ACTIVE.csv"

data <- fread(filename)

plot_alt <- function(group){

    group$V7 <- as.double(group$V7)

    delt_alt <- diff(group$V7)
    delt_time <- diff(group$V6)
    
    speed <- delt_alt / delt_time
    speed <- na.omit(speed)
    speed[!is.finite(speed)] <- 0
    print(max(speed))

    group <- as.data.frame(group)

    p <- ggplot(data=group, aes(x=V6, y=V7)) +
    geom_line() +
    geom_point() +
    ggtitle("Altitude VS Time") +
    xlab("Time (Seconds)") +
    ylab("Altitude (Meters)")

    ggsave(p, file="alt.png")
}

norm_data <- function(group){
    group <- group[,which(unlist(lapply(group, function(x)!all(is.na(x)))))]
    group <- group[,-c(2,3,4,5)]

    if (group[1, "V1"] == 8) {
       group <- group[!(group$V6==0),]
    }
    
    write.csv(group, file = paste(toString(group[[1, "V1"]]), ".csv", sep = ""), row.names = FALSE)

    if(group[1, "V1"] == 5){
        plot_alt(group)
    }
}

call <- ddply(data, .(V1), norm_data)
