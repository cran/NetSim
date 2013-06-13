\name{NetSim-package}
\alias{NetSim-package}
\alias{NetSim}
\docType{package}
\title{
NetSim: A Social Networks Simulation Tool in R.
}
\description{
NetSim allows to combine and simulate a variety of micro-models to 
research their impact on the macro-features of social networks.
}
\details{
\tabular{ll}{
Package: \tab NetSim\cr
Type: \tab Package\cr
Version: \tab 0.9\cr
Date: \tab 2013-06-13\cr
License: \tab GPL-2\cr
}
Simulation models in \pkg{NetSim} are based on model sequences including the following steps\cr
1. Determine the time span until the next change occurs\cr
2. Apply change models based on the time span determined\cr
3. Determine changes based on the updated states\cr
4. Apply these changes

There may be several of such model chains running simultaneously. The comparison of 
step 1 of the models returns which action chain is started. 
}
\author{
Christoph Stadtfeld\cr
Maintainer: Christoph Stadtfeld <c.stadtfeld@rug.nl>
}
\references{
If you use NetSim for publications, please cite \cr
Christoph Stadtfeld (2013): \strong{NetSim: A Social Networks Simulation Tool in R}
URL: http://www.christoph-stadtfeld.com/netsim \cr
(check the web site for updated references)
}
%%~~ Optionally other standard keywords, one per line, from file KEYWORDS in ~~
%%~~ the R documentation directory ~~
\keyword{
Social Network
Simulation
Markov process
Social Science
}
%\seealso{
%}
%\examples{
%% ~~ simple examples of the most important functions ~~
%}
