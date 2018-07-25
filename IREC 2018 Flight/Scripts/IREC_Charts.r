
library(data.table)
library(ggplot2)

bme280 <- fread("../IREC 2018 RF Data/BME280_Data.csv")
alt <- fread("../IREC 2018 RF Data/StratoLogger_Data.csv")

p <- ggplot(data=bme280, aes(y=Temperature, x=Delta_Time)) +
geom_line() +
geom_point() +
ggtitle("Temperature VS Time") +
xlab("Time (Seconds)") +
ylab("Temperature (C)")

ggsave(p, file="Temperature.png")

p <- ggplot(data=bme280, aes(y=Humidity, x=Delta_Time)) +
geom_line() +
geom_point() +
ggtitle("Humidity VS Time") +
xlab("Time (Seconds)") +
ylab("Humidity (%)")

ggsave(p, file="Humidity.png")

p <- ggplot(data=bme280, aes(y=Pressure, x=Delta_Time)) +
geom_line() +
geom_point() +
ggtitle("Pressure VS Time") +
xlab("Time (Seconds)") +
ylab("Pressure (kPa)")

ggsave(p, file="Pressure.png")