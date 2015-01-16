cal_sigma_ion <- function(energy, ne, Temin){
  re <- 2.8179403267e-13 #electron classical radius, in cm
  
  me <- 0.510998928  #electron mass, in MeV
  mc <- 12*931.494061
  z <- 6
  
  E <- energy + mc
  gamma <- E/mc
  beta2 <- 1.0 - 1.0/(gamma*gamma)
  
  Temax <- cal_Temax(energy)
  
  #    plot(energy, Temax)
  
  output <- 2*pi*re*re*me*ne*z*z/beta2*((-1.0/Temax - beta2/Temax*log(Temax) + Temax/2/(E*E)) 
                                    - (-1.0/Temin - beta2/Temax*log(Temin) + Temin/2/(E*E)) )
  output[Temax <= Temin] = 0
  return(output)
}

cal_Temax <- function(energy){
  me <- 0.510998928 
  mc <- 12*931.494061
  memp <- me / mc    #ratio between me and proton mass
  
  E <- energy + mc
  gamma <- E/mc
  #  beta2 <- 1.0 - 1.0./(gamma.*gamma)
  
  output <- 2*me*(gamma*gamma-1)/(1+2*gamma*memp + memp*memp) 
  return(output)
}

energy <- seq(from = 0.5, to = 600.0**12, by = 0.5)
Temin <- 0.1
mc <- 12*931.494061
me <- 0.510998928  
z <- 6
ne <- 3.342774e23 
sigma <- NULL
sigma$energy <- energy
sigma$sigIon <- cal_sigma_ion(energy = energy, ne = ne, Temin = Temin)
sigma$sigPPE <- 0
sigma$sigPOE <- 0
sigma$sigPOI <- 0

write.table(x = sigma, file = "C:/Users/S158879/workspace/ProtonTransport/input/carbon.crossSection", row.names = F, sep = '\t\t')
