cal_rstp <- function(energy, Temin){
##  Summary of this function goes here
##   Detailed explanation goes here
  
  I <- 72.35e-6  #mean exitation enery of water, in MeV
  ne <- 3.342774e23   # for water, in 1/cm^3
  re <- 2.8179403267e-13 #electron classical radius, in cm
  
  E <- energy + mc
  gamma <- E/mc
  beta2 <- 1.0 - 1.0/(gamma*gamma)
  
  
  Temax <- cal_Temax(energy)
  Tup <- pmin(Temin, Temax)
  #  plot(energy, Temax)
  
  
  # density corretction, ONLY works for water
  hnu <- sqrt(4*pi*ne*re^3)*me*137
  C <- 1+ 2*log(I/hnu)
  xa <- C/4.606
  a <- 4.606*(xa - 0.2)/(2-0.2)^3
  x <- log(gamma*gamma*beta2)/4.606
  delta <- rep(0, length(x))
  delta[x >= 0.2 & x <= 2] <- 4.606*x[x >= 0.2 & x <= 2] - C + a*(2 - x[x >= 0.2 & x <= 2])^3
  delta[x > 2] <- 4.606*x[x > 2] - C
  
  output <- 2*pi*re*re*me*ne*z*z/beta2*(log(2*me*beta2*gamma*gamma*Tup/I/I) 
                                   - beta2*(1+Tup/Temax) - delta)
  return(output)
}

cal_Temax <- function(energy){
  memp <- me / mc    #ratio between me and proton mass
  
  E <- energy + mc
  gamma <- E/mc
  #  beta2 <- 1.0 - 1.0./(gamma.*gamma)
  
  output <- 2*me*(gamma*gamma-1)/(1+2*gamma*memp + memp*memp) 
  return(output)
}

cal_b <- function(){
  dTp = 0.5;
  gamma <- (energy + mc)/mc
  beta2 <- 1.0 - 1.0/(gamma*gamma)
  
  C <- Lw*beta2
  dE <- dTp
  # semilogx(Ene,C)
  dCdE <- rep(0, length(C))
  dCdE[2:(length(C)-1)] <- (C[3:length(C)] - C[1:(length(C)-2)])/2/dE
  dCdE[1] <- (C[2] - C[1])/dE
  dCdE[length(C)] <- (C[length(C)] - C[length(C)-1])/dE
  b <- energy/C*dCdE

  return(b)
}

energy <- seq(from = 0.5, to = 600.0*12, by = 0.5)
Temin <- 0.1
mc <- 12*931.494061
me <- 0.510998928  
z <- 6
Lw <- cal_rstp(energy, Temin)
b <- cal_b()
rspw <- NULL
rspw$energy <- energy
rspw$Sw <- rep(0, length(energy))
rspw$Mw <- rep(0, length(energy))
rspw$Lw <- Lw
rspw$b <- b

write.table(x = rspw, file = "C:/Users/S158879/workspace/ProtonTransport/input/carbon.rspw", row.names = F, sep = '\t\t')



