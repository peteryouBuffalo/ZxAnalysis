# ZxAnalysis

To run this file, you need to have these files under a CMS-enabled directory on either LXPLUS or LPC. It is recommended to have it under CMSSW_10_6_4. That is what I used for this file. This program is currently hard-coded for 2016 MC samples. To run the analysis of the files, use the command line "make". This will produce an executable named "main". Run this with the following command:

./main initEntry finalEntry

This will run the file over the given entry numbers. The results will be put into a directory called "results". Within "results" is a macro called MetPlot.C which will generate a MET plot and ROC curve for the data analyzed in the main executable. To run the macro, load it into root and then use the following command:

MetPlot("out.root")
